/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <drivers/gdb/gdb_remote.h>

static cl_device_id *in_devs;
static cl_uint in_devs_num;

static int gdb_remote_getchar(struct gdb_remote_priv *prv)
{
	unsigned char ch;

retry:
	ssize_t nrecv = recv(prv->fd_sock, &ch, sizeof(ch), 0);
	if (nrecv == -1) {
		if (errno == EINTR) {
			// Need to retry
			goto retry;
		} else {
			// Error
			log_err("failed to recv data.\n");
			return CL_OUT_OF_RESOURCES;
		}
	}

	return ch;
}

static int gdb_remote_putchar(struct gdb_remote_priv *prv, int c)
{
	unsigned char ch = c;

retry:
	ssize_t nsent = send(prv->fd_sock, &ch, sizeof(ch), 0);
	if (nsent == -1) {
		if (errno == EINTR) {
			// Need to retry
			goto retry;
		} else {
			// Error
			log_err("failed to send data.\n");
			return CL_OUT_OF_RESOURCES;
		}
	}

	return ch;
}

static cl_int gdb_remote_send(struct gdb_remote_priv *prv, const char *cmd, int ack)
{
	size_t cmdlen = strlen(cmd);
	size_t buflen = cmdlen + 10;
	char *buf = alloca(buflen);
	memset(buf, 0, buflen);

	char sum = 0;
	for (size_t i = 0; i < cmdlen; i++) {
		sum += cmd[i];
	}

	int len = snprintf(buf, buflen, "$%s#%02x", cmd, sum & 0xff);
	if (len < 0) {
		log_err("failed to snprintf.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	log_dbg("-> %s\n", buf);

resend:
	ssize_t nsent = send(prv->fd_sock, buf, len, 0);
	if (nsent != len) {
		log_err("failed to send data.\n");
		return CL_OUT_OF_RESOURCES;
	}

	if (ack) {
		int c = gdb_remote_getchar(prv);
		if (c == '+') {
			// ACK
		} else if (c == '-') {
			// NACK, should resend
			log_warn("Need to resend.\n");
			goto resend;
		} else {
			// Error
			log_warn("'%c' is neither ACK nor NACK.\n", c);
			return CL_INVALID_DEVICE;
		}
	}

	return CL_SUCCESS;
}

static cl_int gdb_remote_recv(struct gdb_remote_priv *prv, char *cmd, size_t cmdlen, int ack)
{
	const char *str_sum_recv;
	int c, sum = 0, sum_recv;

	size_t buflen = cmdlen + 10, p = 0, q = 0;
	char *buf = alloca(buflen);
	memset(buf, 0, buflen);

	c = gdb_remote_getchar(prv);
	buf[p++] = c;

	if (c == '+') {
		c = gdb_remote_getchar(prv);
		buf[p++] = c;
	}
	if (c == '$') {
		c = gdb_remote_getchar(prv);
		buf[p++] = c;
	}

	while (c != '#') {
		sum += c;

		cmd[q++] = c;
		c = gdb_remote_getchar(prv);
		buf[p++] = c;

		if (p >= buflen || q >= cmdlen) {
			log_err("Too long answer (len %d).\n", (int)cmdlen);
			return CL_OUT_OF_HOST_MEMORY;
		}
	}
	cmd[q++] = '\0';
	sum &= 0xff;

	// Checksum
	str_sum_recv = &buf[p];

	c = gdb_remote_getchar(prv);
	buf[p++] = c;
	c = gdb_remote_getchar(prv);
	buf[p++] = c;

	sscanf(str_sum_recv, "%02x", &sum_recv);

	char ack_char = '+';
	if (sum != sum_recv) {
		log_err("Mismatch checksum %02x (expected %02x).\n", sum, sum_recv);
		ack_char = '-';
	}

	log_dbg("<- %s\n", buf);

	if (ack) {
		gdb_remote_putchar(prv, ack_char);
	}

	return CL_SUCCESS;
}

static cl_int gdb_remote_probe(cl_device_id dev)
{
	struct addrinfo hints, *ai;
	int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	r = getaddrinfo(prv->node, prv->service, &hints, &ai);
	if (r) {
		log_err("failed to alloc addrinfo.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	prv->fd_sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (prv->fd_sock == -1) {
		log_err("failed to alloc socket.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	r = connect(prv->fd_sock, ai->ai_addr, ai->ai_addrlen);
	if (r != 0) {
		log_err("failed to connect to '%s:%s'.\n", prv->node, prv->service);
		return CL_OUT_OF_HOST_MEMORY;
	}

	freeaddrinfo(ai);

	/* Discard old replies */
	char tmpbuf[4096];

	gdb_remote_send(prv, "vMustReplyEmpty", 0);
	while (1) {
		gdb_remote_recv(prv, tmpbuf, sizeof(tmpbuf), 1);
		if (strcmp(tmpbuf, "") == 0) {
			break;
		}
	}

	return CL_SUCCESS;
}

static cl_int gdb_remote_remove(cl_device_id dev)
{
	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	close(prv->fd_sock);

	free(prv);
	dev->priv = NULL;

	return CL_SUCCESS;
}

static cl_int gdb_remote_reset(cl_device_id dev)
{
	char tmp[4];
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	// monitor system_reset
	r = gdb_remote_send(prv, "qRcmd,73797374656d5f7265736574", 1);
	if (r != CL_SUCCESS) {
		return r;
	}

	memset(tmp, 0, sizeof(tmp));
	r = gdb_remote_recv(prv, tmp, sizeof(tmp) - 1, 1);
	if (r != CL_SUCCESS) {
		return r;
	}
	if (strcmp(tmp, "OK") != 0) {
		log_err("Failed to reset device.\n");
		return CL_INVALID_DEVICE;
	}

	// continue
	r = gdb_remote_send(prv, "vCont;c", 0);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

static cl_int gdb_remote_read_mem(cl_device_id dev, uint64_t paddr, char *buf, uint64_t len)
{
	/* TODO: to be implemented */
	return CL_INVALID_VALUE;
}

static cl_int gdb_remote_write_mem_one(cl_device_id dev, uint64_t paddr, const char *buf, uint64_t len)
{
	char *strbuf = NULL;
	char tmp[4];
	size_t buflen = len * 2 + 64;
	cl_int r = CL_SUCCESS;
	int n;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}
	if (len > 0x7f0) {
		log_err("Too large to write len:%" PRId64 ".\n", len);
		return CL_INVALID_VALUE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	strbuf = calloc(buflen, sizeof(char));
	if (strbuf == NULL) {
		log_err("Failed to calloc string buf.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	n = snprintf(strbuf, buflen, "M%08" PRIx64 ",%" PRIx64 ":", paddr, len);
	buflen -= n;

	for (uint64_t i = 0; i < len; i++) {
		snprintf(tmp, sizeof(tmp), "%02x", buf[i] & 0xff);
		strncat(strbuf, tmp, buflen);
		buflen -= 2;
	}

	r = gdb_remote_send(prv, strbuf, 1);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	free(strbuf);
	strbuf = NULL;

	memset(tmp, 0, sizeof(tmp));
	r = gdb_remote_recv(prv, tmp, sizeof(tmp) - 1, 1);
	if (r != CL_SUCCESS) {
		goto err_out;
	}
	if (strcmp(tmp, "OK") != 0) {
		log_err("Failed to memory write @%08" PRIx64 ".\n", paddr);
		r = CL_INVALID_DEVICE;
		goto err_out;
	}

err_out:
	if (strbuf != NULL) {
		free(strbuf);
		strbuf = NULL;
	}

	return CL_SUCCESS;
}

static cl_int gdb_remote_write_mem(cl_device_id dev, uint64_t paddr, const char *buf, uint64_t len)
{
	uint64_t p = 0;
	cl_int r;

	while (p < len) {
		uint64_t l = NMIN(len, 0x7f0);

		r = gdb_remote_write_mem_one(dev, paddr + p, &buf[p], l);
		if (r != CL_SUCCESS) {
			return r;
		}

		p += l;
	}

	return CL_SUCCESS;
}

static const struct dev_ops gdb_remote_ops = {
	.probe = gdb_remote_probe,
	.remove = gdb_remote_remove,
	.reset = gdb_remote_reset,
	.read_mem = gdb_remote_read_mem,
	.write_mem = gdb_remote_write_mem,
};

static cl_int gdb_remote_alloc_dev(cl_platform_id platform, cl_device_id *pdev)
{
	cl_int r;

	r = dev_alloc(platform, pdev);
	if (r != CL_SUCCESS) {
		return r;
	}

	cl_device_id dev = *pdev;

	dev->ops = &gdb_remote_ops;
	dev->dev_type = CL_DEVICE_TYPE_ACCELERATOR;

	struct gdb_remote_priv *prv = calloc(1, sizeof(struct gdb_remote_priv));
	if (prv == NULL) {
		log_err("failed to alloc priv data.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	dev->priv = prv;

	return CL_SUCCESS;
}

static cl_int gdb_remote_free_dev(cl_device_id dev)
{
	cl_int r, res = CL_SUCCESS;

	if (dev == NULL) {
		return CL_SUCCESS;
	}

	if (dev->priv != NULL) {
		free(dev->priv);
	}

	r = dev_free(dev);
	if (r != CL_SUCCESS) {
		// Continue anyway
		res = r;
	}

	return res;
}

cl_int gdb_remote_init(cl_platform_id platform)
{
	cl_uint num = 0;
	cl_int r;

	/* TODO: how to enumerate QEMU GDB remote I/F??? */
	num = 1;

	in_devs = calloc(num, sizeof(struct _cl_device_id));
	if (in_devs == NULL) {
		log_err("failed to alloc device info.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	for (cl_uint i = 0; i < num; i++) {
		cl_device_id dev = NULL;

		r = gdb_remote_alloc_dev(platform, &dev);
		if (r != CL_SUCCESS) {
			goto err_out;
		}

		struct gdb_remote_priv *prv = dev->priv;

		/* TODO: how to enumerate QEMU GDB remote I/F??? */
		prv->node = "localhost";
		prv->service = "1234";

		r = dev_add(dev);
		if (r != CL_SUCCESS) {
			goto err_out;
		}

		in_devs[i] = dev;
	}

	in_devs_num = num;

	return CL_SUCCESS;

err_out:
	gdb_remote_exit(platform);

	return r;
}

cl_int gdb_remote_exit(cl_platform_id platform)
{
	cl_int r;

	if (in_devs == NULL) {
		return CL_SUCCESS;
	}

	for (cl_uint i = 0; i < in_devs_num; i++) {
		cl_device_id dev = in_devs[i];

		r = dev_remove(dev);
		if (r != CL_SUCCESS) {
			// Ignore error
		}

		r = gdb_remote_free_dev(dev);
		if (r != CL_SUCCESS) {
			// Ignore error
		}
	}

	free(in_devs);
	in_devs = NULL;
	in_devs_num = 0;

	return CL_SUCCESS;
}

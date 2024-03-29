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
#include "gdb_remote_internal.h"

#define BUFLEN    4096

static int gdb_remote_getchar(struct gdb_remote_priv *prv)
{
	unsigned char ch;
	ssize_t nrecv;

retry:
	nrecv = recv(prv->fd_sock, &ch, sizeof(ch), 0);
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
	ssize_t nsent;

retry:
	nsent = send(prv->fd_sock, &ch, sizeof(ch), 0);
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

cl_int gdb_remote_send_interrupt(struct gdb_remote_priv *prv)
{
	ssize_t nsent, len = 1;

	log_dbg("->\n");

	/* Send ETX (end of text) */
	nsent = send(prv->fd_sock, "\x03", len, 0);
	if (nsent != len) {
		log_err("failed to send interrupt.\n");
		return CL_OUT_OF_RESOURCES;
	}

	return CL_SUCCESS;
}

cl_int gdb_remote_send(struct gdb_remote_priv *prv, const char *cmd, size_t cmdlen, int ack)
{
	size_t buflen = cmdlen * 2 + 10;
	size_t pos = 0, n;
	char *buf = alloca(buflen);
	char sum = 0;
	char tmp[10];
	ssize_t nsent;

	memset(buf, 0, buflen);

	strcat(buf, "$");
	pos += 1;

	for (size_t i = 0; i < cmdlen; i++) {
		switch (cmd[i]) {
		case '#':
		case '$':
		case '}':
		case '*':
			buf[pos] = '}';
			buf[pos + 1] = cmd[i] ^ 0x20;
			pos += 2;
			sum += '}';
			sum += cmd[i] ^ 0x20;
			break;
		default:
			buf[pos] = cmd[i];
			pos += 1;
			sum += cmd[i];
			break;
		}
		if (i >= buflen) {
			log_err("buflen is too short.\n");
			return CL_OUT_OF_HOST_MEMORY;
		}
	}

	int len = snprintf(tmp, sizeof(tmp) - 1, "#%02x", sum & 0xff);
	if (len < 0) {
		log_err("failed to snprintf.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	n = strlen(tmp);
	memcpy(buf + pos, tmp, n);
	pos += n;

	log_dbg("-> %s\n", buf);

resend:
	nsent = send(prv->fd_sock, buf, pos, 0);
	if (nsent != pos) {
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

cl_int gdb_remote_recv(struct gdb_remote_priv *prv, char *cmd, size_t cmdlen, int ack)
{
	const char *str_sum_recv;
	int c, sum = 0, sum_recv;
	size_t p = 0, q = 0;
	char buf[BUFLEN];
	cl_int r = CL_SUCCESS;

	memset(buf, 0, BUFLEN);

	c = gdb_remote_getchar(prv);
	buf[p++] = c;

	while (c == '+') {
		c = gdb_remote_getchar(prv);
		buf[p++] = c;
	}
	if (c == '$') {
		c = gdb_remote_getchar(prv);
		buf[p++] = c;
	}

	while (c != '#') {
		sum += c;

		if (q < cmdlen - 1) {
			cmd[q++] = c;
		} else {
			r = CL_OUT_OF_HOST_MEMORY;
		}
		c = gdb_remote_getchar(prv);
		buf[p++] = c;

		if (p >= BUFLEN) {
			log_err("Too long answer (len %d).\n", BUFLEN);
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

	return r;
}

cl_int gdb_remote_discard_all(struct gdb_remote_priv *prv)
{
	const char *cmd = "vMustReplyEmpty";

	/* Discard old replies */
	char tmpbuf[4096];

	gdb_remote_send(prv, cmd, strlen(cmd), 0);
	while (1) {
		gdb_remote_recv(prv, tmpbuf, sizeof(tmpbuf), 1);
		if (strcmp(tmpbuf, "") == 0) {
			break;
		}
	}

	return CL_SUCCESS;
}

cl_int gdb_remote_enum(const struct gdb_remote_conf *conf, struct gdb_remote_enum_info **inf, cl_uint *num)
{
	struct gdb_remote_enum_info *reminf = NULL;
	cl_uint num_rem = 1;
	cl_int r = CL_SUCCESS;

	if (conf == NULL || inf == NULL || num == NULL) {
		return CL_INVALID_VALUE;
	}

	reminf = calloc(num_rem, sizeof(struct gdb_remote_enum_info));
	if (reminf == NULL) {
		log_err("failed to alloc gdb_remote info array.\n");
		r = CL_OUT_OF_HOST_MEMORY;
		goto err_out;
	}

	/* TODO: how to enumerate GDB remote I/F??? */
	reminf[0].node = conf->node;
	reminf[0].service = conf->service;

	*inf = reminf;
	*num = num_rem;

	return CL_SUCCESS;

err_out:
	return r;
}

cl_int gdb_remote_probe(cl_device_id dev)
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
	r = getaddrinfo(prv->info.node, prv->info.service, &hints, &ai);
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
		log_err("failed to connect to '%s:%s'.\n",
			prv->info.node, prv->info.service);
		return CL_OUT_OF_HOST_MEMORY;
	}

	freeaddrinfo(ai);

	gdb_remote_discard_all(prv);

	return CL_SUCCESS;
}

cl_int gdb_remote_remove(cl_device_id dev)
{
	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	close(prv->fd_sock);

	return CL_SUCCESS;
}

cl_int gdb_remote_run(cl_device_id dev)
{
	const char *cmd = "vCont;c";
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	// continue
	r = gdb_remote_send(prv, cmd, strlen(cmd), 0);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

cl_int gdb_remote_stop(cl_device_id dev)
{
	char tmp[4096];
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	gdb_remote_send_interrupt(prv);

	while (1) {
		memset(tmp, 0, sizeof(tmp));
		r = gdb_remote_recv(prv, tmp, sizeof(tmp) - 1, 1);
		if (r != CL_SUCCESS) {
			return r;
		}
		if (tmp[0] == 'O') {
			continue;
		} else if (tmp[0] == 'T' || tmp[0] == 'W') {
			break;
		}
	}

	gdb_remote_discard_all(prv);

	return CL_SUCCESS;
}

static cl_int gdb_remote_read_mem_one(struct gdb_remote_priv *prv, uint64_t paddr, char *buf, uint64_t len)
{
	/* $m00000000,000#00 */
	char cmd[20];
	char *strbuf = NULL;
	size_t buflen = len * 2 + 64;
	cl_int r = CL_SUCCESS;

	if (len > 0x7f0) {
		log_err("Too large to read len:%" PRId64 ".\n", len);
		return CL_INVALID_VALUE;
	}

	snprintf(cmd, sizeof(cmd), "m%08" PRIx64 ",%" PRIx64, paddr, len);

	r = gdb_remote_send(prv, cmd, strlen(cmd), 1);
	if (r != CL_SUCCESS) {
		return r;
	}

	strbuf = calloc(buflen, sizeof(char));
	if (strbuf == NULL) {
		log_err("Failed to calloc recv buf.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	r = gdb_remote_recv(prv, strbuf, buflen - 1, 1);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	for (uint64_t i = 0; i < len; i++) {
		int b, n;

		if (buflen < 2) {
			log_err("Failed to convert bytes, too short @%" PRId64 ".\n",
				i * 2);
			r = CL_OUT_OF_RESOURCES;
			goto err_out;
		}

		n = sscanf(&strbuf[i * 2], "%02x", &b);
		if (n != 1) {
			log_err("Failed to convert bytes, wrong answer @%" PRId64".\n",
				i * 2);
			r = CL_OUT_OF_RESOURCES;
			goto err_out;
		}

		buf[i] = b;
		buflen -= 2;
	}

	free(strbuf);
	strbuf = NULL;

err_out:
	if (strbuf != NULL) {
		free(strbuf);
		strbuf = NULL;
	}

	return CL_SUCCESS;
}

cl_int gdb_remote_read_mem(cl_device_id dev, uint64_t paddr, char *buf, uint64_t len)
{
	uint64_t p = 0;
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	while (p < len) {
		uint64_t l = NMIN(len - p, 0x7f0);

		r = gdb_remote_read_mem_one(prv, paddr + p, &buf[p], l);
		if (r != CL_SUCCESS) {
			return r;
		}

		p += l;
	}

	return CL_SUCCESS;
}

static cl_int gdb_remote_write_mem_one(struct gdb_remote_priv *prv, uint64_t paddr, const char *buf, uint64_t len)
{
	char *strbuf = NULL;
	char tmp[4];
	size_t buflen = len * 2 + 64;
	cl_int r = CL_SUCCESS;
	int n;

	if (len > 0x7f0) {
		log_err("Too large to write len:%" PRId64 ".\n", len);
		return CL_INVALID_VALUE;
	}

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

	r = gdb_remote_send(prv, strbuf, strlen(strbuf), 1);
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

cl_int gdb_remote_write_mem(cl_device_id dev, uint64_t paddr, const char *buf, uint64_t len)
{
	uint64_t p = 0;
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	while (p < len) {
		uint64_t l = NMIN(len - p, 0x7f0);

		r = gdb_remote_write_mem_one(prv, paddr + p, &buf[p], l);
		if (r != CL_SUCCESS) {
			return r;
		}

		p += l;
	}

	return CL_SUCCESS;
}

static cl_int gdb_remote_write_bin_one(struct gdb_remote_priv *prv, uint64_t paddr, const char *buf, uint64_t len)
{
	char *strbuf = NULL;
	char tmp[4];
	size_t buflen = len + 64;
	size_t pos = 0;
	cl_int r = CL_SUCCESS;
	int n;

	if (len > 0x7f0) {
		log_err("Too large to write len:%" PRId64 ".\n", len);
		return CL_INVALID_VALUE;
	}

	strbuf = calloc(buflen, sizeof(char));
	if (strbuf == NULL) {
		log_err("Failed to calloc string buf.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	n = snprintf(strbuf, buflen - pos, "X%08" PRIx64 ",%" PRIx64 ":", paddr, len);
	pos += n;

	memcpy(strbuf + pos, buf, len);
	pos += len;

	r = gdb_remote_send(prv, strbuf, pos, 1);
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


cl_int gdb_remote_write_bin(cl_device_id dev, uint64_t paddr, const char *buf, uint64_t len)
{
	uint64_t p = 0;
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	while (p < len) {
		uint64_t l = NMIN(len - p, 0x7f0);

		r = gdb_remote_write_bin_one(prv, paddr + p, &buf[p], l);
		if (r != CL_SUCCESS) {
			return r;
		}

		p += l;
	}

	return CL_SUCCESS;
}

cl_int gdb_remote_alloc_dev(cl_platform_id platform, cl_device_id *pdev)
{
	cl_int r;

	r = dev_alloc(platform, pdev);
	if (r != CL_SUCCESS) {
		return r;
	}

	cl_device_id dev = *pdev;

	dev->dev_type = CL_DEVICE_TYPE_ACCELERATOR;

	struct gdb_remote_priv *prv = calloc(1, sizeof(struct gdb_remote_priv));
	if (prv == NULL) {
		log_err("failed to alloc priv data.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	dev->priv = prv;

	return CL_SUCCESS;
}

cl_int gdb_remote_free_dev(cl_device_id dev)
{
	cl_int r, res = CL_SUCCESS;

	if (dev == NULL) {
		return CL_SUCCESS;
	}

	if (dev->priv != NULL) {
		free(dev->priv);
		dev->priv = NULL;
	}

	r = dev_free(dev);
	if (r != CL_SUCCESS) {
		// Continue anyway
		res = r;
	}

	return res;
}

cl_int gdb_remote_init(cl_platform_id platform, const struct gdb_remote_conf *conf, cl_device_id **devs, cl_uint *devs_num)
{
	struct gdb_remote_enum_info *inf = NULL;
	cl_int r;

	if (conf == NULL || devs == NULL || devs_num == NULL) {
		return CL_INVALID_VALUE;
	}

	*devs = NULL;
	*devs_num = 0;

	r = gdb_remote_enum(conf, &inf, devs_num);
	if (r != CL_SUCCESS) {
		return r;
	}

	*devs = calloc(*devs_num, sizeof(struct _cl_device_id));
	if (*devs == NULL) {
		log_err("failed to alloc device info.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	for (cl_uint i = 0; i < *devs_num; i++) {
		cl_device_id dev = NULL;

		r = gdb_remote_alloc_dev(platform, &dev);
		if (r != CL_SUCCESS) {
			goto err_out;
		}
		dev->ops = conf->ops;

		struct gdb_remote_priv *prv = dev->priv;

		prv->conf = conf;
		prv->info = inf[i];

		r = dev_add(dev);
		if (r != CL_SUCCESS) {
			goto err_out;
		}

		(*devs)[i] = dev;
	}

	return CL_SUCCESS;

err_out:
	*devs = NULL;
	*devs_num = 0;

	if (inf != NULL) {
		free(inf);
	}

	gdb_remote_exit(platform, devs, devs_num);

	return r;
}

cl_int gdb_remote_exit(cl_platform_id platform, cl_device_id **devs, cl_uint *devs_num)
{
	cl_int r;

	if (devs == NULL || devs_num == NULL) {
		return CL_INVALID_VALUE;
	}

	for (cl_uint i = 0; i < *devs_num; i++) {
		cl_device_id dev = (*devs)[i];

		r = dev_remove(dev);
		if (r != CL_SUCCESS) {
			// Ignore error
		}

		r = gdb_remote_free_dev(dev);
		if (r != CL_SUCCESS) {
			// Ignore error
		}
	}

	free(*devs);
	*devs = NULL;
	*devs_num = 0;

	return CL_SUCCESS;
}

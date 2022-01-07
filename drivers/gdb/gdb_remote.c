/* SPDX-License-Identifier: Apache-2.0 */

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

static cl_int gdb_remote_send(cl_device_id dev, const char *cmd)
{
	struct gdb_remote_priv *prv = dev->priv;
	size_t len = strlen(cmd);
	char *buf = NULL;
	cl_int r;

	size_t maxlen = len + 10;
	buf = calloc(maxlen, sizeof(char));
	if (buf == NULL) {
		log_err("failed to calloc buffer.\n");
		r = CL_OUT_OF_HOST_MEMORY;
		goto err_out;
	}

	char sum = 0;
	for (size_t i = 0; i < len; i++) {
		sum += cmd[i];
	}

	int buflen = snprintf(buf, maxlen, "+$%s#%02x", cmd, sum & 0xff);
	if (buflen < 0) {
		log_err("failed to snprintf.\n");
		r = CL_OUT_OF_HOST_MEMORY;
		goto err_out;
	}

	log_info("-> %s\n", buf);

	ssize_t nsent = send(prv->fd_sock, buf, buflen, 0);
	if (nsent != buflen) {
		log_err("failed to send data.\n");
		r = CL_OUT_OF_RESOURCES;
		goto err_out;
	}

	return CL_SUCCESS;

err_out:
	if (buf != NULL) {
		free(buf);
		buf = NULL;
	}

	return r;
}

static cl_int gdb_remote_recv(cl_device_id dev, char *cmd, size_t cmdlen)
{
	struct gdb_remote_priv *prv = dev->priv;
	char *buf = NULL;
	cl_int r;

	size_t buflen = cmdlen + 10;
	buf = calloc(buflen, sizeof(char));
	if (buf == NULL) {
		log_err("failed to calloc buffer.\n");
		r = CL_OUT_OF_HOST_MEMORY;
		goto err_out;
	}

	ssize_t nrecv = recv(prv->fd_sock, buf, buflen, 0);
	//if (nrecv != buflen) {
	//	log_err("failed to recv data.\n");
	//	return CL_OUT_OF_RESOURCES;
	//}

	log_info("<- %s\n", buf);

	return CL_SUCCESS;

err_out:
	if (buf != NULL) {
		free(buf);
		buf = NULL;
	}

	return r;
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

	char hoge[256] = {};

	gdb_remote_send(dev, "?");
	gdb_remote_recv(dev, hoge, 256);
	gdb_remote_send(dev, "vMustReplyEmpty");
	gdb_remote_recv(dev, hoge, 256);
	gdb_remote_send(dev, "Hgp0.0");
	gdb_remote_recv(dev, hoge, 256);


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

static const struct dev_ops gdb_remote_ops = {
	.probe = gdb_remote_probe,
	.remove = gdb_remote_remove,
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

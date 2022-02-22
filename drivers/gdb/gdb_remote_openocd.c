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

static cl_device_id *in_devs;
static cl_uint in_devs_num;

static cl_int gdb_remote_openocd_reset(cl_device_id dev)
{
	char tmp[4096];
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	// monitor reset halt
	r = gdb_remote_send(prv, "qRcmd,72657365742068616c74", 1);
	if (r != CL_SUCCESS) {
		return r;
	}

	while (1) {
		memset(tmp, 0, sizeof(tmp));
		r = gdb_remote_recv(prv, tmp, sizeof(tmp) - 1, 1);
		if (r != CL_SUCCESS) {
			return r;
		}
		if (strcmp(tmp, "OK") == 0) {
			break;
		}
	}

	return CL_SUCCESS;
}

static const struct dev_ops gdb_remote_openocd_ops = {
	.probe = gdb_remote_probe,
	.remove = gdb_remote_remove,
	.reset = gdb_remote_openocd_reset,
	.run = gdb_remote_run,
	.stop = gdb_remote_stop,
	.read_mem = gdb_remote_read_mem,
	.write_mem = gdb_remote_write_mem,
};

cl_int gdb_remote_openocd_init(cl_platform_id platform)
{
	cl_uint num = 0;
	cl_int r;

	/* TODO: how to enumerate OpenOCD GDB remote I/F??? */
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
		dev->ops = &gdb_remote_openocd_ops;

		struct gdb_remote_priv *prv = dev->priv;

		/* TODO: how to enumerate OpenOCD GDB remote I/F??? */
		prv->node = "localhost";
		prv->service = "3333";

		r = dev_add(dev);
		if (r != CL_SUCCESS) {
			goto err_out;
		}

		in_devs[i] = dev;
	}

	in_devs_num = num;

	return CL_SUCCESS;

err_out:
	gdb_remote_openocd_exit(platform);

	return r;
}

cl_int gdb_remote_openocd_exit(cl_platform_id platform)
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

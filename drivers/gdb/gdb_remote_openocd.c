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
	const char *cmd = "qRcmd,72657365742068616c74";
	char tmp[4096];
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct gdb_remote_priv *prv = dev->priv;

	// monitor reset halt
	r = gdb_remote_send(prv, cmd, strlen(cmd), 1);
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
	.write_mem = gdb_remote_write_bin
};

static const struct gdb_remote_conf gdb_remote_openocd_conf = {
	.node = "localhost",
	.service = "3333",
	.ops = &gdb_remote_openocd_ops,
};

cl_int gdb_remote_openocd_init(cl_platform_id platform)
{
	return gdb_remote_init(platform, &gdb_remote_openocd_conf, &in_devs, &in_devs_num);
}

cl_int gdb_remote_openocd_exit(cl_platform_id platform)
{
	return gdb_remote_exit(platform, &in_devs, &in_devs_num);
}

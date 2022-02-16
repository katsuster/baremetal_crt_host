/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_GDB_REMOTE
#define BAREMETAL_CRT_HOST_DRV_GDB_REMOTE

#include <type_cl.h>

struct gdb_remote_priv {
	/* Settings */
	const char *node;
	const char *service;

	/* For runtime */
	int fd_sock;
};

#ifdef CONFIG_GDB_OPENOCD
cl_int gdb_remote_openocd_init(cl_platform_id platform);
cl_int gdb_remote_openocd_exit(cl_platform_id platform);
#endif /* CONFIG_GDB_OPENOCD */

#ifdef CONFIG_GDB_QEMU
cl_int gdb_remote_qemu_init(cl_platform_id platform);
cl_int gdb_remote_qemu_exit(cl_platform_id platform);
#endif /* CONFIG_GDB_QEMU */

#endif /* BAREMETAL_CRT_HOST_DRV_GDB_REMOTE */

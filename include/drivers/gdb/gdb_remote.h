/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_GDB_REMOTE
#define BAREMETAL_CRT_HOST_DRV_GDB_REMOTE

#include <type_cl.h>

cl_int gdb_remote_init(cl_platform_id platform);
cl_int gdb_remote_exit(cl_platform_id platform);

#endif /* BAREMETAL_CRT_HOST_DRV_GDB_REMOTE */

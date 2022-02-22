/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_GDB_REMOTE_INTERNAL
#define BAREMETAL_CRT_HOST_DRV_GDB_REMOTE_INTERNAL

#include <type_cl.h>

#include <drivers/gdb/gdb_remote.h>

cl_int gdb_remote_send_interrupt(struct gdb_remote_priv *prv);
cl_int gdb_remote_send(struct gdb_remote_priv *prv, const char *cmd, int ack);
cl_int gdb_remote_recv(struct gdb_remote_priv *prv, char *cmd, size_t cmdlen, int ack);
cl_int gdb_remote_discard_all(struct gdb_remote_priv *prv);
cl_int gdb_remote_probe(cl_device_id dev);
cl_int gdb_remote_remove(cl_device_id dev);
cl_int gdb_remote_run(cl_device_id dev);
cl_int gdb_remote_stop(cl_device_id dev);
cl_int gdb_remote_read_mem(cl_device_id dev, uint64_t paddr, char *buf, uint64_t len);
cl_int gdb_remote_write_mem(cl_device_id dev, uint64_t paddr, const char *buf, uint64_t len);
cl_int gdb_remote_alloc_dev(cl_platform_id platform, cl_device_id *pdev);
cl_int gdb_remote_free_dev(cl_device_id dev);

#endif /* BAREMETAL_CRT_HOST_DRV_GDB_REMOTE_INTERNAL */
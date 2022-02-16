# SPDX-License-Identifier: Apache-2.0

prj_sources(
  gdb_remote.c
  )
prj_sources_ifdef(CONFIG_GDB_OPENOCD gdb_remote_openocd.c)
prj_sources_ifdef(CONFIG_GDB_QEMU gdb_remote_qemu.c)

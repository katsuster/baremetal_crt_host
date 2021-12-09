# SPDX-License-Identifier: Apache-2.0

prj_sources(
  platform.c
  device.c
  )

add_subdirectory_ifdef(CONFIG_GDB gdb)

# SPDX-License-Identifier: Apache-2.0

prj_sources(
  platform.c
  device.c
  context.c
  command_queue.c
  )

add_subdirectory_ifdef(CONFIG_GDB gdb)

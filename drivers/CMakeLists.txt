# SPDX-License-Identifier: Apache-2.0

prj_sources(
  platform.c
  device.c
  context.c
  command_queue.c
  program.c
  program_elf.c
  kernel.c
  mem.c
  )

add_subdirectory_ifdef(CONFIG_GDB gdb)
add_subdirectory_ifdef(CONFIG_PCI pci)

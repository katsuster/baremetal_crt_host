# SPDX-License-Identifier: Apache-2.0

prj_sources(
  pci_sysfs.c
  )
prj_sources_ifdef(CONFIG_PCI_SYSFS_TEST pci_sysfs_test.c)

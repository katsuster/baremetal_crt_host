/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_PCI_SYSFS
#define BAREMETAL_CRT_HOST_DRV_PCI_SYSFS

#include <type_cl.h>

#ifdef CONFIG_PCI_SYSFS_TEST
cl_int pci_sysfs_test_init(cl_platform_id platform);
cl_int pci_sysfs_test_exit(cl_platform_id platform);
#endif

#endif /* BAREMETAL_CRT_HOST_DRV_PCI_SYSFS */

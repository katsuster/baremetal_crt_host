/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <drivers/pci/pci_sysfs.h>
#include "pci_sysfs_internal.h"

static cl_device_id *in_devs;
static cl_uint in_devs_num;

static const struct pci_sysfs_dev_id pci_sysfs_test_id_table[] = {
	/* Sentinel */
	{0xffff, 0xffff},
};

static const struct pci_sysfs_memmap pci_sysfs_test_map_table[] = {
	/* Test AREA */
	{0x10000000, 256 * MB, 0, 0x00000000},
	/* Sentinel */
	{-1, -1, -1, -1},
};

static const struct dev_ops pci_sysfs_test_ops = {
	.probe = pci_sysfs_probe,
	.remove = pci_sysfs_remove,
	//.reset = pci_sysfs_reset,
	//.run = pci_sysfs_run,
	//.stop = pci_sysfs_stop,
	.read_mem = pci_sysfs_read_mem,
	.write_mem = pci_sysfs_write_mem,
};

static const struct pci_sysfs_conf pci_sysfs_test_conf = {
	.id_table = pci_sysfs_test_id_table,
	.map_table = pci_sysfs_test_map_table,
	.ops = &pci_sysfs_test_ops,
};

cl_int pci_sysfs_test_init(cl_platform_id platform)
{
	return pci_sysfs_init(platform, &pci_sysfs_test_conf, &in_devs, &in_devs_num);
}

cl_int pci_sysfs_test_exit(cl_platform_id platform)
{
	return pci_sysfs_exit(platform, &in_devs, &in_devs_num);
}

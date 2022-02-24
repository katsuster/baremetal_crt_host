/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_PCI_SYSFS_INTERNAL
#define BAREMETAL_CRT_HOST_DRV_PCI_SYSFS_INTERNAL

#define PATH_PCI_DEVICES    "/proc/bus/pci/devices"
#define PATH_PCI_SYSFS      "/sys/bus/pci/devices"

#define PCI_RES_MAX    6

struct pci_sysfs_dev_id {
	uint16_t vendor;
	uint16_t device;
};

struct pci_sysfs_memmap {
	/* Device internal physical address */
	uint64_t paddr;
	/* Area size */
	uint64_t size;
	/* BAR number (0 ... 5) */
	uint32_t bar;
	/* Offset of BAR space */
	uint64_t bar_off;
};

struct pci_sysfs_addr {
	uint64_t paddr;
	uint64_t size;
	uint32_t bar;
	uint64_t offset;

	/* mapped address */
	char *ptr;
};

struct pci_sysfs_conf {
	const struct pci_sysfs_dev_id *id_table;
	const struct pci_sysfs_memmap *map_table;
	const struct dev_ops *ops;
};

struct pci_sysfs_enum_info {
	int pcibus;
	int pcidev;
	int pcifn;
	uint16_t vendor;
	uint16_t device;
};

struct pci_sysfs_resource {
	uint64_t bar;
	uint64_t bar_size;
	int is_64;
	int valid;
	int fd;

	/* mapped address */
	char *buf;
};

struct pci_sysfs_priv {
	const struct pci_sysfs_conf *conf;
	struct pci_sysfs_enum_info info;

	char sysfs_path[2048];
	int fd_config;
	struct pci_sysfs_resource pci_res[PCI_RES_MAX];
};

cl_int pci_sysfs_get_addr(struct pci_sysfs_priv *prv, uint64_t paddr, uint64_t len, struct pci_sysfs_addr *desc);
uint32_t pci_sysfs_read32(struct pci_sysfs_addr *desc, uint64_t off);
void pci_sysfs_write32(struct pci_sysfs_addr *desc, uint32_t v, uint64_t off);

cl_int pci_sysfs_enum(const struct pci_sysfs_conf *conf, struct pci_sysfs_enum_info **inf, cl_uint *num);
cl_int pci_sysfs_probe(cl_device_id dev);
cl_int pci_sysfs_remove(cl_device_id dev);
cl_int pci_sysfs_read_mem(cl_device_id dev, uint64_t paddr, char *buf, uint64_t len);
cl_int pci_sysfs_write_mem(cl_device_id dev, uint64_t paddr, const char *buf, uint64_t len);
cl_int pci_sysfs_alloc_dev(cl_platform_id platform, cl_device_id *pdev);
cl_int pci_sysfs_free_dev(cl_device_id dev);
cl_int pci_sysfs_init(cl_platform_id platform, const struct pci_sysfs_conf *conf, cl_device_id **devs, cl_uint *devs_num);
cl_int pci_sysfs_exit(cl_platform_id platform, cl_device_id **devs, cl_uint *devs_num);

#endif /* BAREMETAL_CRT_HOST_DRV_PCI_SYSFS_INTERNAL */

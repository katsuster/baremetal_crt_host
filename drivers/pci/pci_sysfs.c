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

/**
 * Exact byte copy function without unaligned access for safe copy data
 * from/to device memory.
 */
static void *pci_sysfs_memcpy_slow(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	for (; n; n--, d++, s++) *d = *s;

	return dest;
}

/**
 * copy function without unaligned access for safe copy data from/to
 * device memory.
 */
static void *pci_sysfs_memcpy(void *dest, const void *src, size_t n)
{
	if (((uintptr_t)dest & 0x3) != 0 || ((uintptr_t)src & 0x3) != 0) {
		/* not safe */
		return pci_sysfs_memcpy_slow(dest, src, n);
	}

	uint32_t *d = dest;
	const uint32_t *s = src;

	for (; n >= 4; n -= 4, d++, s++) *d = *s;
	if (n > 0) {
		pci_sysfs_memcpy_slow(d, s, n);
	}

	return dest;
}

cl_int pci_sysfs_get_addr(struct pci_sysfs_priv *prv, uint64_t paddr, uint64_t len, struct pci_sysfs_addr *desc)
{
	const struct pci_sysfs_memmap *m = prv->conf->map_table;

	for (; m->paddr != -1; m++) {
		if (paddr < m->paddr || m->paddr + m->size < paddr + len) {
			continue;
		}

		if (desc) {
			desc->paddr = paddr;
			desc->size = len;
			desc->bar = m->bar;
			desc->offset = m->bar_off + paddr - m->paddr;
			desc->ptr = prv->pci_res[desc->bar].buf + desc->offset;

			log_dbg("    paddr: 0x%" PRIx64 "-0x%" PRIx64 " ->"
				" BAR%d 0x%" PRIx64 "-0x%" PRIx64 ", "
				"ptr: %p\n",
				paddr, paddr + len, desc->bar,
				desc->offset, desc->offset + desc->size,
				desc->ptr);
		}

		return CL_SUCCESS;
	}

	log_err("Invalid paddr: 0x%" PRIx64 "-0x%" PRIx64 "\n",
		paddr, paddr + len);

	return CL_INVALID_VALUE;
}

uint32_t pci_sysfs_read32(struct pci_sysfs_addr *desc, uint64_t off)
{
	char *p = desc->ptr + off;

	return *(const volatile uint32_t *)p;
}

void pci_sysfs_write32(struct pci_sysfs_addr *desc, uint32_t v, uint64_t off)
{
	char *p = desc->ptr + off;

	*(volatile uint32_t *)p = v;
}

cl_int pci_sysfs_enum(const struct pci_sysfs_conf *conf, struct pci_sysfs_enum_info **inf, cl_uint *num)
{
	const struct pci_sysfs_dev_id *id_table;
	struct pci_sysfs_enum_info *pciinf = NULL;
	const char *fname = PATH_PCI_DEVICES;
	int pcibus, devfn, pcidev, pcifn, vid, did;
	char line[4096];
	FILE *f = NULL;
	int n = 0, num_pci = 1, r = CL_SUCCESS;

	if (conf == NULL || inf == NULL || num == NULL) {
		return CL_INVALID_VALUE;
	}

	id_table = conf->id_table;

	//TODO: need to change fname by env?
	f = fopen(fname, "r");
	if (f == NULL) {
		log_err("failed to open %s.\n", fname);
		return CL_OUT_OF_HOST_MEMORY;
	}

	pciinf = calloc(num_pci, sizeof(struct pci_sysfs_enum_info));
	if (pciinf == NULL) {
		log_err("failed to alloc pci ID info array.\n");
		r = CL_OUT_OF_HOST_MEMORY;
		goto err_out;
	}

	while (fgets(line, sizeof(line), f) != NULL) {
		int found = 0;

		r = sscanf(line, "%02x%02x\t%04x%04x", &pcibus, &devfn, &vid, &did);
		if (r != 4) {
			log_err("failed to parse %s.\n", fname);
			r = CL_OUT_OF_HOST_MEMORY;
			goto err_out;
		}
		pcidev = (devfn >> 3) & 0x1f;
		pcifn = devfn & 0x07;

		for (size_t i = 0; id_table[i].vendor != 0xffff && id_table[i].device != 0xffff; i++) {
			if (id_table[i].vendor == vid && id_table[i].device == did) {
				found = 1;
				break;
			}
		}
		if (!found) {
			continue;
		}

		log_dbg("pci:%02x:%02x.%01x vid:%04x did:%04x matched\n",
			pcibus, pcidev, pcifn, vid, did);

		pciinf[n].pcibus = pcibus;
		pciinf[n].pcidev = pcidev;
		pciinf[n].pcifn = pcifn;
		pciinf[n].vendor = vid;
		pciinf[n].device = did;

		n++;
		if (n == num_pci) {
			num_pci *= 2;
			pciinf = realloc(pciinf, num_pci * sizeof(struct pci_sysfs_enum_info));
			if (pciinf == NULL) {
				log_err("failed to expand pci ID info array.\n");
				r = CL_OUT_OF_HOST_MEMORY;
				goto err_out;
			}
		}
	}

	*inf = pciinf;
	*num = n;

	return CL_SUCCESS;

err_out:
	if (pciinf) {
		free(pciinf);
	}

	fclose(f);

	return r;
}

static cl_int pci_sysfs_config(struct pci_sysfs_priv *prv)
{
	char tmp[4096];
	uint16_t vid, did;
	ssize_t s;
	cl_int r = CL_SUCCESS;

	// TODO: domain is always zero?
	snprintf(prv->sysfs_path, sizeof(prv->sysfs_path), "%s/%04x:%02x:%02x.%x",
		PATH_PCI_SYSFS, 0, prv->info.pcibus, prv->info.pcidev, prv->info.pcifn);

	snprintf(tmp, sizeof(tmp), "%s/config", prv->sysfs_path);
	prv->fd_config = open(tmp, O_RDWR);
	if (prv->fd_config == -1) {
		perror("open");
		log_err("failed to open pci config space %s.\n", tmp);
		r = CL_OUT_OF_RESOURCES;
		goto err_out;
	}

	s = pread(prv->fd_config, &vid, 2, 0);
	s = pread(prv->fd_config, &did, 2, 2);
	if (prv->info.vendor != vid || prv->info.device != did) {
		log_err("pci vid:did %02x:%02x is invalid (expected %02x:%02x).\n",
			vid, did, prv->info.vendor, prv->info.device);
		r = CL_INVALID_DEVICE;
		goto err_out;
	}

	for (int i = 0; i < PCI_RES_MAX; i++) {
		const uint32_t mask_low = ~0xf, tmp_low = ~0;
		const uint64_t mask = ~0xfULL, tmp = ~0ULL;
		const uint32_t addr = 0x10 + i * 4;
		uint32_t bar_low, bar_size_low;
		uint64_t bar, bar_size;

		s = pread(prv->fd_config, &bar_low, 4, addr);
		s = pwrite(prv->fd_config, &tmp_low, 4, addr);
		s = pread(prv->fd_config, &bar_size_low, 4, addr);
		bar_size_low = ~(bar_size_low & mask_low) + 1;
		s = pwrite(prv->fd_config, &bar_low, 4, addr);
		if (s != 4) {
			log_err("failed to write to BAR.\n");
			r = CL_INVALID_DEVICE;
			goto err_out;
		}

		switch ((bar_low >> 1) & 0x3) {
		case 0x0:
		case 0x1:
			prv->pci_res[i].is_64 = 0;

			bar = bar_low;
			bar_size = bar_size_low;

			break;
		case 0x2:
			prv->pci_res[i].is_64 = 1;
			prv->pci_res[i + 1].is_64 = 0;
			prv->pci_res[i + 1].bar = 0;
			prv->pci_res[i + 1].bar_size = 0;
			prv->pci_res[i + 1].valid = 0;

			s = pread(prv->fd_config, &bar, 8, addr);
			s = pwrite(prv->fd_config, &tmp, 8, addr);
			s = pread(prv->fd_config, &bar_size, 8, addr);
			bar_size = ~(bar_size & mask) + 1;
			s = pwrite(prv->fd_config, &bar, 8, addr);

			break;
		default:
			log_err("unknown BAR type.\n");
			r = CL_INVALID_DEVICE;
			goto err_out;
		}

		prv->pci_res[i].bar = bar;
		prv->pci_res[i].bar_size = bar_size;
		if (bar_size == 0) {
			prv->pci_res[i].valid = 0;
		} else {
			prv->pci_res[i].valid = 1;
		}
		if (prv->pci_res[i].is_64) {
			i++;
		}
	}

	for (int i = 0; i < PCI_RES_MAX; i++) {
		log_dbg("  pci bar %d: 0x%" PRIx64 ", size: 0x%" PRIx64 ", "
			"64bit: %s, valid: %s\n", i,
			prv->pci_res[i].bar, prv->pci_res[i].bar_size,
			prv->pci_res[i].is_64 ? "y" : "n",
			prv->pci_res[i].valid ? "y" : "n");
		if (!prv->pci_res[i].valid) {
			continue;
		}

		snprintf(tmp, sizeof(tmp), "%s/resource%d", prv->sysfs_path, i);
		prv->pci_res[i].fd = open(tmp, O_RDWR | O_SYNC);
		if (prv->pci_res[i].fd == -1) {
			perror("open");
			log_err("failed to open pci resource %s.\n", tmp);
			r = CL_INVALID_DEVICE;
			goto err_out;
		}

		prv->pci_res[i].buf = (char *)mmap(NULL, prv->pci_res[i].bar_size,
			PROT_READ | PROT_WRITE, MAP_SHARED, prv->pci_res[i].fd, 0);
		if (prv->pci_res[i].buf == MAP_FAILED) {
			perror("mmap");
			log_err("failed to mmap file %s.\n", tmp);
			prv->pci_res[i].buf = NULL;
			r = CL_OUT_OF_RESOURCES;
			goto err_out;
		}
		log_dbg("  pci buf: %p\n", prv->pci_res[i].buf);
	}

	return CL_SUCCESS;

err_out:
	return r;
}

cl_int pci_sysfs_probe(cl_device_id dev)
{
	cl_int r = CL_SUCCESS;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct pci_sysfs_priv *prv = dev->priv;

	r = pci_sysfs_config(prv);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	return CL_SUCCESS;

err_out:
	pci_sysfs_remove(dev);

	return r;
}

cl_int pci_sysfs_remove(cl_device_id dev)
{
	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct pci_sysfs_priv *prv = dev->priv;

	for (int i = 0; i < PCI_RES_MAX; i++) {
		if (prv->pci_res[i].buf != NULL) {
			munmap(prv->pci_res[i].buf, prv->pci_res[i].bar_size);
			prv->pci_res[i].buf = NULL;
		}
		if (prv->pci_res[i].fd != -1) {
			close(prv->pci_res[i].fd);
			prv->pci_res[i].fd = -1;
		}
	}

	if (prv->fd_config != -1) {
		close(prv->fd_config);
		prv->fd_config = -1;
	}

	return CL_SUCCESS;
}

cl_int pci_sysfs_read_mem(cl_device_id dev, uint64_t paddr, char *buf, uint64_t len)
{
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct pci_sysfs_priv *prv = dev->priv;
	struct pci_sysfs_addr addr;

	r = pci_sysfs_get_addr(prv, paddr, len, &addr);
	if (r != CL_SUCCESS) {
		return r;
	}

	pci_sysfs_memcpy(buf, addr.ptr, len);

	return CL_SUCCESS;
}

cl_int pci_sysfs_write_mem(cl_device_id dev, uint64_t paddr, const char *buf, uint64_t len)
{
	cl_int r;

	if (dev == NULL || dev->priv == NULL) {
		return CL_INVALID_DEVICE;
	}

	struct pci_sysfs_priv *prv = dev->priv;
	struct pci_sysfs_addr addr;

	r = pci_sysfs_get_addr(prv, paddr, len, &addr);
	if (r != CL_SUCCESS) {
		return r;
	}

	pci_sysfs_memcpy(addr.ptr, buf, len);

	return CL_SUCCESS;
}

cl_int pci_sysfs_alloc_dev(cl_platform_id platform, cl_device_id *pdev)
{
	cl_int r;

	r = dev_alloc(platform, pdev);
	if (r != CL_SUCCESS) {
		return r;
	}

	cl_device_id dev = *pdev;

	dev->dev_type = CL_DEVICE_TYPE_ACCELERATOR;

	struct pci_sysfs_priv *prv = calloc(1, sizeof(struct pci_sysfs_priv));
	if (prv == NULL) {
		log_err("failed to alloc priv data.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	prv->fd_config = -1;

	for (int i = 0; i < PCI_RES_MAX; i++) {
		prv->pci_res[i].fd = -1;
	}

	dev->priv = prv;

	return CL_SUCCESS;
}

cl_int pci_sysfs_free_dev(cl_device_id dev)
{
	cl_int r, res = CL_SUCCESS;

	if (dev == NULL) {
		return CL_SUCCESS;
	}

	if (dev->priv != NULL) {
		free(dev->priv);
	}

	r = dev_free(dev);
	if (r != CL_SUCCESS) {
		// Continue anyway
		res = r;
	}

	return res;
}

cl_int pci_sysfs_init(cl_platform_id platform, const struct pci_sysfs_conf *conf, cl_device_id **devs, cl_uint *devs_num)
{
	struct pci_sysfs_enum_info *inf = NULL;
	cl_int r = CL_SUCCESS;

	if (conf == NULL || devs == NULL || devs_num == NULL) {
		return CL_INVALID_VALUE;
	}

	*devs = NULL;
	*devs_num = 0;

	r = pci_sysfs_enum(conf, &inf, devs_num);
	if (r != CL_SUCCESS) {
		return r;
	}

	*devs = calloc(*devs_num, sizeof(struct _cl_device_id));
	if (*devs == NULL) {
		log_err("failed to alloc device info.\n");
		r = CL_OUT_OF_HOST_MEMORY;
		goto err_out;
	}

	for (cl_uint i = 0; i < *devs_num; i++) {
		cl_device_id dev = NULL;

		r = pci_sysfs_alloc_dev(platform, &dev);
		if (r != CL_SUCCESS) {
			goto err_out;
		}
		dev->ops = conf->ops;

		struct pci_sysfs_priv *prv = dev->priv;

		prv->conf = conf;
		prv->info = inf[i];

		r = dev_add(dev);
		if (r != CL_SUCCESS) {
			goto err_out;
		}

		(*devs)[i] = dev;
	}

	free(inf);

	return CL_SUCCESS;

err_out:
	*devs = NULL;
	*devs_num = 0;

	if (inf != NULL) {
		free(inf);
	}

	pci_sysfs_exit(platform, devs, devs_num);

	return r;
}

cl_int pci_sysfs_exit(cl_platform_id platform, cl_device_id **devs, cl_uint *devs_num)
{
	cl_int r;

	if (devs == NULL || devs_num == NULL) {
		return CL_INVALID_VALUE;
	}

	for (cl_uint i = 0; i < *devs_num; i++) {
		cl_device_id dev = (*devs)[i];

		r = dev_remove(dev);
		if (r != CL_SUCCESS) {
			// Ignore error
		}

		r = pci_sysfs_free_dev(dev);
		if (r != CL_SUCCESS) {
			// Ignore error
		}
	}

	free(*devs);
	*devs = NULL;
	*devs_num = 0;

	return CL_SUCCESS;
}

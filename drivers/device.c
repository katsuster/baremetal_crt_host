/* SPDX-License-Identifier: Apache-2.0 */

#include <stdatomic.h>
#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/device.h>
#include <drivers/platform.h>

#ifdef CONFIG_GDB
#include <drivers/gdb/gdb_remote.h>
#endif

struct dev_group {
	cl_int (*init)(cl_platform_id platform);
	cl_int (*exit)(cl_platform_id platform);

	int inited;
};

static struct _cl_device_id in_head = {
	.id = -1,
};
static atomic_int in_dev_num;

static struct dev_group in_dev_groups[] = {
#ifdef CONFIG_GDB
	{
		.init = gdb_remote_init,
		.exit = gdb_remote_exit,
	},
#endif
};

cl_int dev_get_number(void)
{
	return in_dev_num;
}

cl_int dev_get_devices(cl_device_id *pdev, cl_device_type typ, cl_uint *sz)
{
	cl_device_id dev = &in_head;
	cl_uint cnt = 0;

	if (sz == NULL) {
		return CL_INVALID_VALUE;
	}

	while (dev->dev_next != NULL) {
		dev = dev->dev_next;
		if ((dev->dev_type & typ) == 0) {
			continue;
		}

		if (pdev != NULL && dev->enabled) {
			pdev[cnt] = dev;
			cnt++;
		}
		if (pdev != NULL && cnt >= *sz) {
			break;
		}
	}

	*sz = cnt;

	if (cnt == 0) {
		return CL_DEVICE_NOT_FOUND;
	} else {
		return CL_SUCCESS;
	}
}

cl_int dev_alloc(cl_platform_id plat, cl_device_id *dev)
{
	cl_device_id t;
	cl_int r;

	if ((r = plat_is_valid(plat)) != CL_SUCCESS) {
		return r;
	}
	if (dev == NULL) {
		return CL_INVALID_VALUE;
	}

	t = calloc(1, sizeof(struct _cl_device_id));
	if (t == NULL) {
		log_err("cannot calloc cl_device_id\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	t->dispatch = plat->dispatch;
	t->magic = OPENCL_ICD_MAGIC;
	t->id = plat_get_uniq_id();
	t->plat = plat;

	*dev = t;

	return CL_SUCCESS;
}

cl_int dev_free(cl_device_id dev)
{
	cl_int r;

	if ((r = dev_is_valid(dev)) != CL_SUCCESS) {
		return r;
	}

	free(dev);

	return CL_SUCCESS;
}

cl_int dev_add(cl_device_id dev)
{
	cl_device_id d = &in_head;
	cl_int r;

	if ((r = dev_is_valid(dev)) != CL_SUCCESS) {
		return r;
	}

	while (d->dev_next != NULL) {
		d = d->dev_next;
		if (d == dev) {
			log_err("device already existed.\n");
			return CL_INVALID_VALUE;
		}
	}

	dev->enabled = 1;

	if (dev->ops != NULL && dev->ops->probe != NULL) {
		r = dev->ops->probe(dev);
		if (r != CL_SUCCESS) {
			dev->enabled = 0;
		}
	}

	d->dev_next = dev;
	atomic_fetch_add_explicit(&in_dev_num, 1, memory_order_relaxed);

	return CL_SUCCESS;
}

cl_int dev_remove(cl_device_id dev)
{
	cl_device_id d = &in_head;
	cl_int r, res = CL_SUCCESS;
	int found = 0;

	if ((r = dev_is_valid(dev)) != CL_SUCCESS) {
		return r;
	}

	while (d->dev_next != NULL) {
		d = d->dev_next;
		if (d == dev) {
			found = 1;
			break;
		}
	}
	if (!found) {
		log_warn("device not found.\n");
		return CL_INVALID_VALUE;
	}

	if (dev->ops != NULL && dev->ops->remove != NULL) {
		r = dev->ops->remove(dev);
		if (r != CL_SUCCESS) {
			res = r;
		}
	}

	dev->enabled = 0;

	d->dev_next = dev->dev_next;
	atomic_fetch_add_explicit(&in_dev_num, -1, memory_order_relaxed);

	return res;
}

cl_int dev_reset(cl_device_id dev)
{
	cl_int r;

	if ((r = dev_is_valid(dev)) != CL_SUCCESS) {
		return r;
	}
	if (dev->ops == NULL || dev->ops->reset == NULL) {
		return CL_INVALID_DEVICE;
	}

	return dev->ops->reset(dev);
}

cl_int dev_read_mem(cl_device_id dev, uint64_t paddr, char *buf, uint64_t len)
{
	cl_int r;

	if ((r = dev_is_valid(dev)) != CL_SUCCESS) {
		return r;
	}
	if (dev->ops == NULL || dev->ops->read_mem == NULL) {
		return CL_INVALID_DEVICE;
	}

	return dev->ops->read_mem(dev, paddr, buf, len);
}

cl_int dev_write_mem(cl_device_id dev, uint64_t paddr, const char *buf, uint64_t len)
{
	cl_int r;

	if ((r = dev_is_valid(dev)) != CL_SUCCESS) {
		return r;
	}
	if (dev->ops == NULL || dev->ops->write_mem == NULL) {
		return CL_INVALID_DEVICE;
	}

	return dev->ops->write_mem(dev, paddr, buf, len);
}

cl_int dev_init(cl_platform_id platform)
{
	cl_int r, res = CL_SUCCESS;

	for (cl_int i = 0; i < ARRAY_OF(in_dev_groups); i++) {
		struct dev_group *grp = &in_dev_groups[i];

		if (grp->inited) {
			continue;
		}

		if (grp->init) {
			r = grp->init(platform);
			if (r != CL_SUCCESS) {
				res = r;
			}
		}

		grp->inited = 1;
	}

	return res;
}

cl_int dev_exit(cl_platform_id platform)
{
	cl_int r, res = CL_SUCCESS;

	for (cl_int i = 0; i < ARRAY_OF(in_dev_groups); i++) {
		struct dev_group *grp = &in_dev_groups[i];

		if (!grp->inited) {
			continue;
		}

		if (grp->exit) {
			r = grp->exit(platform);
			if (r != CL_SUCCESS) {
				res = r;
			}
		}

		grp->inited = 0;
	}

	return res;
}

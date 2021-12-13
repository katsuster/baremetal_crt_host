/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_DEVICE
#define BAREMETAL_CRT_HOST_DRV_DEVICE

#include <config_cl.h>

struct dev_ops {
	cl_int (*probe)(cl_device_id dev);
	cl_int (*remove)(cl_device_id dev);
};

struct _cl_device_id {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_platform_id plat;

	const struct dev_ops *ops;
	cl_device_type dev_type;

	void *priv;

	cl_device_id dev_next;
};

static inline cl_int dev_is_valid(cl_device_id device)
{
#ifdef OPENCL_ICD_ENABLE
	if (device == NULL) {
		return CL_INVALID_DEVICE;
	}
	if (device != NULL && device->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_DEVICE;
	}
#endif
	return CL_SUCCESS;
}

cl_int dev_get_number(void);
cl_int dev_get_devices(cl_device_id *pdev, cl_device_type typ, cl_uint *sz);

cl_int dev_alloc(cl_platform_id plat, cl_device_id *dev);
cl_int dev_free(cl_device_id dev);
cl_int dev_add(cl_device_id dev);
cl_int dev_remove(cl_device_id dev);

cl_int dev_init(cl_platform_id platform);
cl_int dev_exit(cl_platform_id platform);

#endif /* BAREMETAL_CRT_HOST_DRV_DEVICE */

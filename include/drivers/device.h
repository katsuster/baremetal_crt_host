/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_DEVICE
#define BAREMETAL_CRT_HOST_DRV_DEVICE

#include <config_cl.h>

struct _cl_device_id {
	struct _cl_icd_dispatch *dispatch;
	int magic;
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

int dev_get_number(void);
cl_device_id dev_get_devices(void);
cl_int dev_probe_devices(cl_platform_id platform);
cl_int dev_remove_devices(cl_platform_id platform);

#endif /* BAREMETAL_CRT_HOST_DRV_DEVICE */

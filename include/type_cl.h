/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_TYPE_CL
#define BAREMETAL_CRT_HOST_TYPE_CL

#include "config_cl.h"

struct _cl_platform_id {
	struct _cl_icd_dispatch *dispatch;
	int magic;
};

static inline cl_int platform_is_valid(cl_platform_id platform)
{
#ifdef OPENCL_ICD_ENABLE
	if (platform == NULL) {
		return CL_INVALID_PLATFORM;
	}
	if (platform != NULL && platform->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_PLATFORM;
	}
#endif
	return CL_SUCCESS;
}

struct _cl_device_id {
	int magic;
};

static inline cl_int device_is_valid(cl_device_id device)
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

#endif /* BAREMETAL_CRT_HOST_TYPE_CL */

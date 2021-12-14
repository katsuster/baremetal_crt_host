/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_PLATFORM
#define BAREMETAL_CRT_HOST_DRV_PLATFORM

#include <config_cl.h>

struct _cl_platform_id {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
};

static inline cl_int plat_is_valid(cl_platform_id platform)
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

cl_int plat_get_uniq_id(void);

#endif /* BAREMETAL_CRT_HOST_DRV_PLATFORM */

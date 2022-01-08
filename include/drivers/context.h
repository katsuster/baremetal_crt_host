/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_CONTEXT
#define BAREMETAL_CRT_HOST_DRV_CONTEXT

#include <config_cl.h>

struct _cl_context {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_device_id dev;
};

static inline cl_int ctx_is_valid(const cl_context ctx)
{
#ifdef OPENCL_ICD_ENABLE
	if (ctx == NULL) {
		return CL_INVALID_CONTEXT;
	}
	if (ctx != NULL && ctx->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_CONTEXT;
	}
#endif
	return CL_SUCCESS;
}

cl_int ctx_alloc(cl_device_id dev, cl_context *ctx);
cl_int ctx_free(cl_context ctx);

cl_device_id ctx_get_device(cl_context ctx);

#endif /* BAREMETAL_CRT_HOST_DRV_CONTEXT */

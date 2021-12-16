/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_MEM
#define BAREMETAL_CRT_HOST_DRV_MEM

#include <config_cl.h>

struct _cl_mem {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_context ctx;
};

static inline cl_int mem_is_valid(const cl_mem mem)
{
#ifdef OPENCL_ICD_ENABLE
	if (mem == NULL) {
		return CL_INVALID_MEM_OBJECT;
	}
	if (mem != NULL && mem->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_MEM_OBJECT;
	}
#endif
	return CL_SUCCESS;
}

cl_int mem_alloc(cl_context ctx, cl_mem *mem);
cl_int mem_free(cl_mem mem);

#endif /* BAREMETAL_CRT_HOST_DRV_MEM */

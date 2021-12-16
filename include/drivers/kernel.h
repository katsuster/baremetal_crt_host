/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_KERNEL
#define BAREMETAL_CRT_HOST_DRV_KERNEL

#include <config_cl.h>

struct _cl_kernel {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_program prg;
};

static inline cl_int kern_is_valid(const cl_kernel kern)
{
#ifdef OPENCL_ICD_ENABLE
	if (kern == NULL) {
		return CL_INVALID_KERNEL;
	}
	if (kern != NULL && kern->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_KERNEL;
	}
#endif
	return CL_SUCCESS;
}

cl_int kern_alloc(cl_program prg, cl_kernel *kern);
cl_int kern_free(cl_kernel kern);

#endif /* BAREMETAL_CRT_HOST_DRV_KERNEL */

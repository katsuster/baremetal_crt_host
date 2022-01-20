/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_KERNEL
#define BAREMETAL_CRT_HOST_DRV_KERNEL

#include <config_cl.h>

struct kern_arg {
	uint32_t argtype;
	uint32_t index;
	uint64_t size;
	const void *val;
};

struct _cl_kernel {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_program prg;

	cl_int num_args;
	struct kern_arg *args;
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

cl_program kern_get_program(cl_kernel kern);

cl_int kern_get_num_args(cl_kernel kern, cl_uint *num);
cl_int kern_set_num_args(cl_kernel kern, cl_uint num);
cl_int kern_get_arg(cl_kernel kern, cl_uint i, struct kern_arg *arg);
cl_int kern_set_arg(cl_kernel kern, cl_uint i, const struct kern_arg *arg);

#endif /* BAREMETAL_CRT_HOST_DRV_KERNEL */

/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>

#include <in_cl.h>
#include <drivers/kernel.h>
#include <drivers/platform.h>
#include <drivers/program.h>

cl_int kern_alloc(cl_program prg, cl_kernel *kern)
{
	cl_kernel t;
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}
	if (kern == NULL) {
		return CL_INVALID_VALUE;
	}

	t = calloc(1, sizeof(struct _cl_kernel));
	if (t == NULL) {
		log_err("cannot calloc cl_kernel.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	t->dispatch = prg->dispatch;
	t->magic = OPENCL_ICD_MAGIC;
	t->id = plat_get_uniq_id();
	t->prg = prg;

	*kern = t;

	return CL_SUCCESS;
}

cl_int kern_free(cl_kernel kern)
{
	cl_int r;

	if ((r = kern_is_valid(kern)) != CL_SUCCESS) {
		return r;
	}

	free(kern);

	return CL_SUCCESS;
}

cl_program kern_get_program(cl_kernel kern)
{
	if (kern_is_valid(kern) != CL_SUCCESS) {
		return NULL;
	}

	return kern->prg;
}

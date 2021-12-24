/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>

#include <in_cl.h>
#include <drivers/mem.h>
#include <drivers/context.h>
#include <drivers/platform.h>

cl_int mem_alloc(cl_context ctx, cl_mem *mem)
{
	cl_mem t;
	cl_int r;

	if ((r = ctx_is_valid(ctx)) != CL_SUCCESS) {
		return r;
	}
	if (mem == NULL) {
		return CL_INVALID_VALUE;
	}

	t = calloc(1, sizeof(struct _cl_mem));
	if (t == NULL) {
		log_err("cannot calloc cl_mem.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	t->dispatch = ctx->dispatch;
	t->magic = OPENCL_ICD_MAGIC;
	t->id = plat_get_uniq_id();
	t->ctx = ctx;

	*mem = t;

	return CL_SUCCESS;
}

cl_int mem_free(cl_mem mem)
{
	cl_int r;

	if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
		return r;
	}

	free(mem);

	return CL_SUCCESS;
}

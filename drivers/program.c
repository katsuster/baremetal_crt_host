/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>

#include <in_cl.h>
#include <drivers/program.h>
#include <drivers/context.h>

cl_int prg_alloc(cl_context ctx, cl_program *prg)
{
	cl_program t;
	cl_int r;

	if ((r = ctx_is_valid(ctx)) != CL_SUCCESS) {
		return r;
	}
	if (prg == NULL) {
		return CL_INVALID_VALUE;
	}

	t = calloc(1, sizeof(struct _cl_program));
	if (t == NULL) {
		log_err("cannot calloc cl_program.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	t->dispatch = ctx->dispatch;
	t->magic = OPENCL_ICD_MAGIC;
	t->id = plat_get_uniq_id();
	t->ctx = ctx;

	*prg = t;

	return CL_SUCCESS;

}

cl_int prg_free(cl_program prg)
{
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}

	free(prg);

	return CL_SUCCESS;
}

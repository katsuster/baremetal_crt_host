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

cl_int prg_get_num_chunks(cl_program prg, int *num)
{
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}

	if (num != NULL) {
		*num = prg->num_chunks;
	}

	return CL_SUCCESS;
}

cl_int prg_set_num_chunks(cl_program prg, int num)
{
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}

	if (prg->chunks != NULL) {
		free(prg->chunks);
		prg->chunks = NULL;
	}

	prg->chunks = calloc(num, sizeof(struct program_chunk));
	if (prg->chunks == NULL) {
		log_err("Failed to alloc program_chunk.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}
	prg->num_chunks = num;

	return CL_SUCCESS;
}

cl_int prg_get_chunk(cl_program prg, int i, struct program_chunk *chunk)
{
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}
	if (i < 0 || prg->num_chunks <= i) {
		return CL_INVALID_VALUE;
	}

	if (chunk) {
		*chunk = prg->chunks[i];
	}

	return CL_SUCCESS;
}

cl_int prg_set_chunk(cl_program prg, int i, const struct program_chunk *chunk)
{
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}
	if (chunk == NULL || i < 0 || prg->num_chunks <= i) {
		return CL_INVALID_VALUE;
	}

	prg->chunks[i] = *chunk;
	prg->chunks[i].index = i;

	return CL_SUCCESS;
}

cl_int prg_get_comm_section(cl_program prg, struct comm_section *comm)
{
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}

	if (comm != NULL) {
		*comm = prg->comm;
	}

	return CL_SUCCESS;
}

cl_int prg_set_comm_section(cl_program prg, const struct comm_section *comm)
{
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}
	if (comm == NULL) {
		return CL_INVALID_VALUE;
	}

	prg->comm = *comm;

	return CL_SUCCESS;
}

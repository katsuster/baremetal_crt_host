/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>

#include <in_cl.h>
#include <drivers/command_queue.h>
#include <drivers/platform.h>
#include <drivers/context.h>

cl_int cmdq_alloc(cl_context ctx, cl_command_queue *cmdq)
{
	cl_command_queue t;
	cl_int r;

	if ((r = ctx_is_valid(ctx)) != CL_SUCCESS) {
		return r;
	}
	if (cmdq == NULL) {
		return CL_INVALID_VALUE;
	}

	t = calloc(1, sizeof(struct _cl_command_queue));
	if (t == NULL) {
		log_err("cannot calloc cl_command_queue.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	t->dispatch = ctx->dispatch;
	t->magic = OPENCL_ICD_MAGIC;
	t->id = plat_get_uniq_id();
	t->ctx = ctx;

	*cmdq = t;

	return CL_SUCCESS;
}

cl_int cmdq_free(cl_command_queue cmdq)
{
	cl_int r;

	if ((r = cmdq_is_valid(cmdq)) != CL_SUCCESS) {
		return r;
	}

	free(cmdq);

	return CL_SUCCESS;
}

cl_context cmdq_get_context(cl_command_queue cmdq)
{
	if (cmdq_is_valid(cmdq) != CL_SUCCESS) {
		return NULL;
	}

	return cmdq->ctx;
}

/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>

#include <in_cl.h>
#include <drivers/context.h>
#include <drivers/platform.h>

cl_int ctx_alloc(cl_device_id dev, cl_context *ctx)
{
	cl_context t;
	cl_int r;

	if ((r = dev_is_valid(dev)) != CL_SUCCESS) {
		return r;
	}
	if (ctx == NULL) {
		return CL_INVALID_VALUE;
	}

	t = calloc(1, sizeof(struct _cl_context));
	if (t == NULL) {
		log_err("cannot calloc cl_context.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	t->dispatch = dev->dispatch;
	t->magic = OPENCL_ICD_MAGIC;
	t->id = plat_get_uniq_id();
	t->dev = dev;

	*ctx = t;

	return CL_SUCCESS;
}

cl_int ctx_free(cl_context ctx)
{
	cl_int r;

	if ((r = ctx_is_valid(ctx)) != CL_SUCCESS) {
		return r;
	}

	free(ctx);

	return CL_SUCCESS;
}

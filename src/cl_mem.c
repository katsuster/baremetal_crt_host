/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/mem.h>
#include <drivers/context.h>

cl_mem in_clCreateBuffer(cl_context   context,
			 cl_mem_flags flags,
			 size_t       size,
			 void         *host_ptr,
			 cl_int       *errcode_ret)
{
	cl_mem mem;
	cl_int r = CL_SUCCESS;

	if (size == 0) {
		r = CL_INVALID_BUFFER_SIZE;
		goto err_out;
	}
	if (host_ptr == NULL) {
		r = CL_INVALID_HOST_PTR;
		goto err_out;
	}
	if ((r = ctx_is_valid(context)) != CL_SUCCESS) {
		goto err_out;
	}

	r = mem_alloc(context, &mem);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	return mem;

err_out:
	if (errcode_ret != NULL) {
		*errcode_ret = r;
	}

	return NULL;
}

cl_int in_clRetainMemObject(cl_mem memobj)
{
	return CL_INVALID_MEM_OBJECT;
}

cl_int in_clReleaseMemObject(cl_mem memobj)
{
	cl_int r;

	if ((r = mem_is_valid(memobj)) != CL_SUCCESS) {
		return r;
	}

	r = mem_free(memobj);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

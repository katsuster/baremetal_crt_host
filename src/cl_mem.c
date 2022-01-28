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
	cl_mem_flags mask_rw = CL_MEM_READ_WRITE | CL_MEM_READ_ONLY | CL_MEM_WRITE_ONLY;
	cl_mem_flags mask_host = CL_MEM_USE_HOST_PTR | CL_MEM_ALLOC_HOST_PTR | CL_MEM_COPY_HOST_PTR;
	cl_mem mem;
	cl_int r = CL_SUCCESS;
	int need_host_ptr = 0;

	if (flags == 0) {
		flags = CL_MEM_READ_WRITE;
	}
	switch (flags & mask_rw) {
	case CL_MEM_READ_WRITE:
	case CL_MEM_WRITE_ONLY:
	case CL_MEM_READ_ONLY:
		/* Valid */
		break;
	default:
		r = CL_INVALID_VALUE;
		goto err_out;
	}
	switch (flags & mask_host) {
	case 0:
		/* Valid if not specified */
		break;
	case CL_MEM_ALLOC_HOST_PTR:
		need_host_ptr = 0;
		break;
	case CL_MEM_USE_HOST_PTR:
	case CL_MEM_COPY_HOST_PTR:
	case CL_MEM_ALLOC_HOST_PTR | CL_MEM_COPY_HOST_PTR:
		need_host_ptr = 1;
		break;
	default:
		r = CL_INVALID_VALUE;
		goto err_out;
	}

	if (size == 0) {
		r = CL_INVALID_BUFFER_SIZE;
		goto err_out;
	}
	if (host_ptr == NULL && need_host_ptr) {
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

	r = mem_set_attr(mem, flags);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	r = mem_alloc_buffer(mem, size, host_ptr);
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

	r = mem_free_buffer(memobj);
	if (r != CL_SUCCESS) {
		/* Ignore error */
	}

	r = mem_free(memobj);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

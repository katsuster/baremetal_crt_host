/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

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

cl_int mem_get_attr(cl_mem mem, cl_int *flags)
{
	cl_int r;

	if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
		return r;
	}

	if (flags != NULL) {
		*flags = mem->flags;
	}

	return CL_SUCCESS;
}

cl_int mem_set_attr(cl_mem mem, cl_int flags)
{
	cl_int r;

	if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
		return r;
	}

	mem->flags = flags;

	return CL_SUCCESS;
}

cl_int mem_alloc_buffer(cl_mem mem, size_t size, void *ptr)
{
	cl_int r;

	if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
		return r;
	}

	if (mem_need_alloc_host(mem)) {
		/* TODO: not supported to alloc device memory */
		log_err("cannot alloc device memory.\n");
		return CL_OUT_OF_RESOURCES;
	} else if (mem_need_use_host(mem)) {
		mem->ptr = ptr;
	} else {
		mem->ptr = calloc(size, sizeof(char));
		if (mem->ptr == NULL) {
			log_err("cannot calloc buffer.\n");
			return CL_OUT_OF_HOST_MEMORY;
		}
		if (mem_need_copy_host(mem)) {
			memcpy(mem->ptr, ptr, size);
		}
	}
	mem->size = size;

	return CL_SUCCESS;
}

cl_int mem_free_buffer(cl_mem mem)
{
	cl_int r;

	if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
		return r;
	}

	if (mem->ptr != NULL) {
		free(mem->ptr);
		mem->ptr = NULL;
		mem->size = 0;
	}

	return CL_SUCCESS;
}

cl_int mem_read_buffer(cl_mem mem, size_t offset, size_t size, void *ptr)
{
	char *bufptr;
	cl_int r;

	if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
		return r;
	}
	if (ptr == NULL || offset + size > mem->size) {
		return CL_INVALID_VALUE;
	}

	bufptr = &mem->ptr[offset];
	memcpy(ptr, bufptr, size);

	return CL_SUCCESS;
}

cl_int mem_write_buffer(cl_mem mem, size_t offset, size_t size, const void *ptr)
{
	char *bufptr;
	cl_int r;

	if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
		return r;
	}
	if (ptr == NULL || offset + size > mem->size) {
		return CL_INVALID_VALUE;
	}

	bufptr = &mem->ptr[offset];
	memcpy(bufptr, ptr, size);

	return CL_SUCCESS;
}

int mem_can_read(cl_mem mem)
{
	return !!(mem->flags & CL_MEM_READ_WRITE) || !!(mem->flags & CL_MEM_READ_ONLY);
}

int mem_can_write(cl_mem mem)
{
	return !!(mem->flags & CL_MEM_READ_WRITE) || !!(mem->flags & CL_MEM_WRITE_ONLY);
}

int mem_need_use_host(cl_mem mem)
{
	return !!(mem->flags & CL_MEM_USE_HOST_PTR);
}

int mem_need_alloc_host(cl_mem mem)
{
	return !!(mem->flags & CL_MEM_ALLOC_HOST_PTR);
}

int mem_need_copy_host(cl_mem mem)
{
	return !!(mem->flags & CL_MEM_COPY_HOST_PTR);
}

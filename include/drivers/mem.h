/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_MEM
#define BAREMETAL_CRT_HOST_DRV_MEM

#include <config_cl.h>

struct _cl_mem {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_context ctx;

	cl_int flags;
	size_t size;
	char *ptr;
};

static inline cl_int mem_is_valid(const cl_mem mem)
{
#ifdef OPENCL_ICD_ENABLE
	if (mem == NULL) {
		return CL_INVALID_MEM_OBJECT;
	}
	if (mem != NULL && mem->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_MEM_OBJECT;
	}
#endif
	return CL_SUCCESS;
}

cl_int mem_alloc(cl_context ctx, cl_mem *mem);
cl_int mem_free(cl_mem mem);
cl_int mem_get_attr(cl_mem mem, cl_int *flags);
cl_int mem_set_attr(cl_mem mem, cl_int flags);
cl_int mem_alloc_buffer(cl_mem mem, size_t size, void *ptr);
cl_int mem_free_buffer(cl_mem mem);
cl_int mem_read_buffer(cl_mem mem, size_t offset, size_t size, void *ptr);
cl_int mem_write_buffer(cl_mem mem, size_t offset, size_t size, const void *ptr);

int mem_can_read(cl_mem mem);
int mem_can_write(cl_mem mem);
int mem_need_use_host(cl_mem mem);
int mem_need_alloc_host(cl_mem mem);
int mem_need_copy_host(cl_mem mem);

#endif /* BAREMETAL_CRT_HOST_DRV_MEM */

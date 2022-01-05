/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_PROGRAM
#define BAREMETAL_CRT_HOST_DRV_PROGRAM

#include <config_cl.h>

struct _cl_program {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_context ctx;

	void *priv;
};

struct program_chunk {
	cl_int index;
	void *buf;
	cl_uint size;
};

static inline cl_int prg_is_valid(const cl_program program)
{
#ifdef OPENCL_ICD_ENABLE
	if (program == NULL) {
		return CL_INVALID_PROGRAM;
	}
	if (program != NULL && program->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_PROGRAM;
	}
#endif
	return CL_SUCCESS;
}

cl_int prg_alloc(cl_context ctx, cl_program *prg);
cl_int prg_free(cl_program prg);

/* binary: ELF */
cl_int prg_elf_load(cl_program prg, const unsigned char *buf, size_t len);
cl_int prg_elf_unload(cl_program prg);

#endif /* BAREMETAL_CRT_HOST_DRV_PROGRAM */

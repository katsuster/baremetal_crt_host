/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_PROGRAM
#define BAREMETAL_CRT_HOST_DRV_PROGRAM

#include <config_cl.h>
#include <drivers/comm.h>

struct program_chunk {
	int index;
	uint64_t paddr;
	const void *buf;
	cl_uint size;
	int readonly;
	int loaded;
};

struct program_comm {
	uint64_t addr;
	uint64_t size;
};

struct _cl_program {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_context ctx;

	void *priv;

	/* Number of text/data areas of program */
	int num_chunks;

	/* Array of text/data area of program */
	struct program_chunk *chunks;

	/*
	 * Communication area for device side to pass various data:
	 *   - arguments (device <- host)
	 *   - results (host <- device)
	 */
	struct program_comm comm;
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

cl_int prg_get_num_chunks(cl_program prg, int *num);
cl_int prg_set_num_chunks(cl_program prg, int num);
cl_int prg_get_chunk(cl_program prg, int i, struct program_chunk *chunk);
cl_int prg_set_chunk(cl_program prg, int i, const struct program_chunk *chunk);

cl_int prg_get_comm(cl_program prg, struct program_comm *comm);
cl_int prg_set_comm(cl_program prg, const struct program_comm *comm);

/* binary: ELF */
cl_int prg_elf_load(cl_program prg, const unsigned char *buf, size_t len);
cl_int prg_elf_unload(cl_program prg);

#endif /* BAREMETAL_CRT_HOST_DRV_PROGRAM */

/* SPDX-License-Identifier: Apache-2.0 */

#include <elf.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <in_cl.h>
#include <drivers/program.h>
#include <drivers/context.h>

struct prg_header {
	// Program header
	uint32_t p_type;
	uint32_t p_flags;
	uint64_t p_offset;
	uint64_t p_vaddr;
	uint64_t p_paddr;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint64_t p_align;

	// Memory chunk (size: p_filesz)
	unsigned int index;
	const uint8_t *src;
};

struct elf_info {
	cl_int magic;

	cl_program prg;

	size_t elf_size;
	const uint8_t *buf;
	const uint8_t *p_hdr_ent;

	// ELF header
	uint32_t elf_class;
	uint32_t e_type;
	uint32_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint32_t e_ehsize;
	uint32_t e_phentsize;
	uint32_t e_phnum;
	uint32_t e_shentsize;
	uint32_t e_shnum;
	uint32_t e_shstrndx;

	// Program headers (len: e_phnum)
	struct prg_header *prgs;
};

static inline cl_int prg_elf_is_valid(const cl_program prg)
{
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}

	const struct elf_info *elf = prg->priv;

	if (elf != NULL && elf->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_PROGRAM;
	}

	return CL_SUCCESS;
}

static cl_int prg_elf_init(cl_program prg)
{
	struct elf_info *elf = NULL;

	elf = calloc(1, sizeof(struct elf_info));
	if (elf == NULL) {
		log_err("cannot calloc elf_info.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	elf->magic = OPENCL_ICD_MAGIC;
	elf->prg = prg;
	elf->buf = NULL;
	elf->prgs = NULL;

	prg->priv = elf;

	return CL_SUCCESS;
}

static cl_int prg_elf_fini(cl_program prg)
{
	struct elf_info *elf = prg->priv;

	prg->priv = NULL;

	if (elf->prgs != NULL) {
		free(elf->prgs);
		elf->prgs = NULL;
	}

	elf->buf = NULL;

	free(elf);

	return CL_SUCCESS;
}

static cl_int prg_elf_get_class(const uint8_t *e_ident, int *parch)
{
	int arch;

	// ELF magic number
	if ((e_ident[0] != ELFMAG0)
	    || (e_ident[1] != ELFMAG1)
	    || (e_ident[2] != ELFMAG2)
	    || (e_ident[3] != ELFMAG3)) {
		log_err("[elf] invalid magic number\n");
		return CL_INVALID_BINARY;
	}
	log_dbg("  ELF ident      : %02x %02x %02x %02x %02x\n",
		e_ident[0], e_ident[1],
		e_ident[2], e_ident[3],
		e_ident[4]);

	// architecture class
	switch (e_ident[EI_CLASS]) {
	case ELFCLASS32:
		log_dbg("  ELF class      : 32\n");
		arch = 32;
		break;
	case ELFCLASS64:
		log_dbg("  ELF class      : 64\n");
		arch = 64;
		break;
	default:
		log_err("[elf] unknown architecture class\n");
		return CL_INVALID_BINARY;
	}
	if (parch != NULL) {
		*parch = arch;
	}

	return CL_SUCCESS;
}

static cl_int prg_elf_is_valid_header(struct elf_info *elf)
{
	if (elf->e_type != ET_EXEC) {
		log_err("[elf] support only executable type\n");
		return CL_INVALID_BINARY;
	}

	if (elf->e_machine != EM_RISCV) {
		log_err("[elf] support only RISC-V machine\n");
		return CL_INVALID_BINARY;
	}

	if (elf->e_phoff == 0 || elf->e_phnum == 0 || elf->e_phentsize == 0) {
		log_err("[elf] no program header\n");
		return CL_INVALID_BINARY;
	}

	if (elf->elf_size < (elf->e_phoff + (elf->e_phnum * elf->e_phentsize))) {
		log_err("[elf] program header exceeds file size\n");
		return CL_INVALID_BINARY;
	}

	if (elf->e_shentsize == 0) {
		log_err("[elf] invalid section header\n");
		return CL_INVALID_BINARY;
	}

	if (elf->elf_size < (elf->e_shoff + (elf->e_shnum * elf->e_shentsize))) {
		log_err("[elf] section header exceeds file size\n");
		return CL_INVALID_BINARY;
	}

	// entry point address
	if ((elf->e_entry % 4) != 0) {
		log_err("[elf] entry violates 4bytes alignment."
			"0x%" PRIx64 "\n", elf->e_entry);
		return CL_INVALID_BINARY;
	}

	return CL_SUCCESS;
}

static cl_int prg_elf_read_elf32(struct elf_info *elf)
{
	const Elf32_Ehdr *elf_hdr = (const Elf32_Ehdr *)elf->buf;

	elf->e_type      = elf_hdr->e_type;
	elf->e_machine   = elf_hdr->e_machine;
	elf->e_version   = elf_hdr->e_version;
	elf->e_entry     = elf_hdr->e_entry;
	elf->e_phoff     = elf_hdr->e_phoff;
	elf->e_shoff     = elf_hdr->e_shoff;
	elf->e_flags     = elf_hdr->e_flags;
	elf->e_ehsize    = elf_hdr->e_ehsize;
	elf->e_phentsize = elf_hdr->e_phentsize;
	elf->e_phnum     = elf_hdr->e_phnum;
	elf->e_shentsize = elf_hdr->e_shentsize;
	elf->e_shnum     = elf_hdr->e_shnum;
	elf->e_shstrndx  = elf_hdr->e_shstrndx;

	return CL_SUCCESS;
}

static cl_int prg_elf_read_elf64(struct elf_info *elf)
{
	const Elf64_Ehdr *elf_hdr = (const Elf64_Ehdr *)elf->buf;

	elf->e_type      = elf_hdr->e_type;
	elf->e_machine   = elf_hdr->e_machine;
	elf->e_version   = elf_hdr->e_version;
	elf->e_entry     = elf_hdr->e_entry;
	elf->e_phoff     = elf_hdr->e_phoff;
	elf->e_shoff     = elf_hdr->e_shoff;
	elf->e_flags     = elf_hdr->e_flags;
	elf->e_ehsize    = elf_hdr->e_ehsize;
	elf->e_phentsize = elf_hdr->e_phentsize;
	elf->e_phnum     = elf_hdr->e_phnum;
	elf->e_shentsize = elf_hdr->e_shentsize;
	elf->e_shnum     = elf_hdr->e_shnum;
	elf->e_shstrndx  = elf_hdr->e_shstrndx;

	return CL_SUCCESS;
}

static cl_int prg_elf_read_elf(struct elf_info *elf)
{
	cl_int r;

	switch (elf->elf_class) {
	case 32:
		r = prg_elf_read_elf32(elf);
		break;
	case 64:
		r = prg_elf_read_elf64(elf);
		break;
	default:
		log_err("architecture class is not 32 nor 64\n");
		return CL_INVALID_BINARY;
	}
	if (r != CL_SUCCESS) {
		return r;
	}

	if (elf->e_phentsize < sizeof(Elf64_Phdr)) {
		log_err("not support program header size %" PRId32 "\n",
			elf->e_phentsize);
		return CL_INVALID_BINARY;
	}

	elf->p_hdr_ent = elf->buf + elf->e_phoff;

	return CL_SUCCESS;
}

static void prg_elf_dump_elf(const struct elf_info *elf)
{
	log_dbg("  ELF entry point: 0x%" PRIx64 "\n", elf->e_entry);
	log_dbg("  program num    : %d\n", elf->e_phnum);
	log_dbg("  program size   : %d\n", elf->e_phentsize);
	log_dbg("  program offset : 0x%" PRIx64 "\n", elf->e_phoff);
	log_dbg("  program addr   : %p\n", elf->p_hdr_ent);
}

static cl_int prg_elf_read_prg32(struct elf_info *elf)
{
	struct prg_header *prgs = NULL;

	prgs = calloc(elf->e_phnum, sizeof(struct prg_header));
	if (prgs == NULL) {
		log_err("failed to calloc prg_header.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}
	elf->prgs = prgs;

	for (uint32_t i = 0; i < elf->e_phnum; i++) {
		const Elf32_Phdr *p_hdr = (const Elf32_Phdr *)(elf->p_hdr_ent + i * elf->e_phentsize);

		prgs[i].p_type   = p_hdr->p_type;
		prgs[i].p_flags  = p_hdr->p_flags;
		prgs[i].p_offset = p_hdr->p_offset;
		prgs[i].p_vaddr  = p_hdr->p_vaddr;
		prgs[i].p_paddr  = p_hdr->p_paddr;
		prgs[i].p_filesz = p_hdr->p_filesz;
		prgs[i].p_memsz  = p_hdr->p_memsz;
		prgs[i].p_align  = p_hdr->p_align;
	}

	return CL_SUCCESS;
}

static cl_int prg_elf_read_prg64(struct elf_info *elf)
{
	struct prg_header *prgs = NULL;

	prgs = calloc(elf->e_phnum, sizeof(struct prg_header));
	if (prgs == NULL) {
		log_err("failed to calloc prg_chunk.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}
	elf->prgs = prgs;

	for (uint32_t i = 0; i < elf->e_phnum; i++) {
		const Elf64_Phdr *p_hdr = (const Elf64_Phdr *)(elf->p_hdr_ent + i * elf->e_phentsize);

		prgs[i].p_type   = p_hdr->p_type;
		prgs[i].p_flags  = p_hdr->p_flags;
		prgs[i].p_offset = p_hdr->p_offset;
		prgs[i].p_vaddr  = p_hdr->p_vaddr;
		prgs[i].p_paddr  = p_hdr->p_paddr;
		prgs[i].p_filesz = p_hdr->p_filesz;
		prgs[i].p_memsz  = p_hdr->p_memsz;
		prgs[i].p_align  = p_hdr->p_align;
	}

	return CL_SUCCESS;
}

static cl_int prg_elf_read_prg(struct elf_info *elf)
{
	cl_int r;

	switch (elf->elf_class) {
	case 32:
		r = prg_elf_read_prg32(elf);
		break;
	case 64:
		r = prg_elf_read_prg64(elf);
		break;
	default:
		log_err("architecture class is not 32 nor 64\n");
		return CL_INVALID_BINARY;
	}
	if (r != CL_SUCCESS) {
		return r;
	}

	r = prg_set_num_chunks(elf->prg, elf->e_phnum);
	if (r != CL_SUCCESS) {
		return r;
	}

	for (uint32_t i = 0; i < elf->e_phnum; i++) {
		struct prg_header *prg = &elf->prgs[i];
		struct program_chunk chunk;
		const uint8_t *src = elf->buf + prg->p_offset;

		prg->index = i;
		prg->src = src;

		chunk.paddr = prg->p_paddr;
		chunk.buf = src;
		chunk.size = prg->p_filesz;
		r = prg_set_chunk(elf->prg, i, &chunk);
		if (r != CL_SUCCESS) {
			return r;
		}
	}

	return CL_SUCCESS;
}

static void prg_elf_dump_prg(const struct elf_info *elf)
{
	for (uint32_t i = 0; i < elf->e_phnum; i++) {
		const struct prg_header *prg = &elf->prgs[i];

		log_dbg("  ph: %d, p_hdr: %p, offset: 0x%x\n",
			i, elf->p_hdr_ent + i * elf->e_phentsize, i * elf->e_phentsize);
		log_dbg("    ph type  : 0x%04x\n", prg->p_type);
		log_dbg("    ph offset: 0x%" PRIx64 "\n", prg->p_offset);
		log_dbg("    ph vaddr : 0x%" PRIx64 "\n", prg->p_vaddr);
		log_dbg("    ph paddr : 0x%" PRIx64 "\n", prg->p_paddr);
		log_dbg("    ph filesz: 0x%" PRIx64 "\n", prg->p_filesz);
		log_dbg("    ph flag  : %s%s%s (0x%04x)\n",
			(prg->p_flags & PF_W) ? "w" : "-",
			(prg->p_flags & PF_R) ? "r" : "-",
			(prg->p_flags & PF_X) ? "x" : "-",
			prg->p_flags);
		if (prg->p_type != PT_LOAD || prg->p_filesz == 0) {
			log_dbg("    do not need to load, skipped\n");
			continue;
		}
		log_dbg("  copy paddr: 0x%" PRIx64 "-0x%" PRIx64 ", size: 0x%" PRIx64 "\n",
			prg->p_paddr, prg->p_paddr + prg->p_filesz,
			prg->p_filesz);

		log_dbg("    elf hdr_ptr: %p, offset: 0x%" PRIx64 ", src: %p\n",
			elf->buf, prg->p_offset, prg->src);
	}
}

cl_int prg_elf_load(cl_program prg, const uint8_t *buf, size_t len)
{
	int arch;
	cl_int r;

	// Not check prg->priv
	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}

	r = prg_elf_init(prg);
	if ((r = prg_elf_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}

	struct elf_info *elf = prg->priv;

	elf->buf = buf;
	elf->elf_size = len;

	log_dbg("  ELF addr       : %p\n", elf->buf);

	r = prg_elf_get_class(elf->buf, &arch);
	if (r != CL_SUCCESS) {
		return r;
	}
	elf->elf_class = arch;

	r = prg_elf_read_elf(elf);
	if (r != CL_SUCCESS) {
		return r;
	}
	prg_elf_dump_elf(elf);

	r = prg_elf_is_valid_header(elf);
	if (r != CL_SUCCESS) {
		return r;
	}

	r = prg_elf_read_prg(elf);
	if (r != CL_SUCCESS) {
		return r;
	}
	prg_elf_dump_prg(elf);

	return CL_SUCCESS;
}

cl_int prg_elf_unload(cl_program prg)
{
	cl_int r, res = CL_SUCCESS;

	if ((r = prg_elf_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}

	r = prg_elf_fini(prg);
	if (r != CL_SUCCESS) {
		// Continue anyway
		res = r;
	}

	return res;
}

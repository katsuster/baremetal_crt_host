/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_COMM
#define BAREMETAL_CRT_HOST_DRV_COMM

#define BAREMETAL_CRT_COMM_SECTION    ".comm"
#define BAREMETAL_CRT_COMM_MAGIC      0xbadcafe5

struct comm_section {
	uint32_t magic;
	uint64_t base_addr;
	uint64_t phys_addr;
	uint64_t size;
};

struct comm_area_header {
	uint32_t magic;
	uint32_t num_args;
};

struct comm_arg_header {
	uint32_t argtype;
	uint32_t index;
	uint64_t size;
};

#endif /* BAREMETAL_CRT_HOST_DRV_COMM */

/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_DRV_COMMAND_QUEUE
#define BAREMETAL_CRT_HOST_DRV_COMMAND_QUEUE

#include <config_cl.h>

struct _cl_command_queue {
	struct _cl_icd_dispatch *dispatch;
	cl_int magic;
	cl_int id;
	cl_context ctx;
};

static inline cl_int cmdq_is_valid(const cl_command_queue cmdq)
{
#ifdef OPENCL_ICD_ENABLE
	if (cmdq == NULL) {
		return CL_INVALID_COMMAND_QUEUE;
	}
	if (cmdq != NULL && cmdq->magic != OPENCL_ICD_MAGIC) {
		return CL_INVALID_COMMAND_QUEUE;
	}
#endif
	return CL_SUCCESS;
}

cl_int cmdq_alloc(cl_context ctx, cl_command_queue *cmdq);
cl_int cmdq_free(cl_command_queue cmdq);


#endif /* BAREMETAL_CRT_HOST_DRV_COMMAND_QUEUE */

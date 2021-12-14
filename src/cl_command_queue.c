/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/command_queue.h>
#include <drivers/context.h>
#include <drivers/device.h>

cl_command_queue in_clCreateCommandQueueWithProperties(cl_context               context,
						       cl_device_id             device,
						       const cl_queue_properties *properties,
						       cl_int                   *errcode_ret)
{
	cl_command_queue cmdq;
	cl_int r = CL_SUCCESS;

	if ((r = ctx_is_valid(context)) != CL_SUCCESS) {
		goto err_out;
	}
	if ((r = dev_is_valid(device)) != CL_SUCCESS) {
		goto err_out;
	}

	/* TODO: check device is associated with context */

	r = cmdq_alloc(context, &cmdq);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	return cmdq;

err_out:
	if (errcode_ret != NULL) {
		*errcode_ret = r;
	}

	return NULL;
}

cl_int in_clRetainCommandQueue(cl_command_queue command_queue)
{
	return CL_INVALID_COMMAND_QUEUE;
}

cl_int in_clReleaseCommandQueue(cl_command_queue command_queue)
{
	cl_int r;

	if ((r = cmdq_is_valid(command_queue)) != CL_SUCCESS) {
		return r;
	}

	r = cmdq_free(command_queue);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_INVALID_COMMAND_QUEUE;
}

cl_int in_clGetCommandQueueInfo(cl_command_queue      command_queue,
				cl_command_queue_info param_name,
				size_t                param_value_size,
				void                  *param_value,
				size_t                *param_value_size_ret)
{
	return CL_INVALID_COMMAND_QUEUE;
}

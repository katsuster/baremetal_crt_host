/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/program.h>
#include <drivers/context.h>
#include <drivers/device.h>

cl_program in_clCreateProgramWithSource(cl_context   context,
					cl_uint      count,
					const char   **strings,
					const size_t *lengths,
					cl_int       *errcode_ret)
{
	return NULL;
}

cl_program in_clCreateProgramWithBinary(cl_context          context,
					cl_uint             num_devices,
					const cl_device_id  *device_list,
					const size_t        *lengths,
					const unsigned char **binaries,
					cl_int              *binary_status,
					cl_int              *errcode_ret)
{
	cl_program t;
	cl_int r;

	if (lengths == NULL || binaries == NULL) {
		r = CL_INVALID_VALUE;
		goto err_out;
	}
	if ((r = ctx_is_valid(context)) != CL_SUCCESS) {
		goto err_out;
	}
	for (cl_uint i = 0; i < num_devices; i++) {
		const cl_device_id dev = device_list[i];

		if (lengths[i] == 0 || binaries[i] == NULL) {
			r = CL_INVALID_VALUE;
			goto err_out;
		}
		if ((r = dev_is_valid(dev)) != CL_SUCCESS) {
			goto err_out;
		}
	}

	/* TODO: support multiple devices */
	/* TODO: remove duplicated devices */
	if (num_devices > 1) {
		r = CL_INVALID_VALUE;
		goto err_out;
	}

	r = prg_alloc(context, &t);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	return t;
err_out:
	if (errcode_ret != NULL) {
		*errcode_ret = r;
	}

	return NULL;
}

cl_int in_clRetainProgram(cl_program program)
{
	return CL_INVALID_PROGRAM;
}

cl_int in_clReleaseProgram(cl_program program)
{
	cl_int r;

	if ((r = prg_is_valid(program)) != CL_SUCCESS) {
		return r;
	}

	r = prg_free(program);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

cl_int in_clBuildProgram(cl_program         program,
			 cl_uint            num_devices,
			 const cl_device_id *device_list,
			 const char         *options,
			 void (CL_CALLBACK *pfn_notify)(cl_program program,
							void *user_data),
			 void               *user_data)
{
	cl_int r;

	if ((r = prg_is_valid(program)) != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

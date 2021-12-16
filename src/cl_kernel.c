/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/kernel.h>
#include <drivers/program.h>

cl_kernel in_clCreateKernel(cl_program program,
			    const char *kernel_name,
			    cl_int     *errcode_ret)
{
	cl_kernel kern;
	cl_int r = CL_SUCCESS;

	if (kernel_name == NULL) {
		r = CL_INVALID_KERNEL_NAME;
		goto err_out;
	}
	if ((r = prg_is_valid(program)) != CL_SUCCESS) {
		goto err_out;
	}

	r = kern_alloc(program, &kern);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	return kern;

err_out:
	if (errcode_ret != NULL) {
		*errcode_ret = r;
	}

	return NULL;
}

cl_int in_clCreateKernelsInProgram(cl_program program,
				   cl_uint    num_kernels,
				   cl_kernel  *kernels,
				   cl_uint    *num_kernels_ret)
{
	return CL_INVALID_PROGRAM;
}

cl_kernel in_clCloneKernel(cl_kernel source_kernel, cl_int *errcode_ret)
{
	return NULL;
}

cl_int in_clRetainKernel(cl_kernel kernel)
{
	return CL_INVALID_KERNEL;
}

cl_int in_clReleaseKernel(cl_kernel kernel)
{
	cl_int r;

	if ((r = kern_is_valid(kernel)) != CL_SUCCESS) {
		return r;
	}

	r = kern_free(kernel);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

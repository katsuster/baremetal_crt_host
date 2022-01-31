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

	/* TODO: how to get number of args of program?? */
	int n = 8;

	r = kern_set_num_args(kern, n);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	return kern;

err_out:
	kern_free(kern);

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

cl_int in_clSetKernelArg(cl_kernel  kernel,
			 cl_uint    arg_index,
			 size_t     arg_size,
			 const void *arg_value)
{
	cl_int r;

	if (arg_value == NULL) {
		return CL_INVALID_ARG_VALUE;
	}
	if ((r = kern_is_valid(kernel)) != CL_SUCCESS) {
		return r;
	}

	struct kern_arg arg;

	/* FIXME:
	 * current implementation cannot pass the values that has same size
	 * of pointer. And cannot identify CL types because cl_mem, cl_sampler and
	 * cl_command_queue are pointer so they has same size.
	 * We need to get kernel arguments type from kernel binary.
	 */
	if (arg_size == sizeof(void *)) {
		arg.argtype = __COMM_ARG_MEM;
		arg.size = arg_size;
		arg.val = *(cl_mem *)arg_value;
	} else {
		arg.argtype = __COMM_ARG_VAL;
		arg.size = arg_size;
		arg.val = arg_value;
	}

	r = kern_set_arg(kernel, arg_index, &arg);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

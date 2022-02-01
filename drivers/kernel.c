/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/kernel.h>
#include <drivers/platform.h>
#include <drivers/program.h>

cl_int kern_alloc(cl_program prg, cl_kernel *kern)
{
	cl_kernel t;
	cl_int r;

	if ((r = prg_is_valid(prg)) != CL_SUCCESS) {
		return r;
	}
	if (kern == NULL) {
		return CL_INVALID_VALUE;
	}

	t = calloc(1, sizeof(struct _cl_kernel));
	if (t == NULL) {
		log_err("cannot calloc cl_kernel.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}

	t->dispatch = prg->dispatch;
	t->magic = OPENCL_ICD_MAGIC;
	t->id = plat_get_uniq_id();
	t->prg = prg;

	*kern = t;

	return CL_SUCCESS;
}

cl_int kern_free(cl_kernel kern)
{
	cl_int r;

	if ((r = kern_is_valid(kern)) != CL_SUCCESS) {
		return r;
	}

	if (kern->name != NULL) {
		free(kern->name);
		kern->name = NULL;
	}

	if (kern->args != NULL) {
		free(kern->args);
		kern->args = NULL;
	}

	free(kern);

	return CL_SUCCESS;
}

cl_program kern_get_program(cl_kernel kern)
{
	if (kern_is_valid(kern) != CL_SUCCESS) {
		return NULL;
	}

	return kern->prg;
}

cl_int kern_get_name(cl_kernel kern, char *name, size_t len)
{
	cl_int r;

	if ((r = kern_is_valid(kern)) != CL_SUCCESS) {
		return r;
	}
	if (kern->name == NULL) {
		return CL_INVALID_KERNEL;
	}
	if (name == NULL) {
		return CL_INVALID_VALUE;
	}
	if (len == 0) {
		return CL_SUCCESS;
	}

	strncpy(name, kern->name, len - 1);
	name[len - 1] = '\0';

	return CL_SUCCESS;
}

cl_int kern_set_name(cl_kernel kern, const char *name)
{
	cl_int r;

	if ((r = kern_is_valid(kern)) != CL_SUCCESS) {
		return r;
	}
	if (name == NULL) {
		return CL_INVALID_VALUE;
	}

	if (kern->name != NULL) {
		free(kern->name);
		kern->name = NULL;
	}

	kern->name = strdup(name);

	return CL_SUCCESS;
}

cl_int kern_get_num_args(cl_kernel kern, cl_uint *num)
{
	cl_int r;

	if ((r = kern_is_valid(kern)) != CL_SUCCESS) {
		return r;
	}

	if (num != NULL) {
		*num = kern->num_args;
	}

	return CL_SUCCESS;
}

cl_int kern_set_num_args(cl_kernel kern, cl_uint num)
{
	cl_int r;

	if ((r = kern_is_valid(kern)) != CL_SUCCESS) {
		return r;
	}

	if (kern->args != NULL) {
		free(kern->args);
		kern->args = NULL;
	}

	kern->args = calloc(num, sizeof(struct kern_arg));
	if (kern->args == NULL) {
		log_err("Failed to alloc kern_arg.\n");
		return CL_OUT_OF_HOST_MEMORY;
	}
	kern->num_args = num;

	return CL_SUCCESS;
}

cl_int kern_get_arg(cl_kernel kern, cl_uint i, struct kern_arg *arg)
{
	cl_int r;

	if ((r = kern_is_valid(kern)) != CL_SUCCESS) {
		return r;
	}
	if (kern->num_args <= i) {
		return CL_INVALID_VALUE;
	}
	if (kern->args == NULL) {
		return CL_INVALID_KERNEL;
	}

	if (arg) {
		*arg = kern->args[i];
	}

	return CL_SUCCESS;
}

cl_int kern_set_arg(cl_kernel kern, cl_uint i, const struct kern_arg *arg)
{
	cl_int r;

	if ((r = kern_is_valid(kern)) != CL_SUCCESS) {
		return r;
	}
	if (arg == NULL || kern->num_args <= i) {
		return CL_INVALID_VALUE;
	}

	kern->args[i] = *arg;
	kern->args[i].index = i;

	return CL_SUCCESS;
}

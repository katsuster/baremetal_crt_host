/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>

static struct _cl_device_id *in_devices = NULL;
static int in_num_devices;

cl_int device_probe(void)
{
	if (in_devices != NULL) {
		return CL_SUCCESS;
	}

	in_num_devices = 1;

	in_devices = calloc(in_num_devices, sizeof(struct _cl_device_id));
	if (in_devices == NULL) {
		perror("calloc(device_probe)");
		return CL_OUT_OF_HOST_MEMORY;
	}

	for (int i = 0; i < in_num_devices; i++) {
		in_devices[i].magic = OPENCL_ICD_MAGIC;
	}

	return CL_SUCCESS;
}

cl_int in_clGetDeviceIDs(cl_platform_id platform,
			 cl_device_type device_type,
			 cl_uint        num_entries,
			 cl_device_id   *devices,
			 cl_uint        *num_devices)
{
	cl_int r;

	if ((r = platform_is_valid(platform)) != CL_SUCCESS) {
		return r;
	}

	if (devices != NULL && num_entries == 0) {
		return CL_INVALID_VALUE;
	}
	if (devices == NULL && num_devices == NULL) {
		return CL_INVALID_VALUE;
	}

	r = device_probe();
	if (r != CL_SUCCESS) {
		return r;
	}

	if (devices != NULL) {
		*devices = in_devices;
	}
	if (num_devices != NULL) {
		*num_devices = in_num_devices;
	}

	return CL_DEVICE_NOT_FOUND;
}

cl_int in_clGetDeviceInfo(cl_device_id   device,
			  cl_device_info param_name,
			  size_t         param_value_size,
			  void           *param_value,
			  size_t         *param_value_size_ret)
{
	switch (param_name) {
	default:
		return CL_INVALID_VALUE;
	}

	return CL_INVALID_DEVICE;
}

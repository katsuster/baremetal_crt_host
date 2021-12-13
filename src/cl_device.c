/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/device.h>

cl_int in_clGetDeviceIDs(cl_platform_id platform,
			 cl_device_type device_type,
			 cl_uint        num_entries,
			 cl_device_id   *devices,
			 cl_uint        *num_devices)
{
	cl_uint num;
	cl_int r;

	if ((r = plat_is_valid(platform)) != CL_SUCCESS) {
		return r;
	}

	if (devices != NULL && num_entries == 0) {
		return CL_INVALID_VALUE;
	}
	if (devices == NULL && num_devices == NULL) {
		return CL_INVALID_VALUE;
	}

	r = dev_init(platform);
	if (r != CL_SUCCESS) {
		return r;
	}

	num = num_entries;
	r = dev_get_devices(devices, device_type, &num);
	if (r != CL_SUCCESS) {
		return r;
	}

	if (num_devices != NULL) {
		*num_devices = num;
	}

	return CL_SUCCESS;
}

cl_int in_clGetDeviceInfo(cl_device_id   device,
			  cl_device_info param_name,
			  size_t         param_value_size,
			  void           *param_value,
			  size_t         *param_value_size_ret)
{
	cl_int r;

	if ((r = dev_is_valid(device)) != CL_SUCCESS) {
		return r;
	}

	switch (param_name) {
	default:
		return CL_INVALID_VALUE;
	}

	if (param_value) {
		*(int *)param_value = 0;
	}
	if (param_value_size_ret) {
		*param_value_size_ret = 4;
	}

	return CL_INVALID_DEVICE;
}

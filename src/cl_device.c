/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include "in_cl.h"

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

	return CL_DEVICE_NOT_FOUND;
}

cl_int in_clGetDeviceInfo(cl_device_id   device,
			  cl_device_info param_name,
			  size_t         param_value_size,
			  void           *param_value,
			  size_t         *param_value_size_ret)
{
	return CL_DEVICE_NOT_FOUND;
}

/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include "in_cl.h"

cl_context in_clCreateContext(const cl_context_properties *properties,
			      cl_uint            num_devices,
			      const cl_device_id *devices,
			      void (CL_CALLBACK  *pfn_notify)(const char *errinfo,
							      const void *private_info,
							      size_t     cb,
							      void       *user_data),
			      void               *user_data,
			      cl_int             *errcode_ret)
{
	return NULL;
}

cl_context in_clCreateContextFromType(const cl_context_properties *properties,
				      cl_device_type      device_type,
				      void (CL_CALLBACK   *pfn_notify)(const char *errinfo,
								       const void *private_info,
								       size_t     cb,
								       void       *user_data),
				      void                *user_data,
				      cl_int              *errcode_ret)
{
	return NULL;
}

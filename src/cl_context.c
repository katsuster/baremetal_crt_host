/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/context.h>

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
	cl_context ctx;
	cl_int r = CL_SUCCESS;

	if (num_devices == 0 || devices == NULL) {
		r = CL_INVALID_VALUE;
		goto err_out;
	}
	if (pfn_notify != NULL && user_data == NULL) {
		r = CL_INVALID_VALUE;
		goto err_out;
	}
	for (cl_uint i = 0; i < num_devices; i++) {
		cl_device_id dev = devices[i];

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

	cl_device_id dev = devices[0];

	r = ctx_alloc(dev, &ctx);
	if (r != CL_SUCCESS) {
		goto err_out;
	}

	return ctx;

err_out:
	if (errcode_ret != NULL) {
		*errcode_ret = r;
	}

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

cl_int in_clRetainContext(cl_context context)
{
	return CL_INVALID_CONTEXT;
}

cl_int in_clReleaseContext(cl_context context)
{
	cl_int r;

	if ((r = ctx_is_valid(context)) != CL_SUCCESS) {
		return r;
	}

	r = ctx_free(context);
	if (r != CL_SUCCESS) {
		return r;
	}

	return CL_SUCCESS;
}

cl_int in_clGetContextInfo(cl_context      context,
			   cl_context_info param_name,
			   size_t          param_value_size,
			   void            *param_value,
			   size_t          *param_value_size_ret)
{
	return CL_INVALID_CONTEXT;
}

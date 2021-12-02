/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include "use_cl.h"
#include "in_cl.h"

cl_int in_clGetPlatformIDs(cl_uint        num_entries,
			   cl_platform_id *platforms,
			   cl_uint        *num_platforms);

cl_int clGetPlatformIDs(cl_uint num_entries,
			cl_platform_id *platforms,
			cl_uint *num_platforms)
{
	return in_clGetPlatformIDs(num_entries, platforms, num_platforms);
}

cl_int clIcdGetPlatformIDsKHR(cl_uint        num_entries,
			      cl_platform_id *platforms,
			      cl_uint        *num_platforms)
{
	return in_clGetPlatformIDs(num_entries, platforms, num_platforms);
}

cl_int clGetPlatformInfo(cl_platform_id   platform,
			 cl_platform_info param_name,
			 size_t           param_value_size,
			 void             *param_value,
			 size_t           *param_value_size_ret)
{
	return in_clGetPlatformInfo(platform, param_name, param_value_size, param_value, param_value_size_ret);
}

static const struct func_name_table {
	const char *func_name;
	void *func;
} table[] = {
#ifdef OPENCL_ICD_ENABLE
	{"clIcdGetPlatformIDsKHR", clIcdGetPlatformIDsKHR},
#endif
	{NULL, NULL},
};

void *clGetExtensionFunctionAddress(const char *func_name)
{
	if (func_name == NULL) {
		return NULL;
	}

	for (int i = 0; table[i].func_name; i++) {
		if (strcmp(func_name, table[i].func_name) == 0) {
			return table[i].func;
		}
	}

	return NULL;
}

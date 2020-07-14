#include <string.h>
#include <stdio.h>

#include "CL/cl.h"
#include "CL/cl_ext.h"

cl_int ext_clGetPlatformInfo(cl_platform_id platform,
			     cl_platform_info param_name,
			     size_t param_value_size,
			     void *param_value,
			     size_t *param_value_size_ret);
cl_int ext_clIcdGetPlatformIDsKHR(cl_uint num_entries,
				  cl_platform_id *platforms,
				  cl_uint *num_platforms);

struct func_name_table {
	const char *func_name;
	void *func;
} table[] = {
	{"clIcdGetPlatformIDsKHR", ext_clIcdGetPlatformIDsKHR},
	{"clGetPlatformInfo", ext_clGetPlatformInfo},
	{NULL, NULL},
};

void *clGetExtensionFunctionAddress(const char *func_name)
{
	if (func_name == NULL)
		return NULL;

	for (int i = 0; table[i].func_name; i++) {
		if (strcmp(func_name, table[i].func_name) == 0)
			return table[i].func;
	}

	return NULL;
}

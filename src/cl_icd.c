#include <stdio.h>
#include <string.h>

#include "use_cl.h"
#include "in_cl.h"

cl_int in_clGetPlatformIDs(cl_uint        num_entries,
			   cl_platform_id *platforms,
			   cl_uint        *num_platforms);

cl_int clIcdGetPlatformIDsKHR(cl_uint        num_entries,
			      cl_platform_id *platforms,
			      cl_uint        *num_platforms)
{
	return in_clGetPlatformIDs(num_entries, platforms, num_platforms);
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
	if (func_name == NULL)
		return NULL;

	for (int i = 0; table[i].func_name; i++) {
		if (strcmp(func_name, table[i].func_name) == 0)
			return table[i].func;
	}

	return NULL;
}

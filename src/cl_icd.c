#include <string.h>

#include "CL/cl.h"
#include "CL/cl_ext.h"
#include "CL/cl_icd.h"

struct _cl_icd_dispatch disp = {
	clGetPlatformIDs,
	clGetPlatformInfo,
	clGetDeviceIDs,
	clGetDeviceInfo,
	NULL,
};

struct _cl_platform_id {
	struct _cl_icd_dispatch *dispatch;
} plat[] = {
	{&disp},
};

cl_int ext_clIcdGetPlatformIDsKHR(cl_uint num_entries,
				  cl_platform_id *platforms,
				  cl_uint *num_platforms)
{
	if (num_entries == 0 && platforms != NULL)
		return CL_INVALID_VALUE;
	if (num_entries > 0 && platforms == NULL)
		return CL_INVALID_VALUE;

	if (platforms != NULL)
		platforms[0] = &plat[0];

	if (num_platforms != NULL)
		*num_platforms = 1;

	return CL_SUCCESS;
}

cl_int ext_clGetPlatformInfo(cl_platform_id platform,
			     cl_platform_info param_name,
			     size_t param_value_size,
			     void *param_value,
			     size_t *param_value_size_ret)
{
printf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\n");
	return CL_INVALID_VALUE;
}

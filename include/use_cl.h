#ifndef OPENCL_ICD_USE_CL
#define OPENCL_ICD_USE_CL

/* temp config */
#define OPENCL_ICD_ENABLE    1

/* for clGetPlatformIDs */
#define OPENCL_ICD_MAGIC     0xdeadbeaf

/* for clGetPlatformInfo */
#define OPENCL_ICD_PLATFORM_PROFILE           "FULL_PROFILE"
#define OPENCL_ICD_PLATFORM_VERSION           "OpenCL 2.2"
#define OPENCL_ICD_PLATFORM_NAME              "NAME of TEST_OPENCL_ICD"
#define OPENCL_ICD_PLATFORM_VENDOR            "VENDOR of TEST_OPENCL_ICD"
#define OPENCL_ICD_PLATFORM_EXTENSIONS        "cl_khr_icd"
#define OPENCL_ICD_PLATFORM_ICD_SUFFIX_KHR    "TEST_OPENCL_ICD"

/* for OpenCL headers */
#define CL_TARGET_OPENCL_VERSION 220
#define CL_USE_DEPRECATED_OPENCL_1_0_APIS
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define CL_USE_DEPRECATED_OPENCL_2_1_APIS

#include "CL/cl.h"
#include "CL/cl_icd.h"

#endif /* OPENCL_ICD_USE_CL */

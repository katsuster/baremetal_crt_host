/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>

static struct _cl_icd_dispatch disp = {
	/* OpenCL 1.0 */
	in_clGetPlatformIDs,
	in_clGetPlatformInfo,
	in_clGetDeviceIDs,
	in_clGetDeviceInfo,
	in_clCreateContext,
	in_clCreateContextFromType,
	in_clRetainContext,
	in_clReleaseContext,
	in_clGetContextInfo,
	NULL, //in_clCreateCommandQueue,
	NULL, //in_clRetainCommandQueue,
	NULL, //in_clReleaseCommandQueue,
	NULL, //in_clGetCommandQueueInfo,
	NULL, //in_clSetCommandQueueProperty,
	NULL, //in_clCreateBuffer,
	NULL, //in_clCreateImage2D,
	NULL, //in_clCreateImage3D,
	NULL, //in_clRetainMemObject,
	NULL, //in_clReleaseMemObject,
	NULL, //in_clGetSupportedImageFormats,
	NULL, //in_clGetMemObjectInfo,
	NULL, //in_clGetImageInfo,
	NULL, //in_clCreateSampler,
	NULL, //in_clRetainSampler,
	NULL, //in_clReleaseSampler,
	NULL, //in_clGetSamplerInfo,
	NULL, //in_clCreateProgramWithSource,
	NULL, //in_clCreateProgramWithBinary,
	NULL, //in_clRetainProgram,
	NULL, //in_clReleaseProgram,
	NULL, //in_clBuildProgram,
	NULL, //in_clUnloadCompiler,
	NULL, //in_clGetProgramInfo,
	NULL, //in_clGetProgramBuildInfo,
	NULL, //in_clCreateKernel,
	NULL, //in_clCreateKernelsInProgram,
	NULL, //in_clRetainKernel,
	NULL, //in_clReleaseKernel,
	NULL, //in_clSetKernelArg,
	NULL, //in_clGetKernelInfo,
	NULL, //in_clGetKernelWorkGroupInfo,
	NULL, //in_clWaitForEvents,
	NULL, //in_clGetEventInfo,
	NULL, //in_clRetainEvent,
	NULL, //in_clReleaseEvent,
	NULL, //in_clGetEventProfilingInfo,
	NULL, //in_clFlush,
	NULL, //in_clFinish,
	NULL, //in_clEnqueueReadBuffer,
	NULL, //in_clEnqueueWriteBuffer,
	NULL, //in_clEnqueueCopyBuffer,
	NULL, //in_clEnqueueReadImage,
	NULL, //in_clEnqueueWriteImage,
	NULL, //in_clEnqueueCopyImage,
	NULL, //in_clEnqueueCopyImageToBuffer,
	NULL, //in_clEnqueueCopyBufferToImage,
	NULL, //in_clEnqueueMapBuffer,
	NULL, //in_clEnqueueMapImage,
	NULL, //in_clEnqueueUnmapMemObject,
	NULL, //in_clEnqueueNDRangeKernel,
	NULL, //in_clEnqueueTask,
	NULL, //in_clEnqueueNativeKernel,
	NULL, //in_clEnqueueMarker,
	NULL, //in_clEnqueueWaitForEvents,
	NULL, //in_clEnqueueBarrier,
	NULL, //in_clGetExtensionFunctionAddress,
	NULL, //in_clCreateFromGLBuffer,
	NULL, //in_clCreateFromGLTexture2D,
	NULL, //in_clCreateFromGLTexture3D,
	NULL, //in_clCreateFromGLRenderbuffer,
	NULL, //in_clGetGLObjectInfo,
	NULL, //in_clGetGLTextureInfo,
	NULL, //in_clEnqueueAcquireGLObjects,
	NULL, //in_clEnqueueReleaseGLObjects,
	NULL, //in_clGetGLContextInfoKHR,

	/* cl_khr_d3d10_sharing */
	NULL, //in_clGetDeviceIDsFromD3D10KHR,
	NULL, //in_clCreateFromD3D10BufferKHR,
	NULL, //in_clCreateFromD3D10Texture2DKHR,
	NULL, //in_clCreateFromD3D10Texture3DKHR,
	NULL, //in_clEnqueueAcquireD3D10ObjectsKHR,
	NULL, //in_clEnqueueReleaseD3D10ObjectsKHR,

	/* OpenCL 1.1 */
	NULL, //in_clSetEventCallback,
	NULL, //in_clCreateSubBuffer,
	NULL, //in_clSetMemObjectDestructorCallback,
	NULL, //in_clCreateUserEvent,
	NULL, //in_clSetUserEventStatus,
	NULL, //in_clEnqueueReadBufferRect,
	NULL, //in_clEnqueueWriteBufferRect,
	NULL, //in_clEnqueueCopyBufferRect,

	/* cl_ext_device_fission */
	NULL, //in_clCreateSubDevicesEXT,
	NULL, //in_clRetainDeviceEXT,
	NULL, //in_clReleaseDeviceEXT,

	/* cl_khr_gl_event */
	NULL, //in_clCreateEventFromGLsyncKHR,

	/* OpenCL 1.2 */
	NULL, //in_clCreateSubDevices,
	NULL, //in_clRetainDevice,
	NULL, //in_clReleaseDevice,
	NULL, //in_clCreateImage,
	NULL, //in_clCreateProgramWithBuiltInKernels,
	NULL, //in_clCompileProgram,
	NULL, //in_clLinkProgram,
	NULL, //in_clUnloadPlatformCompiler,
	NULL, //in_clGetKernelArgInfo,
	NULL, //in_clEnqueueFillBuffer,
	NULL, //in_clEnqueueFillImage,
	NULL, //in_clEnqueueMigrateMemObjects,
	NULL, //in_clEnqueueMarkerWithWaitList,
	NULL, //in_clEnqueueBarrierWithWaitList,
	NULL, //in_clGetExtensionFunctionAddressForPlatform,
	NULL, //in_clCreateFromGLTexture,

	/* cl_khr_d3d11_sharing */
	NULL, //in_clGetDeviceIDsFromD3D11KHR,
	NULL, //in_clCreateFromD3D11BufferKHR,
	NULL, //in_clCreateFromD3D11Texture2DKHR,
	NULL, //in_clCreateFromD3D11Texture3DKHR,
	NULL, //in_clCreateFromDX9MediaSurfaceKHR,
	NULL, //in_clEnqueueAcquireD3D11ObjectsKHR,
	NULL, //in_clEnqueueReleaseD3D11ObjectsKHR,

	/* cl_khr_dx9_media_sharing */
	NULL, //in_clGetDeviceIDsFromDX9MediaAdapterKHR,
	NULL, //in_clEnqueueAcquireDX9MediaSurfacesKHR,
	NULL, //in_clEnqueueReleaseDX9MediaSurfacesKHR,

	/* cl_khr_egl_image */
	NULL, //in_clCreateFromEGLImageKHR,
	NULL, //in_clEnqueueAcquireEGLObjectsKHR,
	NULL, //in_clEnqueueReleaseEGLObjectsKHR,

	/* cl_khr_egl_event */
	NULL, //in_clCreateEventFromEGLSyncKHR,

	/* OpenCL 2.0 */
	NULL, //in_clCreateCommandQueueWithProperties,
	NULL, //in_clCreatePipe,
	NULL, //in_clGetPipeInfo,
	NULL, //in_clSVMAlloc,
	NULL, //in_clSVMFree,
	NULL, //in_clEnqueueSVMFree,
	NULL, //in_clEnqueueSVMMemcpy,
	NULL, //in_clEnqueueSVMMemFill,
	NULL, //in_clEnqueueSVMMap,
	NULL, //in_clEnqueueSVMUnmap,
	NULL, //in_clCreateSamplerWithProperties,
	NULL, //in_clSetKernelArgSVMPointer,
	NULL, //in_clSetKernelExecInfo,

	/* cl_khr_sub_groups */
	NULL, //in_clGetKernelSubGroupInfoKHR,

	/* OpenCL 2.1 */
	NULL, //in_clCloneKernel,
	NULL, //in_clCreateProgramWithIL,
	NULL, //in_clEnqueueSVMMigrateMem,
	NULL, //in_clGetDeviceAndHostTimer,
	NULL, //in_clGetHostTimer,
	NULL, //in_clGetKernelSubGroupInfo,
	NULL, //in_clSetDefaultDeviceCommandQueue,

	/* OpenCL 2.2 */
	NULL, //in_clSetProgramReleaseCallback,
	NULL, //in_clSetProgramSpecializationConstant,

	/* OpenCL 3.0 */
	NULL, //in_clCreateBufferWithProperties,
	NULL, //in_clCreateImageWithProperties,
};

struct _cl_platform_id plat[] = {
	{ &disp, OPENCL_ICD_MAGIC, },
};

cl_int in_clGetPlatformIDs(cl_uint         num_entries,
			   cl_platform_id  *platforms,
			   cl_uint         *num_platforms)
{
	if (num_entries == 0 && platforms != NULL) {
		return CL_INVALID_VALUE;
	}
	if (num_entries > 0 && platforms == NULL) {
		return CL_INVALID_VALUE;
	}

	if (platforms != NULL) {
		platforms[0] = &plat[0];
	}

	if (num_platforms != NULL) {
		*num_platforms = 1;
	}

	return CL_SUCCESS;
}

cl_int in_clGetPlatformInfo(cl_platform_id   platform,
			    cl_platform_info param_name,
			    size_t           param_value_size,
			    void             *param_value,
			    size_t           *param_value_size_ret)
{
	void *val = "";
	size_t siz = 0;
	cl_int r;

	if ((r = plat_is_valid(platform)) != CL_SUCCESS) {
		return r;
	}

	switch (param_name) {
	case CL_PLATFORM_PROFILE:
		val = OPENCL_ICD_PLATFORM_PROFILE;
		siz = sizeof(OPENCL_ICD_PLATFORM_PROFILE);
		break;
	case CL_PLATFORM_VERSION:
		val = OPENCL_ICD_PLATFORM_VERSION;
		siz = sizeof(OPENCL_ICD_PLATFORM_VERSION);
		break;
	case CL_PLATFORM_NAME:
		val = OPENCL_ICD_PLATFORM_NAME;
		siz = sizeof(OPENCL_ICD_PLATFORM_NAME);
		break;
	case CL_PLATFORM_VENDOR:
		val = OPENCL_ICD_PLATFORM_VENDOR;
		siz = sizeof(OPENCL_ICD_PLATFORM_VENDOR);
		break;
	case CL_PLATFORM_EXTENSIONS:
		val = OPENCL_ICD_PLATFORM_EXTENSIONS;
		siz = sizeof(OPENCL_ICD_PLATFORM_EXTENSIONS);
		break;
	case CL_PLATFORM_ICD_SUFFIX_KHR:
		val = OPENCL_ICD_PLATFORM_ICD_SUFFIX_KHR;
		siz = sizeof(OPENCL_ICD_PLATFORM_ICD_SUFFIX_KHR);
		break;
	default:
		return CL_INVALID_VALUE;
	}

	if (param_value != NULL) {
		if (param_value_size < siz) {
			return CL_INVALID_VALUE;
		}

		memcpy(param_value, val, siz);
	}

	if (param_value_size_ret != NULL) {
		*param_value_size_ret = siz;
	}

	return CL_SUCCESS;
}

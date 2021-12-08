/*******************************************************************************
 * Copyright (c) 2008-2020 The Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef BAREMETAL_CRT_HOST_IN_CL_H
#define BAREMETAL_CRT_HOST_IN_CL_H

#include "config_cl.h"
#include "type_cl.h"

/* Platform API */
extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetPlatformIDs(cl_uint          num_entries,
                    cl_platform_id * platforms,
                    cl_uint *        num_platforms) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetPlatformInfo(cl_platform_id   platform,
                     cl_platform_info param_name,
                     size_t           param_value_size,
                     void *           param_value,
                     size_t *         param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

/* Device APIs */
extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetDeviceIDs(cl_platform_id   platform,
                  cl_device_type   device_type,
                  cl_uint          num_entries,
                  cl_device_id *   devices,
                  cl_uint *        num_devices) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetDeviceInfo(cl_device_id    device,
                   cl_device_info  param_name,
                   size_t          param_value_size,
                   void *          param_value,
                   size_t *        param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clCreateSubDevices(cl_device_id                         in_device,
                      const cl_device_partition_property * properties,
                      cl_uint                              num_devices,
                      cl_device_id *                       out_devices,
                      cl_uint *                            num_devices_ret) CL_API_SUFFIX__VERSION_1_2;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clRetainDevice(cl_device_id device) CL_API_SUFFIX__VERSION_1_2;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clReleaseDevice(cl_device_id device) CL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef CL_VERSION_2_1

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetDefaultDeviceCommandQueue(cl_context           context,
                                  cl_device_id         device,
                                  cl_command_queue     command_queue) CL_API_SUFFIX__VERSION_2_1;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetDeviceAndHostTimer(cl_device_id    device,
                           cl_ulong*       device_timestamp,
                           cl_ulong*       host_timestamp) CL_API_SUFFIX__VERSION_2_1;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetHostTimer(cl_device_id device,
                  cl_ulong *   host_timestamp) CL_API_SUFFIX__VERSION_2_1;

#endif

/* Context APIs */
extern CL_API_ENTRY cl_context CL_API_CALL
in_clCreateContext(const cl_context_properties * properties,
                   cl_uint              num_devices,
                   const cl_device_id * devices,
                   void (CL_CALLBACK * pfn_notify)(const char * errinfo,
                                                   const void * private_info,
                                                   size_t       cb,
                                                   void *       user_data),
                   void *               user_data,
                   cl_int *             errcode_ret) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_context CL_API_CALL
in_clCreateContextFromType(const cl_context_properties * properties,
                           cl_device_type      device_type,
                           void (CL_CALLBACK * pfn_notify)(const char * errinfo,
                                                           const void * private_info,
                                                           size_t       cb,
                                                           void *       user_data),
                           void *              user_data,
                           cl_int *            errcode_ret) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clRetainContext(cl_context context) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clReleaseContext(cl_context context) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetContextInfo(cl_context         context,
                    cl_context_info    param_name,
                    size_t             param_value_size,
                    void *             param_value,
                    size_t *           param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

/* Command Queue APIs */

#ifdef CL_VERSION_2_0

extern CL_API_ENTRY cl_command_queue CL_API_CALL
in_clCreateCommandQueueWithProperties(cl_context               context,
                                      cl_device_id             device,
                                      const cl_queue_properties *    properties,
                                      cl_int *                 errcode_ret) CL_API_SUFFIX__VERSION_2_0;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clRetainCommandQueue(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clReleaseCommandQueue(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetCommandQueueInfo(cl_command_queue      command_queue,
                         cl_command_queue_info param_name,
                         size_t                param_value_size,
                         void *                param_value,
                         size_t *              param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

/* Memory Object APIs */
extern CL_API_ENTRY cl_mem CL_API_CALL
in_clCreateBuffer(cl_context   context,
                  cl_mem_flags flags,
                  size_t       size,
                  void *       host_ptr,
                  cl_int *     errcode_ret) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_1

extern CL_API_ENTRY cl_mem CL_API_CALL
in_clCreateSubBuffer(cl_mem                   buffer,
                     cl_mem_flags             flags,
                     cl_buffer_create_type    buffer_create_type,
                     const void *             buffer_create_info,
                     cl_int *                 errcode_ret) CL_API_SUFFIX__VERSION_1_1;

#endif

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_mem CL_API_CALL
in_clCreateImage(cl_context              context,
                 cl_mem_flags            flags,
                 const cl_image_format * image_format,
                 const cl_image_desc *   image_desc,
                 void *                  host_ptr,
                 cl_int *                errcode_ret) CL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef CL_VERSION_2_0

extern CL_API_ENTRY cl_mem CL_API_CALL
in_clCreatePipe(cl_context                 context,
                cl_mem_flags               flags,
                cl_uint                    pipe_packet_size,
                cl_uint                    pipe_max_packets,
                const cl_pipe_properties * properties,
                cl_int *                   errcode_ret) CL_API_SUFFIX__VERSION_2_0;

#endif

#ifdef CL_VERSION_3_0

extern CL_API_ENTRY cl_mem CL_API_CALL
in_clCreateBufferWithProperties(cl_context                context,
                                const cl_mem_properties * properties,
                                cl_mem_flags              flags,
                                size_t                    size,
                                void *                    host_ptr,
                                cl_int *                  errcode_ret) CL_API_SUFFIX__VERSION_3_0;

extern CL_API_ENTRY cl_mem CL_API_CALL
in_clCreateImageWithProperties(cl_context                context,
                               const cl_mem_properties * properties,
                               cl_mem_flags              flags,
                               const cl_image_format *   image_format,
                               const cl_image_desc *     image_desc,
                               void *                    host_ptr,
                               cl_int *                  errcode_ret) CL_API_SUFFIX__VERSION_3_0;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clRetainMemObject(cl_mem memobj) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clReleaseMemObject(cl_mem memobj) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetSupportedImageFormats(cl_context           context,
                              cl_mem_flags         flags,
                              cl_mem_object_type   image_type,
                              cl_uint              num_entries,
                              cl_image_format *    image_formats,
                              cl_uint *            num_image_formats) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetMemObjectInfo(cl_mem           memobj,
                      cl_mem_info      param_name,
                      size_t           param_value_size,
                      void *           param_value,
                      size_t *         param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetImageInfo(cl_mem           image,
                  cl_image_info    param_name,
                  size_t           param_value_size,
                  void *           param_value,
                  size_t *         param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_2_0

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetPipeInfo(cl_mem           pipe,
                 cl_pipe_info     param_name,
                 size_t           param_value_size,
                 void *           param_value,
                 size_t *         param_value_size_ret) CL_API_SUFFIX__VERSION_2_0;

#endif

#ifdef CL_VERSION_1_1

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetMemObjectDestructorCallback(cl_mem memobj,
                                    void (CL_CALLBACK * pfn_notify)(cl_mem memobj,
                                                                    void * user_data),
                                    void * user_data) CL_API_SUFFIX__VERSION_1_1;

#endif

/* SVM Allocation APIs */

#ifdef CL_VERSION_2_0

extern CL_API_ENTRY void * CL_API_CALL
in_clSVMAlloc(cl_context       context,
              cl_svm_mem_flags flags,
              size_t           size,
              cl_uint          alignment) CL_API_SUFFIX__VERSION_2_0;

extern CL_API_ENTRY void CL_API_CALL
in_clSVMFree(cl_context        context,
             void *            svm_pointer) CL_API_SUFFIX__VERSION_2_0;

#endif

/* Sampler APIs */

#ifdef CL_VERSION_2_0

extern CL_API_ENTRY cl_sampler CL_API_CALL
in_clCreateSamplerWithProperties(cl_context                     context,
                                 const cl_sampler_properties *  sampler_properties,
                                 cl_int *                       errcode_ret) CL_API_SUFFIX__VERSION_2_0;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clRetainSampler(cl_sampler sampler) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clReleaseSampler(cl_sampler sampler) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetSamplerInfo(cl_sampler         sampler,
                    cl_sampler_info    param_name,
                    size_t             param_value_size,
                    void *             param_value,
                    size_t *           param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

/* Program Object APIs */
extern CL_API_ENTRY cl_program CL_API_CALL
in_clCreateProgramWithSource(cl_context        context,
                             cl_uint           count,
                             const char **     strings,
                             const size_t *    lengths,
                             cl_int *          errcode_ret) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_program CL_API_CALL
in_clCreateProgramWithBinary(cl_context                     context,
                             cl_uint                        num_devices,
                             const cl_device_id *           device_list,
                             const size_t *                 lengths,
                             const unsigned char **         binaries,
                             cl_int *                       binary_status,
                             cl_int *                       errcode_ret) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_program CL_API_CALL
in_clCreateProgramWithBuiltInKernels(cl_context            context,
                                     cl_uint               num_devices,
                                     const cl_device_id *  device_list,
                                     const char *          kernel_names,
                                     cl_int *              errcode_ret) CL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef CL_VERSION_2_1

extern CL_API_ENTRY cl_program CL_API_CALL
in_clCreateProgramWithIL(cl_context    context,
                        const void*    il,
                        size_t         length,
                        cl_int*        errcode_ret) CL_API_SUFFIX__VERSION_2_1;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clRetainProgram(cl_program program) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clReleaseProgram(cl_program program) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clBuildProgram(cl_program           program,
                  cl_uint              num_devices,
                  const cl_device_id * device_list,
                  const char *         options,
                  void (CL_CALLBACK *  pfn_notify)(cl_program program,
                                                   void * user_data),
                  void *               user_data) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clCompileProgram(cl_program           program,
                    cl_uint              num_devices,
                    const cl_device_id * device_list,
                    const char *         options,
                    cl_uint              num_input_headers,
                    const cl_program *   input_headers,
                    const char **        header_include_names,
                    void (CL_CALLBACK *  pfn_notify)(cl_program program,
                                                     void * user_data),
                    void *               user_data) CL_API_SUFFIX__VERSION_1_2;

extern CL_API_ENTRY cl_program CL_API_CALL
in_clLinkProgram(cl_context           context,
                 cl_uint              num_devices,
                 const cl_device_id * device_list,
                 const char *         options,
                 cl_uint              num_input_programs,
                 const cl_program *   input_programs,
                 void (CL_CALLBACK *  pfn_notify)(cl_program program,
                                                  void * user_data),
                 void *               user_data,
                 cl_int *             errcode_ret) CL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef CL_VERSION_2_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetProgramReleaseCallback(cl_program          program,
                               void (CL_CALLBACK * pfn_notify)(cl_program program,
                                                               void * user_data),
                               void *              user_data) CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetProgramSpecializationConstant(cl_program  program,
                                      cl_uint     spec_id,
                                      size_t      spec_size,
                                      const void* spec_value) CL_API_SUFFIX__VERSION_2_2;

#endif

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clUnloadPlatformCompiler(cl_platform_id platform) CL_API_SUFFIX__VERSION_1_2;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetProgramInfo(cl_program         program,
                    cl_program_info    param_name,
                    size_t             param_value_size,
                    void *             param_value,
                    size_t *           param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetProgramBuildInfo(cl_program            program,
                         cl_device_id          device,
                         cl_program_build_info param_name,
                         size_t                param_value_size,
                         void *                param_value,
                         size_t *              param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

/* Kernel Object APIs */
extern CL_API_ENTRY cl_kernel CL_API_CALL
in_clCreateKernel(cl_program      program,
                  const char *    kernel_name,
                  cl_int *        errcode_ret) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clCreateKernelsInProgram(cl_program     program,
                            cl_uint        num_kernels,
                            cl_kernel *    kernels,
                            cl_uint *      num_kernels_ret) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_2_1

extern CL_API_ENTRY cl_kernel CL_API_CALL
in_clCloneKernel(cl_kernel     source_kernel,
                 cl_int*       errcode_ret) CL_API_SUFFIX__VERSION_2_1;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clRetainKernel(cl_kernel    kernel) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clReleaseKernel(cl_kernel   kernel) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetKernelArg(cl_kernel    kernel,
                  cl_uint      arg_index,
                  size_t       arg_size,
                  const void * arg_value) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_2_0

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetKernelArgSVMPointer(cl_kernel    kernel,
                            cl_uint      arg_index,
                            const void * arg_value) CL_API_SUFFIX__VERSION_2_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetKernelExecInfo(cl_kernel            kernel,
                       cl_kernel_exec_info  param_name,
                       size_t               param_value_size,
                       const void *         param_value) CL_API_SUFFIX__VERSION_2_0;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetKernelInfo(cl_kernel       kernel,
                   cl_kernel_info  param_name,
                   size_t          param_value_size,
                   void *          param_value,
                   size_t *        param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetKernelArgInfo(cl_kernel       kernel,
                      cl_uint         arg_indx,
                      cl_kernel_arg_info  param_name,
                      size_t          param_value_size,
                      void *          param_value,
                      size_t *        param_value_size_ret) CL_API_SUFFIX__VERSION_1_2;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetKernelWorkGroupInfo(cl_kernel                  kernel,
                            cl_device_id               device,
                            cl_kernel_work_group_info  param_name,
                            size_t                     param_value_size,
                            void *                     param_value,
                            size_t *                   param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_2_1

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetKernelSubGroupInfo(cl_kernel                   kernel,
                           cl_device_id                device,
                           cl_kernel_sub_group_info    param_name,
                           size_t                      input_value_size,
                           const void*                 input_value,
                           size_t                      param_value_size,
                           void*                       param_value,
                           size_t*                     param_value_size_ret) CL_API_SUFFIX__VERSION_2_1;

#endif

/* Event Object APIs */
extern CL_API_ENTRY cl_int CL_API_CALL
in_clWaitForEvents(cl_uint             num_events,
                   const cl_event *    event_list) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetEventInfo(cl_event         event,
                  cl_event_info    param_name,
                  size_t           param_value_size,
                  void *           param_value,
                  size_t *         param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_1

extern CL_API_ENTRY cl_event CL_API_CALL
in_clCreateUserEvent(cl_context    context,
                     cl_int *      errcode_ret) CL_API_SUFFIX__VERSION_1_1;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clRetainEvent(cl_event event) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clReleaseEvent(cl_event event) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_1

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetUserEventStatus(cl_event   event,
                        cl_int     execution_status) CL_API_SUFFIX__VERSION_1_1;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clSetEventCallback(cl_event    event,
                      cl_int      command_exec_callback_type,
                      void (CL_CALLBACK * pfn_notify)(cl_event event,
                                                      cl_int   event_command_status,
                                                      void *   user_data),
                      void *      user_data) CL_API_SUFFIX__VERSION_1_1;

#endif

/* Profiling APIs */
extern CL_API_ENTRY cl_int CL_API_CALL
in_clGetEventProfilingInfo(cl_event            event,
                           cl_profiling_info   param_name,
                           size_t              param_value_size,
                           void *              param_value,
                           size_t *            param_value_size_ret) CL_API_SUFFIX__VERSION_1_0;

/* Flush and Finish APIs */
extern CL_API_ENTRY cl_int CL_API_CALL
in_clFlush(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clFinish(cl_command_queue command_queue) CL_API_SUFFIX__VERSION_1_0;

/* Enqueued Commands APIs */
extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueReadBuffer(cl_command_queue    command_queue,
                       cl_mem              buffer,
                       cl_bool             blocking_read,
                       size_t              offset,
                       size_t              size,
                       void *              ptr,
                       cl_uint             num_events_in_wait_list,
                       const cl_event *    event_wait_list,
                       cl_event *          event) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_1

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueReadBufferRect(cl_command_queue    command_queue,
                           cl_mem              buffer,
                           cl_bool             blocking_read,
                           const size_t *      buffer_offset,
                           const size_t *      host_offset,
                           const size_t *      region,
                           size_t              buffer_row_pitch,
                           size_t              buffer_slice_pitch,
                           size_t              host_row_pitch,
                           size_t              host_slice_pitch,
                           void *              ptr,
                           cl_uint             num_events_in_wait_list,
                           const cl_event *    event_wait_list,
                           cl_event *          event) CL_API_SUFFIX__VERSION_1_1;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueWriteBuffer(cl_command_queue   command_queue,
                        cl_mem             buffer,
                        cl_bool            blocking_write,
                        size_t             offset,
                        size_t             size,
                        const void *       ptr,
                        cl_uint            num_events_in_wait_list,
                        const cl_event *   event_wait_list,
                        cl_event *         event) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_1

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueWriteBufferRect(cl_command_queue    command_queue,
                            cl_mem              buffer,
                            cl_bool             blocking_write,
                            const size_t *      buffer_offset,
                            const size_t *      host_offset,
                            const size_t *      region,
                            size_t              buffer_row_pitch,
                            size_t              buffer_slice_pitch,
                            size_t              host_row_pitch,
                            size_t              host_slice_pitch,
                            const void *        ptr,
                            cl_uint             num_events_in_wait_list,
                            const cl_event *    event_wait_list,
                            cl_event *          event) CL_API_SUFFIX__VERSION_1_1;

#endif

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueFillBuffer(cl_command_queue   command_queue,
                       cl_mem             buffer,
                       const void *       pattern,
                       size_t             pattern_size,
                       size_t             offset,
                       size_t             size,
                       cl_uint            num_events_in_wait_list,
                       const cl_event *   event_wait_list,
                       cl_event *         event) CL_API_SUFFIX__VERSION_1_2;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueCopyBuffer(cl_command_queue    command_queue,
                       cl_mem              src_buffer,
                       cl_mem              dst_buffer,
                       size_t              src_offset,
                       size_t              dst_offset,
                       size_t              size,
                       cl_uint             num_events_in_wait_list,
                       const cl_event *    event_wait_list,
                       cl_event *          event) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_1

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueCopyBufferRect(cl_command_queue    command_queue,
                           cl_mem              src_buffer,
                           cl_mem              dst_buffer,
                           const size_t *      src_origin,
                           const size_t *      dst_origin,
                           const size_t *      region,
                           size_t              src_row_pitch,
                           size_t              src_slice_pitch,
                           size_t              dst_row_pitch,
                           size_t              dst_slice_pitch,
                           cl_uint             num_events_in_wait_list,
                           const cl_event *    event_wait_list,
                           cl_event *          event) CL_API_SUFFIX__VERSION_1_1;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueReadImage(cl_command_queue     command_queue,
                      cl_mem               image,
                      cl_bool              blocking_read,
                      const size_t *       origin,
                      const size_t *       region,
                      size_t               row_pitch,
                      size_t               slice_pitch,
                      void *               ptr,
                      cl_uint              num_events_in_wait_list,
                      const cl_event *     event_wait_list,
                      cl_event *           event) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueWriteImage(cl_command_queue    command_queue,
                       cl_mem              image,
                       cl_bool             blocking_write,
                       const size_t *      origin,
                       const size_t *      region,
                       size_t              input_row_pitch,
                       size_t              input_slice_pitch,
                       const void *        ptr,
                       cl_uint             num_events_in_wait_list,
                       const cl_event *    event_wait_list,
                       cl_event *          event) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueFillImage(cl_command_queue   command_queue,
                      cl_mem             image,
                      const void *       fill_color,
                      const size_t *     origin,
                      const size_t *     region,
                      cl_uint            num_events_in_wait_list,
                      const cl_event *   event_wait_list,
                      cl_event *         event) CL_API_SUFFIX__VERSION_1_2;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueCopyImage(cl_command_queue     command_queue,
                      cl_mem               src_image,
                      cl_mem               dst_image,
                      const size_t *       src_origin,
                      const size_t *       dst_origin,
                      const size_t *       region,
                      cl_uint              num_events_in_wait_list,
                      const cl_event *     event_wait_list,
                      cl_event *           event) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueCopyImageToBuffer(cl_command_queue command_queue,
                              cl_mem           src_image,
                              cl_mem           dst_buffer,
                              const size_t *   src_origin,
                              const size_t *   region,
                              size_t           dst_offset,
                              cl_uint          num_events_in_wait_list,
                              const cl_event * event_wait_list,
                              cl_event *       event) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueCopyBufferToImage(cl_command_queue command_queue,
                              cl_mem           src_buffer,
                              cl_mem           dst_image,
                              size_t           src_offset,
                              const size_t *   dst_origin,
                              const size_t *   region,
                              cl_uint          num_events_in_wait_list,
                              const cl_event * event_wait_list,
                              cl_event *       event) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY void * CL_API_CALL
in_clEnqueueMapBuffer(cl_command_queue command_queue,
                      cl_mem           buffer,
                      cl_bool          blocking_map,
                      cl_map_flags     map_flags,
                      size_t           offset,
                      size_t           size,
                      cl_uint          num_events_in_wait_list,
                      const cl_event * event_wait_list,
                      cl_event *       event,
                      cl_int *         errcode_ret) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY void * CL_API_CALL
in_clEnqueueMapImage(cl_command_queue  command_queue,
                     cl_mem            image,
                     cl_bool           blocking_map,
                     cl_map_flags      map_flags,
                     const size_t *    origin,
                     const size_t *    region,
                     size_t *          image_row_pitch,
                     size_t *          image_slice_pitch,
                     cl_uint           num_events_in_wait_list,
                     const cl_event *  event_wait_list,
                     cl_event *        event,
                     cl_int *          errcode_ret) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueUnmapMemObject(cl_command_queue command_queue,
                           cl_mem           memobj,
                           void *           mapped_ptr,
                           cl_uint          num_events_in_wait_list,
                           const cl_event * event_wait_list,
                           cl_event *       event) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueMigrateMemObjects(cl_command_queue       command_queue,
                              cl_uint                num_mem_objects,
                              const cl_mem *         mem_objects,
                              cl_mem_migration_flags flags,
                              cl_uint                num_events_in_wait_list,
                              const cl_event *       event_wait_list,
                              cl_event *             event) CL_API_SUFFIX__VERSION_1_2;

#endif

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueNDRangeKernel(cl_command_queue command_queue,
                          cl_kernel        kernel,
                          cl_uint          work_dim,
                          const size_t *   global_work_offset,
                          const size_t *   global_work_size,
                          const size_t *   local_work_size,
                          cl_uint          num_events_in_wait_list,
                          const cl_event * event_wait_list,
                          cl_event *       event) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueNativeKernel(cl_command_queue  command_queue,
                         void (CL_CALLBACK * user_func)(void *),
                         void *            args,
                         size_t            cb_args,
                         cl_uint           num_mem_objects,
                         const cl_mem *    mem_list,
                         const void **     args_mem_loc,
                         cl_uint           num_events_in_wait_list,
                         const cl_event *  event_wait_list,
                         cl_event *        event) CL_API_SUFFIX__VERSION_1_0;

#ifdef CL_VERSION_1_2

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueMarkerWithWaitList(cl_command_queue  command_queue,
                               cl_uint           num_events_in_wait_list,
                               const cl_event *  event_wait_list,
                               cl_event *        event) CL_API_SUFFIX__VERSION_1_2;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueBarrierWithWaitList(cl_command_queue  command_queue,
                                cl_uint           num_events_in_wait_list,
                                const cl_event *  event_wait_list,
                                cl_event *        event) CL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef CL_VERSION_2_0

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueSVMFree(cl_command_queue  command_queue,
                    cl_uint           num_svm_pointers,
                    void *            svm_pointers[],
                    void (CL_CALLBACK * pfn_free_func)(cl_command_queue queue,
                                                       cl_uint          num_svm_pointers,
                                                       void *           svm_pointers[],
                                                       void *           user_data),
                    void *            user_data,
                    cl_uint           num_events_in_wait_list,
                    const cl_event *  event_wait_list,
                    cl_event *        event) CL_API_SUFFIX__VERSION_2_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueSVMMemcpy(cl_command_queue  command_queue,
                      cl_bool           blocking_copy,
                      void *            dst_ptr,
                      const void *      src_ptr,
                      size_t            size,
                      cl_uint           num_events_in_wait_list,
                      const cl_event *  event_wait_list,
                      cl_event *        event) CL_API_SUFFIX__VERSION_2_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueSVMMemFill(cl_command_queue  command_queue,
                       void *            svm_ptr,
                       const void *      pattern,
                       size_t            pattern_size,
                       size_t            size,
                       cl_uint           num_events_in_wait_list,
                       const cl_event *  event_wait_list,
                       cl_event *        event) CL_API_SUFFIX__VERSION_2_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueSVMMap(cl_command_queue  command_queue,
                   cl_bool           blocking_map,
                   cl_map_flags      flags,
                   void *            svm_ptr,
                   size_t            size,
                   cl_uint           num_events_in_wait_list,
                   const cl_event *  event_wait_list,
                   cl_event *        event) CL_API_SUFFIX__VERSION_2_0;

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueSVMUnmap(cl_command_queue  command_queue,
                     void *            svm_ptr,
                     cl_uint           num_events_in_wait_list,
                     const cl_event *  event_wait_list,
                     cl_event *        event) CL_API_SUFFIX__VERSION_2_0;

#endif

#ifdef CL_VERSION_2_1

extern CL_API_ENTRY cl_int CL_API_CALL
in_clEnqueueSVMMigrateMem(cl_command_queue         command_queue,
                          cl_uint                  num_svm_pointers,
                          const void **            svm_pointers,
                          const size_t *           sizes,
                          cl_mem_migration_flags   flags,
                          cl_uint                  num_events_in_wait_list,
                          const cl_event *         event_wait_list,
                          cl_event *               event) CL_API_SUFFIX__VERSION_2_1;

#endif

#ifdef CL_VERSION_1_2

/* Extension function access
 *
 * Returns the extension function address for the given function name,
 * or NULL if a valid function can not be found.  The client must
 * check to make sure the address is not NULL, before using or
 * calling the returned function address.
 */
extern CL_API_ENTRY void * CL_API_CALL
in_clGetExtensionFunctionAddressForPlatform(cl_platform_id platform,
                                            const char *   func_name) CL_API_SUFFIX__VERSION_1_2;

#endif

#ifdef CL_USE_DEPRECATED_OPENCL_1_0_APIS
    /*
     *  WARNING:
     *     This API introduces mutable state into the OpenCL implementation. It has been REMOVED
     *  to better facilitate thread safety.  The 1.0 API is not thread safe. It is not tested by the
     *  OpenCL 1.1 conformance test, and consequently may not work or may not work dependably.
     *  It is likely to be non-performant. Use of this API is not advised. Use at your own risk.
     *
     *  Software developers previously relying on this API are instructed to set the command queue
     *  properties when creating the queue, instead.
     */
    extern CL_API_ENTRY cl_int CL_API_CALL
    clSetCommandQueueProperty(cl_command_queue              command_queue,
                                 cl_command_queue_properties   properties,
                                 cl_bool                       enable,
                                 cl_command_queue_properties * old_properties) CL_EXT_SUFFIX__VERSION_1_0_DEPRECATED;
#endif /* CL_USE_DEPRECATED_OPENCL_1_0_APIS */

/* Deprecated OpenCL 1.1 APIs */
extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_1_DEPRECATED cl_mem CL_API_CALL
in_clCreateImage2D(cl_context              context,
                   cl_mem_flags            flags,
                   const cl_image_format * image_format,
                   size_t                  image_width,
                   size_t                  image_height,
                   size_t                  image_row_pitch,
                   void *                  host_ptr,
                   cl_int *                errcode_ret) CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_1_DEPRECATED cl_mem CL_API_CALL
in_clCreateImage3D(cl_context              context,
                   cl_mem_flags            flags,
                   const cl_image_format * image_format,
                   size_t                  image_width,
                   size_t                  image_height,
                   size_t                  image_depth,
                   size_t                  image_row_pitch,
                   size_t                  image_slice_pitch,
                   void *                  host_ptr,
                   cl_int *                errcode_ret) CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
in_clEnqueueMarker(cl_command_queue    command_queue,
                   cl_event *          event) CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
in_clEnqueueWaitForEvents(cl_command_queue  command_queue,
                           cl_uint          num_events,
                           const cl_event * event_list) CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
in_clEnqueueBarrier(cl_command_queue command_queue) CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
in_clUnloadCompiler(void) CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_1_DEPRECATED void * CL_API_CALL
in_clGetExtensionFunctionAddress(const char * func_name) CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;

/* Deprecated OpenCL 2.0 APIs */
extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_2_DEPRECATED cl_command_queue CL_API_CALL
in_clCreateCommandQueue(cl_context                     context,
                        cl_device_id                   device,
                        cl_command_queue_properties    properties,
                        cl_int *                       errcode_ret) CL_EXT_SUFFIX__VERSION_1_2_DEPRECATED;

extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_2_DEPRECATED cl_sampler CL_API_CALL
in_clCreateSampler(cl_context          context,
                   cl_bool             normalized_coords,
                   cl_addressing_mode  addressing_mode,
                   cl_filter_mode      filter_mode,
                   cl_int *            errcode_ret) CL_EXT_SUFFIX__VERSION_1_2_DEPRECATED;

extern CL_API_ENTRY CL_EXT_PREFIX__VERSION_1_2_DEPRECATED cl_int CL_API_CALL
in_clEnqueueTask(cl_command_queue  command_queue,
                 cl_kernel         kernel,
                 cl_uint           num_events_in_wait_list,
                 const cl_event *  event_wait_list,
                 cl_event *        event) CL_EXT_SUFFIX__VERSION_1_2_DEPRECATED;

#endif /* BAREMETAL_CRT_HOST_IN_CL_H */

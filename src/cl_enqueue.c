/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/command_queue.h>
#include <drivers/mem.h>

cl_int in_clFlush(cl_command_queue command_queue)
{
	return CL_SUCCESS;
}

cl_int in_clFinish(cl_command_queue command_queue)
{
	return CL_SUCCESS;
}

cl_int in_clEnqueueReadBuffer(cl_command_queue command_queue,
			      cl_mem           buffer,
			      cl_bool          blocking_read,
			      size_t           offset,
			      size_t           size,
			      void             *ptr,
			      cl_uint          num_events_in_wait_list,
			      const cl_event   *event_wait_list,
			      cl_event         *event)
{
	return CL_SUCCESS;
}

cl_int in_clEnqueueReadBufferRect(cl_command_queue command_queue,
				  cl_mem           buffer,
				  cl_bool          blocking_read,
				  const size_t     *buffer_offset,
				  const size_t     *host_offset,
				  const size_t     *region,
				  size_t           buffer_row_pitch,
				  size_t           buffer_slice_pitch,
				  size_t           host_row_pitch,
				  size_t           host_slice_pitch,
				  void             *ptr,
				  cl_uint          num_events_in_wait_list,
				  const cl_event   *event_wait_list,
				  cl_event         *event)
{
	return CL_INVALID_COMMAND_QUEUE;
}

cl_int in_clEnqueueWriteBuffer(cl_command_queue command_queue,
			       cl_mem           buffer,
			       cl_bool          blocking_write,
			       size_t           offset,
			       size_t           size,
			       const void       *ptr,
			       cl_uint          num_events_in_wait_list,
			       const cl_event   *event_wait_list,
			       cl_event         *event)
{
	return CL_SUCCESS;
}

cl_int in_clEnqueueWriteBufferRect(cl_command_queue command_queue,
				   cl_mem           buffer,
				   cl_bool          blocking_write,
				   const size_t     *buffer_offset,
				   const size_t     *host_offset,
				   const size_t     *region,
				   size_t           buffer_row_pitch,
				   size_t           buffer_slice_pitch,
				   size_t           host_row_pitch,
				   size_t           host_slice_pitch,
				   const void       *ptr,
				   cl_uint          num_events_in_wait_list,
				   const cl_event   *event_wait_list,
				   cl_event         *event)
{
	return CL_INVALID_COMMAND_QUEUE;
}

cl_int in_clEnqueueFillBuffer(cl_command_queue command_queue,
			      cl_mem           buffer,
			      const void       *pattern,
			      size_t           pattern_size,
			      size_t           offset,
			      size_t           size,
			      cl_uint          num_events_in_wait_list,
			      const cl_event   *event_wait_list,
			      cl_event         *event)
{
	return CL_INVALID_COMMAND_QUEUE;
}

cl_int in_clEnqueueCopyBuffer(cl_command_queue command_queue,
			      cl_mem           src_buffer,
			      cl_mem           dst_buffer,
			      size_t           src_offset,
			      size_t           dst_offset,
			      size_t           size,
			      cl_uint          num_events_in_wait_list,
			      const cl_event   *event_wait_list,
			      cl_event         *event)
{
	return CL_INVALID_COMMAND_QUEUE;
}

cl_int in_clEnqueueCopyBufferRect(cl_command_queue    command_queue,
				  cl_mem              src_buffer,
				  cl_mem              dst_buffer,
				  const size_t        *src_origin,
				  const size_t        *dst_origin,
				  const size_t        *region,
				  size_t              src_row_pitch,
				  size_t              src_slice_pitch,
				  size_t              dst_row_pitch,
				  size_t              dst_slice_pitch,
				  cl_uint             num_events_in_wait_list,
				  const cl_event      *event_wait_list,
				  cl_event            *event)
{
	return CL_INVALID_COMMAND_QUEUE;
}

void *in_clEnqueueMapBuffer(cl_command_queue command_queue,
			    cl_mem           buffer,
			    cl_bool          blocking_map,
			    cl_map_flags     map_flags,
			    size_t           offset,
			    size_t           size,
			    cl_uint          num_events_in_wait_list,
			    const cl_event   *event_wait_list,
			    cl_event         *event,
			    cl_int           *errcode_ret)
{
	return NULL;
}

cl_int in_clEnqueueUnmapMemObject(cl_command_queue command_queue,
				  cl_mem           memobj,
				  void             *mapped_ptr,
				  cl_uint          num_events_in_wait_list,
				  const cl_event   *event_wait_list,
				  cl_event         *event)
{
	return CL_INVALID_COMMAND_QUEUE;
}

cl_int in_clEnqueueMigrateMemObjects(cl_command_queue       command_queue,
				     cl_uint                num_mem_objects,
				     const cl_mem           *mem_objects,
				     cl_mem_migration_flags flags,
				     cl_uint                num_events_in_wait_list,
				     const cl_event         *event_wait_list,
				     cl_event               *event)
{
	return CL_INVALID_COMMAND_QUEUE;
}

cl_int in_clEnqueueNDRangeKernel(cl_command_queue command_queue,
				 cl_kernel        kernel,
				 cl_uint          work_dim,
				 const size_t     *global_work_offset,
				 const size_t     *global_work_size,
				 const size_t     *local_work_size,
				 cl_uint          num_events_in_wait_list,
				 const cl_event   *event_wait_list,
				 cl_event         *event)
{
	return CL_SUCCESS;
}

cl_int in_clEnqueueNativeKernel(cl_command_queue  command_queue,
				void (CL_CALLBACK *user_func)(void *),
				void              *args,
				size_t            cb_args,
				cl_uint           num_mem_objects,
				const cl_mem      *mem_list,
				const void        **args_mem_loc,
				cl_uint           num_events_in_wait_list,
				const cl_event    *event_wait_list,
				cl_event          *event)
{
	return CL_INVALID_COMMAND_QUEUE;
}

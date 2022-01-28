/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/command_queue.h>
#include <drivers/context.h>
#include <drivers/device.h>
#include <drivers/kernel.h>
#include <drivers/mem.h>
#include <drivers/program.h>

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
	cl_int r;

	if ((r = cmdq_is_valid(command_queue)) != CL_SUCCESS) {
		return r;
	}
	if ((r = mem_is_valid(buffer)) != CL_SUCCESS) {
		return r;
	}

	/* TODO: non-blocking write operation */

	r = mem_read_buffer(buffer, offset, size, ptr);
	if (r != CL_SUCCESS) {
		return r;
	}

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
	cl_int r;

	if ((r = cmdq_is_valid(command_queue)) != CL_SUCCESS) {
		return r;
	}
	if ((r = mem_is_valid(buffer)) != CL_SUCCESS) {
		return r;
	}

	/* TODO: non-blocking write operation */

	r = mem_write_buffer(buffer, offset, size, ptr);
	if (r != CL_SUCCESS) {
		return r;
	}

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
	cl_context ctx = cmdq_get_context(command_queue);
	cl_device_id dev = ctx_get_device(ctx);
	cl_program prg = kern_get_program(kernel);
	struct program_comm comm;
	int num_chunks;
	cl_int r;

	/* Send text/data areas */
	r = prg_get_num_chunks(prg, &num_chunks);
	if (r != CL_SUCCESS) {
		return r;
	}

	for (int i = 0; i < num_chunks; i++) {
		struct program_chunk chunk;

		r = prg_get_chunk(prg, i, &chunk);
		if (r != CL_SUCCESS) {
			return r;
		}

		r = dev_write_mem(dev, chunk.paddr, chunk.buf, chunk.size);
		if (r != CL_SUCCESS) {
			return r;
		}
	}

	/* Send arguments */
	r = prg_get_comm(prg, &comm);
	if (r != CL_SUCCESS) {
		return r;
	}

	cl_uint num_args;

	r = kern_get_num_args(kernel, &num_args);
	if (r != CL_SUCCESS) {
		return r;
	}

	uint64_t paddr = comm.addr;
	struct __comm_area_header h_comm;
	size_t sz;

	h_comm.magic = BAREMETAL_CRT_COMM_MAGIC;
	h_comm.num_args = num_args;

	sz = sizeof(struct __comm_area_header);
	r = dev_write_mem(dev, paddr, (const char *)&h_comm, sz);
	if (r != CL_SUCCESS) {
		return r;
	}
	paddr += sz;

	for (cl_uint i = 0; i < num_args; i++) {
		struct kern_arg arg;
		struct __comm_arg_header h_arg;

		r = kern_get_arg(kernel, i, &arg);
		if (r != CL_SUCCESS) {
			return r;
		}

		h_arg.argtype = 0;
		h_arg.index = i;
		h_arg.size = arg.size;

		sz = sizeof(struct __comm_arg_header);
		if (comm.addr + comm.size <= paddr + sz + arg.size) {
			log_err("kernel arguments exceeds comm area size 0x%" PRIx64 ".\n",
				comm.size);
			return CL_OUT_OF_RESOURCES;
		}

		r = dev_write_mem(dev, paddr, (const char *)&h_arg, sz);
		if (r != CL_SUCCESS) {
			return r;
		}
		paddr += sz;

		r = dev_write_mem(dev, paddr, arg.val, arg.size);
		if (r != CL_SUCCESS) {
			return r;
		}
		paddr += arg.size;
		if (paddr % 4 != 0) {
			paddr = ((paddr / 4) + 1) * 4;
		}
	}

	/* Exec */
	r = dev_reset(dev);
	if (r != CL_SUCCESS) {
		return r;
	}

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

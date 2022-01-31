/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

static cl_int enqueue_arg(cl_device_id dev, const struct kern_arg *arg, const struct program_comm *comm, uint64_t *paddr)
{
	struct __comm_arg_header h_arg;
	const void *ptr;
	size_t size_head, size_buf;
	int need_write = 0;
	cl_int r;

	switch (arg->argtype) {
	case __COMM_ARG_NOTUSED:
		ptr = NULL;
		size_buf = 0;

		break;
	case __COMM_ARG_VAL:
		/* Direct value */
		ptr = arg->val;
		size_buf = arg->size;
		need_write = 1;

		break;
	case __COMM_ARG_MEM:
		/* cl_mem */
		const cl_mem mem = (const cl_mem)arg->val;

		if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
			return r;
		}

		ptr = mem->ptr;
		size_buf = mem->size;

		/*
		 * If mem_can_read is true, it means that the kernel
		 * will read from this buffer. Need to write.
		 */
		if (mem_can_read(mem)) {
			need_write = 1;
		}

		break;
	default:
		log_err("unknown kernel argument type.\n");
		return CL_INVALID_KERNEL;
	}

	h_arg.argtype = arg->argtype;
	h_arg.index = arg->index;
	h_arg.size = size_buf;

	size_head = sizeof(struct __comm_arg_header);
	if (comm->addr + comm->size <= *paddr + size_head + size_buf) {
		log_err("kernel arguments exceeds comm area size 0x%" PRIx64 ".\n",
			comm->size);
		return CL_OUT_OF_RESOURCES;
	}

	r = dev_write_mem(dev, *paddr, (const char *)&h_arg, size_head);
	if (r != CL_SUCCESS) {
		return r;
	}
	*paddr += size_head;

	if (need_write) {
		r = dev_write_mem(dev, *paddr, ptr, size_buf);
		if (r != CL_SUCCESS) {
			return r;
		}
	}
	*paddr += size_buf;

	*paddr = ALIGN_OF(*paddr, 4);

	return CL_SUCCESS;
}

static cl_int dequeue_arg(cl_device_id dev, const struct kern_arg *arg, const struct program_comm *comm, uint64_t *paddr)
{
	void *ptr;
	size_t size_head, size_buf;
	int need_read = 0;
	cl_int r;

	switch (arg->argtype) {
	case __COMM_ARG_NOTUSED:
		ptr = NULL;
		size_buf = 0;

		break;
	case __COMM_ARG_VAL:
		/* Direct value cannot be change by the kernel */
		ptr = NULL;
		size_buf = arg->size;
		need_read = 0;

		break;
	case __COMM_ARG_MEM:
		/* cl_mem */
		const cl_mem mem = (const cl_mem)arg->val;

		if ((r = mem_is_valid(mem)) != CL_SUCCESS) {
			return r;
		}

		ptr = mem->ptr;
		size_buf = mem->size;

		/*
		 * If mem_can_write is true, it means that the kernel
		 * may change this buffer. Need to read back.
		 */
		if (mem_can_write(mem)) {
			need_read = 1;
		}

		break;
	default:
		log_err("unknown kernel argument type.\n");
		return CL_INVALID_KERNEL;
	}

	size_head = sizeof(struct __comm_arg_header);
	if (comm->addr + comm->size <= *paddr + size_head + size_buf) {
		log_err("kernel arguments exceeds comm area size 0x%" PRIx64 ".\n",
			comm->size);
		return CL_OUT_OF_RESOURCES;
	}

	*paddr += size_head;

	if (need_read) {
		r = dev_read_mem(dev, *paddr, ptr, size_buf);
		if (r != CL_SUCCESS) {
			return r;
		}
	}
	*paddr += size_buf;

	*paddr = ALIGN_OF(*paddr, 4);

	return CL_SUCCESS;
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

	/* Get arguments */
	r = prg_get_comm(prg, &comm);
	if (r != CL_SUCCESS) {
		return r;
	}

	cl_uint num_args;

	r = kern_get_num_args(kernel, &num_args);
	if (r != CL_SUCCESS) {
		return r;
	}

	struct __comm_area_header h_comm;
	size_t sz;

	h_comm.magic = BAREMETAL_CRT_COMM_MAGIC;
	h_comm.num_args = num_args;
	h_comm.done = 0;
	h_comm.ret_main = 0;

	sz = sizeof(struct __comm_area_header);
	r = dev_write_mem(dev, comm.addr, (const char *)&h_comm, sz);
	if (r != CL_SUCCESS) {
		return r;
	}

	/* Send arguments */
	uint64_t paddr = comm.addr + sizeof(struct __comm_area_header);
	struct kern_arg arg;

	/* argv[0] is kernel name */
	arg.argtype = __COMM_ARG_VAL;
	arg.index = -1;
	arg.size = strlen(kernel->name) + 1;
	arg.val = kernel->name;

	r = enqueue_arg(dev, &arg, &comm, &paddr);
	if (r != CL_SUCCESS) {
		return r;
	}

	for (cl_uint i = 0; i < num_args; i++) {
		r = kern_get_arg(kernel, i, &arg);
		if (r != CL_SUCCESS) {
			return r;
		}

		r = enqueue_arg(dev, &arg, &comm, &paddr);
		if (r != CL_SUCCESS) {
			return r;
		}
	}

	/* Exec */
	r = dev_reset(dev);
	if (r != CL_SUCCESS) {
		return r;
	}

	r = dev_run(dev);
	if (r != CL_SUCCESS) {
		return r;
	}

	/* Polling 100 seconds */
	int done = 0;

	for (int j = 0; j < 10000; j++) {
		usleep(10000);

		r = dev_stop(dev);
		if (r != CL_SUCCESS) {
			return r;
		}

		sz = sizeof(struct __comm_area_header);
		r = dev_read_mem(dev, comm.addr, (char *)&h_comm, sz);
		if (r != CL_SUCCESS) {
			return r;
		}

		if (h_comm.magic != BAREMETAL_CRT_COMM_MAGIC) {
			return CL_INVALID_DEVICE;
		}
		if (h_comm.done) {
			done = 1;
			break;
		}

		r = dev_run(dev);
		if (r != CL_SUCCESS) {
			return r;
		}
	}
	if (!done) {
		/* Timeout */
		log_err("Device not answered, abort.\n");
		return CL_INVALID_DEVICE;
	}

	/* Recv arguments */
	paddr = comm.addr + sizeof(struct __comm_area_header);

	/* Skip argv[0] (kernel name) */
	arg.argtype = __COMM_ARG_VAL;
	arg.index = -1;
	arg.size = strlen(kernel->name) + 1;
	arg.val = kernel->name;

	r = dequeue_arg(dev, &arg, &comm, &paddr);
	if (r != CL_SUCCESS) {
		return r;
	}

	for (cl_uint i = 0; i < num_args; i++) {
		r = kern_get_arg(kernel, i, &arg);
		if (r != CL_SUCCESS) {
			return r;
		}

		r = dequeue_arg(dev, &arg, &comm, &paddr);
		if (r != CL_SUCCESS) {
			return r;
		}
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

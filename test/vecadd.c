/* SPDX-License-Identifier: Apache-2.0 */

#define CL_TARGET_OPENCL_VERSION 220

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <CL/opencl.h>

#define MODE_BINARY

// OpenCL kernel. Each work item takes care of one element of c
const char *kernelSource =                                        "" \
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable                    \n" \
"__kernel void vecAdd(__global double *a,                         \n" \
"                     __global double *b,                         \n" \
"                     __global double *c,                         \n" \
"                     const unsigned int n)                      \n" \
"{                                                               \n" \
"    // Get our global thread ID                                 \n" \
"    int id = get_global_id(0);                                  \n" \
"                                                                \n" \
"    // Make sure we do not go out of bounds                     \n" \
"    if (id < n)                                                 \n" \
"        c[id] = a[id] + b[id];                                  \n" \
"}                                                               \n" \
"                                                                \n" ;

int main(int argc, char *argv[])
{
	// Length of vectors
	int n = 10;
	size_t bytes = n * sizeof(double);

	cl_platform_id cpPlatform[2];
	cl_device_id device_id;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;

#ifdef MODE_BINARY
	const char *fname = "a.out";
	int fd, r;
	struct stat st;
	unsigned char *bin[1];
	size_t bin_sz[1];
	ssize_t rd;
#endif

	// Device input/output buffers
	cl_mem d_a, d_b;
	cl_mem d_c;

	// Number of work items in each local work group
	size_t localSize = 64;

	// Number of total work items - localSize must be devisor
	size_t globalSize = ceil(n / (float)localSize) * localSize;

	cl_int err;

	// Bind to platform
	err = clGetPlatformIDs(2, cpPlatform, NULL);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	// Get ID for the device
	err = clGetDeviceIDs(cpPlatform[0], CL_DEVICE_TYPE_ACCELERATOR, 1, &device_id, NULL);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	// Create a context
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

#ifndef MODE_BINARY
	// Create the compute program from the source buffer
	program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &err);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}
#else
	if (argc > 1) {
		fname = argv[1];
	}

	fd = open(fname, O_RDONLY);
	if (fd == -1) {
		perror("open");
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	r = fstat(fd, &st);
	if (r != 0) {
		perror("fstat");
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	bin_sz[0] = st.st_size;
	bin[0] = calloc(bin_sz[0], sizeof(unsigned char));
	if (bin[0] == NULL) {
		perror("calloc");
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	rd = read(fd, bin[0], bin_sz[0]);
	if (rd != bin_sz[0]) {
		perror("read");
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	// Create the compute program from binary
	program = clCreateProgramWithBinary(context, 1, &device_id, bin_sz, (const unsigned char **)bin, NULL, &err);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}
#endif

	// Build the program executable
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	// Create the compute kernel in the program we wish to run
	kernel = clCreateKernel(program, "vecAdd", &err);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	// Create the input and output arrays in device memory for our calculation
	d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
	if (d_a == NULL) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}
	d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
	if (d_b == NULL) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}
	d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);
	if (d_c == NULL) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	// Set the arguments to our compute kernel
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}
	err = clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);
	if (err != CL_SUCCESS) {
		printf("%s:%d err:%d\n", __func__, __LINE__, err);
		return 0;
	}

	for (int j = 0; j < 5; j++) {
		struct timeval st, ed, ela;
		// Allocate memory for each vector on host
		double *h_a = (double *)malloc(bytes);
		double *h_b = (double *)malloc(bytes);
		double *h_c = (double *)malloc(bytes);

		// Initialize vectors on host
		for (int i = 0; i < n; i++) {
			h_a[i] = sinf(i) * sinf(i) + j;
			h_b[i] = cosf(i) * cosf(i) + j;
		}

		gettimeofday(&st, NULL);

		// Write our data set into the input array in device memory
		err = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0, bytes, h_a, 0, NULL, NULL);
		if (err != CL_SUCCESS) {
			printf("%s:%d err:%d\n", __func__, __LINE__, err);
			return 0;
		}
		err = clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0, bytes, h_b, 0, NULL, NULL);
		if (err != CL_SUCCESS) {
			printf("%s:%d err:%d\n", __func__, __LINE__, err);
			return 0;
		}

		// Execute the kernel over the entire range of the data set
		err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
		if (err != CL_SUCCESS) {
			printf("%s:%d err:%d\n", __func__, __LINE__, err);
			return 0;
		}

		// Wait for the command queue to get serviced before reading back results
		err = clFinish(queue);
		if (err != CL_SUCCESS) {
			printf("%s:%d err:%d\n", __func__, __LINE__, err);
			return 0;
		}

		gettimeofday(&ed, NULL);
		timersub(&ed, &st, &ela);

		// Read the results from the device
		err = clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0, bytes, h_c, 0, NULL, NULL);
		if (err != CL_SUCCESS) {
			printf("%s:%d err:%d\n", __func__, __LINE__, err);
			return 0;
		}

		//Sum up vector c and print result divided by n, this should equal 1 within error
		double sum = 0;
		for (int i = 0; i < n; i++) {
			sum += h_c[i];
		}
		printf("final result: %f\n", sum / n);
		printf("  time: %d.%06d\n", (int)ela.tv_sec, (int)ela.tv_usec);

		free(h_a);
		free(h_b);
		free(h_c);
	}

	clReleaseMemObject(d_a);
	clReleaseMemObject(d_b);
	clReleaseMemObject(d_c);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	return 0;
}

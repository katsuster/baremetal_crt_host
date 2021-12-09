/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <in_cl.h>
#include <drivers/device.h>

static int in_num_devices;
static cl_device_id in_devices = NULL;

int dev_get_number(void)
{
	return in_num_devices;
}

cl_device_id dev_get_devices(void)
{
	return in_devices;
}

cl_int dev_probe_devices(cl_platform_id platform)
{
	if (in_devices != NULL) {
		return CL_SUCCESS;
	}

	in_num_devices = 1;

	in_devices = calloc(in_num_devices, sizeof(struct _cl_device_id));
	if (in_devices == NULL) {
		perror("calloc(device_probe)");
		return CL_OUT_OF_HOST_MEMORY;
	}

	for (int i = 0; i < in_num_devices; i++) {
		in_devices[i].dispatch = platform->dispatch;
		in_devices[i].magic = OPENCL_ICD_MAGIC;
	}

	return CL_SUCCESS;
}

cl_int dev_remove_devices(cl_platform_id platform)
{
	/* TODO: to be implemented */
	in_num_devices = 0;
	in_devices = NULL;

	return CL_SUCCESS;
}

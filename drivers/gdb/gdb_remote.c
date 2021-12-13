/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>

#include <drivers/gdb/gdb_remote.h>

static cl_device_id in_devs[1];

static cl_int gdb_remote_probe(cl_device_id dev)
{
	if (dev == NULL) {
		return CL_INVALID_VALUE;
	}

	dev->dev_type = CL_DEVICE_TYPE_ACCELERATOR;

	printf("%s!!!!\n", __func__);

	return CL_SUCCESS;
}

static cl_int gdb_remote_remove(cl_device_id dev)
{
	if (dev == NULL) {
		return CL_INVALID_VALUE;
	}

	printf("%s!!!!\n", __func__);

	return CL_SUCCESS;
}

static const struct dev_ops gdb_remote_ops = {
	.probe = gdb_remote_probe,
	.remove = gdb_remote_remove,
};

cl_int gdb_remote_init(cl_platform_id platform)
{
	cl_device_id dev;
	cl_int r;

	r = dev_alloc(platform, &dev);
	if (r != CL_SUCCESS) {
		return r;
	}

	dev->ops = &gdb_remote_ops;

	r = dev_add(dev);
	if (r != CL_SUCCESS) {
		return r;
	}

	/* TODO: support multiple targets */
	in_devs[0] = dev;

	return CL_SUCCESS;
}

cl_int gdb_remote_exit(cl_platform_id platform)
{
	cl_int r;

	for (int i = 0; i < ARRAY_OF(in_devs); i++) {
		cl_device_id dev = in_devs[i];

		r = dev_remove(dev);
		if (r != CL_SUCCESS) {
			return r;
		}

		r = dev_free(dev);
		if (r != CL_SUCCESS) {
			return r;
		}
	}

	return CL_SUCCESS;
}

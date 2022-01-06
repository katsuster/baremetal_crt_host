/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_TYPE_CL
#define BAREMETAL_CRT_HOST_TYPE_CL

#include <config_cl.h>
#include <drivers/platform.h>
#include <drivers/device.h>

#define ARRAY_OF(x)    (sizeof(x) / sizeof((x)[0]))

#define LOG_HEADER    "%s:%d: "
#define log_dbg(fmt, ...) \
	do { \
		fprintf(stdout, LOG_HEADER fmt, __func__, __LINE__, ##__VA_ARGS__); \
	} while (0)
#define log_info(fmt, ...) \
	do { \
		fprintf(stdout, LOG_HEADER fmt, __func__, __LINE__, ##__VA_ARGS__); \
	} while (0)
#define log_warn(fmt, ...) \
	do { \
		fprintf(stdout, LOG_HEADER fmt, __func__, __LINE__, ##__VA_ARGS__); \
	} while (0)
#define log_err(fmt, ...) \
	do { \
		fprintf(stdout, LOG_HEADER fmt, __func__, __LINE__, ##__VA_ARGS__); \
	} while (0)

#endif /* BAREMETAL_CRT_HOST_TYPE_CL */

/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_TYPE_CL
#define BAREMETAL_CRT_HOST_TYPE_CL

#include <config_cl.h>
#include <drivers/platform.h>
#include <drivers/device.h>

#define KB    (1024)
#define MB    (1024 * 1024)
#define GB    (1024 * 1024 * 1024ULL)

#define NMAX(a, b)     (((a) > (b)) ? (a) : (b))
#define NMIN(a, b)     (((a) < (b)) ? (a) : (b))
#define ARRAY_OF(x)    (sizeof(x) / sizeof((x)[0]))
#define ALIGN_OF(x, align)    (((x) % (align) == 0) ? (x) : ((x) / (align) + 1) * (align))

#define LOG_LV_DBG     10
#define LOG_LV_INFO    20
#define LOG_LV_WARN    30
#define LOG_LV_ERR     40

#define LOG_HEADER    "%s:%d: "
#define log_dbg(fmt, ...) \
	do { \
		if (LOG_LV_DBG >= __log_lv) { \
			fprintf(stdout, "dbg : " LOG_HEADER fmt, __func__, __LINE__, ##__VA_ARGS__); \
		} \
	} while (0)
#define log_info(fmt, ...) \
	do { \
		if (LOG_LV_INFO >= __log_lv) { \
			fprintf(stdout, "info: " LOG_HEADER fmt, __func__, __LINE__, ##__VA_ARGS__); \
		} \
	} while (0)
#define log_warn(fmt, ...) \
	do { \
		if (LOG_LV_WARN >= __log_lv) { \
			fprintf(stdout, "warn: " LOG_HEADER fmt, __func__, __LINE__, ##__VA_ARGS__); \
		} \
	} while (0)
#define log_err(fmt, ...) \
	do { \
		if (LOG_LV_ERR >= __log_lv) { \
			fprintf(stdout, "err : " LOG_HEADER fmt, __func__, __LINE__, ##__VA_ARGS__); \
		} \
	} while (0)

extern int __log_lv;

#endif /* BAREMETAL_CRT_HOST_TYPE_CL */

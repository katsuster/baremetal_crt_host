/* SPDX-License-Identifier: Apache-2.0 */

#ifndef BAREMETAL_CRT_HOST_TYPE_CL
#define BAREMETAL_CRT_HOST_TYPE_CL

#include "config_cl.h"

struct _cl_platform_id {
	struct _cl_icd_dispatch *dispatch;
	int magic;
};

#endif /* BAREMETAL_CRT_HOST_TYPE_CL */

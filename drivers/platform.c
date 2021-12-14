/* SPDX-License-Identifier: Apache-2.0 */

#include <stdatomic.h>

#include <in_cl.h>
#include <drivers/platform.h>

static atomic_int in_uniq_id;

cl_int plat_get_uniq_id(void)
{
	return atomic_fetch_add_explicit(&in_uniq_id, 1, memory_order_relaxed);
}

// SPDX-License-Identifier: GPL-2.0-only OR MIT
/*
 * Copyright (C) 2023 The Falco Authors.
 *
 * This file is dual licensed under either the MIT or GPL 2. See MIT.txt
 * or GPL2.txt for full copies of the license.
 */

#include <helpers/interfaces/fixed_size_event.h>

/*=============================== ENTER EVENT ===========================*/

SEC("tp_btf/sys_enter")
int BPF_PROG(futex_e,
        struct pt_regs *regs,
        long id)
{
	struct ringbuf_struct ringbuf;
	if(!ringbuf__reserve_space(&ringbuf, ctx, FUTEX_E_SIZE, PPME_SYSCALL_FUTEX_E))
	{
		return 0;
	}

	ringbuf__store_event_header(&ringbuf);

	/*=============================== COLLECT PARAMETERS  ===========================*/

	/* Parameter 1: addr (type: PT_UINT64) */
	u64 addr = extract__syscall_argument(regs, 0);
	ringbuf__store_u64(&ringbuf, addr);

	/* Parameter 2: op (type: PT_ENUMFLAGS16) */
	s32 op = (s32)extract__syscall_argument(regs, 1);
	ringbuf__store_u16(&ringbuf, futex_op_to_scap((unsigned long)op));

	/* Parameter 3: val (type: PT_UINT64) */
	u64 val = extract__syscall_argument(regs, 2);
	ringbuf__store_u64(&ringbuf, val);

	/*=============================== COLLECT PARAMETERS  ===========================*/

	ringbuf__submit_event(&ringbuf);

	return 0;
}

/*=============================== ENTER EVENT ===========================*/

/*=============================== EXIT EVENT ===========================*/

SEC("tp_btf/sys_exit")
int BPF_PROG(futex_x,
        struct pt_regs *regs,
        long ret)
{
	struct ringbuf_struct ringbuf;
	if(!ringbuf__reserve_space(&ringbuf, ctx, FUTEX_X_SIZE, PPME_SYSCALL_FUTEX_X))
	{
		return 0;
	}

	ringbuf__store_event_header(&ringbuf);

	/*=============================== COLLECT PARAMETERS  ===========================*/

	/* Parameter 1: res (type: PT_ERRNO) */
	ringbuf__store_s64(&ringbuf, (s64)ret);

	/*=============================== COLLECT PARAMETERS  ===========================*/

	ringbuf__submit_event(&ringbuf);

	return 0;
}

/*=============================== EXIT EVENT ===========================*/

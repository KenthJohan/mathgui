#pragma once

#include "csc/csc_debug.h"
#include "csc/csc_debug_nng.h"

#include <nng/nng.h>
#include <nng/protocol/pair0/pair.h>
#include <nng/protocol/pair1/pair.h>
#include <nng/supplemental/util/platform.h>





static void mg_send (nng_socket sock, uint32_t entity, uint32_t attribute, void * ptr, size_t size)
{
	nng_msg * msg;
	nng_msg_alloc (&msg, 0);
	nng_msg_append_u32 (msg, entity);
	nng_msg_append_u32 (msg, attribute);
	nng_msg_append (msg, ptr, size);
	int r;
	r = nng_sendmsg (sock, msg, 0);
	if (r)
	{
		perror (nng_strerror (r));
	}
}


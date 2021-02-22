#pragma once

#include <flecs.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"
#include "csc/csc_debug_nng.h"

#include <nng/nng.h>
#include <nng/protocol/pair0/pair.h>
#include <nng/protocol/pair1/pair.h>
#include <nng/supplemental/util/platform.h>

#include "components.h"

#define EAVNET_ENTITY_MAX 1000
#define EAVNET_SEND(sock, type, data) SDLNet_TCP_Send ((sock), &(type)data, sizeof((type)data));



struct eavnet_context
{
	ecs_world_t * world;
	ecs_entity_t entities[EAVNET_ENTITY_MAX];
	nng_socket sock;
};


static void eavnet_context_init (struct eavnet_context * ctx, char const * address)
{
	ecs_entity_t const * e = ecs_bulk_new (ctx->world, 0, EAVNET_ENTITY_MAX);
	memcpy (ctx->entities, e, sizeof (ecs_entity_t) * EAVNET_ENTITY_MAX);
	int r;
	r = nng_pair0_open (&ctx->sock);
	NNG_EXIT_ON_ERROR (r);
	r = nng_listen (ctx->sock, address, NULL, 0);
	NNG_EXIT_ON_ERROR (r);
}


static void eavnet_receiver (struct eavnet_context * ctx, uint32_t entity, uint32_t attribute, void * ptr, uint32_t value_size)
{
	ecs_world_t * world = ctx->world;
	ecs_entity_t e = ctx->entities[entity];
	switch (attribute)
	{
	case ATTR_COUNT:{
		uint32_t * a = ptr;
		ecs_set (world, e, component_count, {*a});
		break;}
	case ATTR_POINTCLOUD:
		ecs_add (world, e, component_pointcloud);
		break;
	case ATTR_POINTCLOUD_POS:{
		component_pointcloud const * cloud = ecs_get (world, e, component_pointcloud);
		component_count const * count = ecs_get (world, e, component_count);
		uint32_t size = (*count) * sizeof (component_position);
		glBindBuffer (GL_ARRAY_BUFFER, cloud->vbop);
		component_position * p = ptr;
		for (int i = 0; i < 1; ++i)
		{
			p[i][0] = 0.0f;
			p[i][1] = 0.0f;
			p[i][2] = 0.0f;
			p[i][3] = 40.0f;
		}
		glBufferSubData (GL_ARRAY_BUFFER, 0, MIN(value_size, size), p);
		break;}
	case ATTR_POINTCLOUD_COL:{
		component_pointcloud const * cloud = ecs_get (world, e, component_pointcloud);
		component_count const * count = ecs_get (world, e, component_count);
		uint32_t size = (*count) * sizeof (component_color);
		glBindBuffer (GL_ARRAY_BUFFER, cloud->vboc);
		glBufferSubData (GL_ARRAY_BUFFER, 0, MIN(value_size, size), ptr);
		break;}
	case ATTR_MESH:
		ecs_add (world, e, component_mesh);
		ecs_add (world, e, component_vao);
		break;
	case ATTR_TEXTURE:{
		ecs_add (world, e, component_tbo);
		ecs_set_ptr (world, e, component_texture, ptr);
		break;}
	case ATTR_POSITION:{
		ecs_set_ptr (world, e, component_position, ptr);
		break;}
	case ATTR_SCALE:{
		ecs_set_ptr (world, e, component_scale, ptr);
		break;}
	case ATTR_QUATERNION:{
		ecs_set_ptr (world, e, component_quaternion, ptr);
		break;}
	case ATTR_ADD_INSTANCEOF:{
		uint32_t * a = ptr;
		ecs_add_entity (world, e, ECS_INSTANCEOF | ctx->entities[*a]);
		break;}
	case ATTR_RECTANGLE:{
		ecs_set_ptr (world, e, component_rectangle, ptr);
		break;}
	}

}


static void eavnet_receiver1 (struct eavnet_context * ctx)
{
	nng_msg *msg;
	int rv = nng_recvmsg (ctx->sock, &msg, NNG_FLAG_NONBLOCK | NNG_FLAG_ALLOC);
	if (rv == NNG_EAGAIN)
	{
		return;
	}
	else if (rv != 0)
	{
		NNG_EXIT_ON_ERROR (rv);
	}
	uint32_t entity;
	uint32_t attribute;
	nng_msg_trim_u32 (msg, &entity);
	nng_msg_trim_u32 (msg, &attribute);
	//printf ("nng_msg_len %i\n", nng_msg_len (msg));
	//float * body = nng_msg_body (msg);
	eavnet_receiver (ctx, entity, attribute, nng_msg_body (msg), nng_msg_len (msg));
	nng_msg_free (msg);
}


/*
static int eavnet_thread_recv (void * ptr)
{
	printf ("mythead started\n");
	struct eavnet_context * ctx = ptr;
	while (1)
	{
		size_t sz;
		void * buffer = NULL;
		printf ("mythead nng_recv\n");
		int rv = nng_recv (ctx->sock, &buffer, &sz, NNG_FLAG_ALLOC);
		if (rv == NNG_EAGAIN)
		{
			printf ("NNG_EAGAIN\n");
			continue;
		}
		else if (rv != 0)
		{
			NNG_EXIT_ON_ERROR (rv);
		}
		printf ("mythead sz: %lli\n", sz);
		if (sz >= sizeof (struct mynet_eav))
		{
			//eavnet_receiver (ctx, buffer, sz - sizeof (struct mynet_eav));
		}
		nng_free (buffer, sz);

		SDL_Delay (1000);
	}
	return 0;
}
*/




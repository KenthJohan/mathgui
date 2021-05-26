#pragma once

#include <flecs.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"
#include "csc/csc_debug_nng.h"
#include "csc/csc_xlog.h"

#include <nng/nng.h>
#include <nng/protocol/pair0/pair.h>
#include <nng/protocol/pair1/pair.h>
#include <nng/supplemental/util/platform.h>

#include "eavnet.h"
#include "systems.h"
#include "mg_attr.h"
#include "mg_comp.h"
#include "mg_compdef.h"



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
	case MG_MESH:
		ecs_add (world, e, Mesh_OpenGL);
		ecs_add (world, e, VAO_OpenGL);
		break;

	case MG_POINTCLOUD:
		ecs_add (world, e, Pointcloud_OpenGL);
		break;

	case MG_LINES:
		ecs_add (world, e, Lines_OpenGL);
		break;

	case MG_CAPACITY:{
		Capacity c = *(uint32_t*)ptr;
		ecs_set (world, e, Capacity, {c});
		break;}

	case MG_POINTCLOUD_POS:{
		//ecs_add (world, e, component_pointcloud);
		//ecs_progress (world, 0);
		Pointcloud_OpenGL const * cloud = ecs_get (world, e, Pointcloud_OpenGL);
		Capacity const * count = ecs_get (world, e, Capacity);
		ASSERT_NOTNULL (cloud);
		ASSERT_NOTNULL (count);
		ASSERT (glIsBuffer (cloud->vbop));
		uint32_t size = (*count) * sizeof (Position4);
		glBindBuffer (GL_ARRAY_BUFFER, cloud->vbop);
		Position4 * p = ptr;
		glBufferSubData (GL_ARRAY_BUFFER, 0, MIN(value_size, size), p);
		Count point_count = value_size / sizeof(Position4);
		ecs_set (world, e, Count, {point_count});
		ecs_set (world, e, Offset, {0});
		break;}

	case MG_POINTCLOUD_COL:{
		//ecs_add (world, e, component_pointcloud);
		//ecs_progress (world, 0);
		Pointcloud_OpenGL const * cloud = ecs_get (world, e, Pointcloud_OpenGL);
		Capacity const * count = ecs_get (world, e, Capacity);
		ASSERT_NOTNULL (cloud);
		ASSERT_NOTNULL (count);
		ASSERT (glIsBuffer (cloud->vboc));
		uint32_t size = (*count) * sizeof (Color);
		glBindBuffer (GL_ARRAY_BUFFER, cloud->vboc);
		Color * c = ptr;
		glBufferSubData (GL_ARRAY_BUFFER, 0, MIN(value_size, size), c);
		break;}

	case MG_LINES_POS:{
		//ecs_add (world, e, component_pointcloud);
		//ecs_progress (world, 0);
		Lines_OpenGL const * lines = ecs_get (world, e, Lines_OpenGL);
		Capacity const * count = ecs_get (world, e, Capacity);
		ASSERT_NOTNULL (lines);
		ASSERT_NOTNULL (count);
		ASSERT (glIsBuffer (lines->vbop));
		uint32_t size = (*count) * sizeof (Position4);
		glBindBuffer (GL_ARRAY_BUFFER, lines->vbop);
		Position4 * p = ptr;
		glBufferSubData (GL_ARRAY_BUFFER, 0, MIN(value_size, size), p);
		ecs_set (world, e, Count, {value_size / sizeof(Position4)});
		ecs_set (world, e, Offset, {0});
		break;}

	case MG_LINES_COL:{
		//ecs_add (world, e, component_pointcloud);
		//ecs_progress (world, 0);
		Lines_OpenGL const * lines = ecs_get (world, e, Lines_OpenGL);
		Capacity const * count = ecs_get (world, e, Capacity);
		ASSERT_NOTNULL (lines);
		ASSERT_NOTNULL (count);
		ASSERT (glIsBuffer (lines->vboc));
		uint32_t size = (*count) * sizeof (Color);
		glBindBuffer (GL_ARRAY_BUFFER, lines->vboc);
		Color * c = ptr;
		glBufferSubData (GL_ARRAY_BUFFER, 0, MIN(value_size, size), c);
		break;}

	case MG_TEXTURE:{
		ecs_add (world, e, Tex2DArray_OpenGL);
		ecs_set_ptr (world, e, Texture_OpenGL, ptr);
		break;}

	case MG_POSITION:{
		ecs_set_ptr (world, e, Position4, ptr);
		break;}

	case MG_SCALE:{
		ecs_set_ptr (world, e, Scale4, ptr);
		break;}

	case MG_QUATERNION:{
		ecs_set_ptr (world, e, Quaternion, ptr);
		break;}

	case MG_ADD_INSTANCEOF:{
		uint32_t * a = ptr;
		ecs_add_entity (world, e, ECS_INSTANCEOF | ctx->entities[*a]);
		break;}

	case MG_RECTANGLE:{
		ecs_set_ptr (world, e, Rectangle2f, ptr);
		break;}

	case MG_TRANSFORM:{
		ecs_set_ptr (world, e, Transform, ptr);
		break;}

	case MG_TEXTURE_CONTENT:{
		Texture_OpenGL const * texture = ecs_get (world, e, Texture_OpenGL);
		Tex2DArray_OpenGL const * tex = ecs_get (world, e, Tex2DArray_OpenGL);
		glActiveTexture (GL_TEXTURE0 + texture->unit);
		glBindTexture (GL_TEXTURE_2D_ARRAY, *tex);//Depends on glActiveTexture()
		ASSERT (value_size <= (texture->width * texture->height * sizeof (uint32_t)));
		{
			glTexSubImage3D (GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, texture->width, texture->height, 1, GL_RGBA, GL_UNSIGNED_BYTE, ptr);
		}
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
	else if (nng_msg_len (msg) < (sizeof (uint32_t) * 2))
	{
		nng_msg_free (msg);
		return;
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




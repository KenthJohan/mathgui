#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"


#include "mg_comp.h"
#include "systems.h"


//OpenGL:
ECS_COMPONENT_DECLARE (component_gl_program);
ECS_COMPONENT_DECLARE (component_vbo);
ECS_COMPONENT_DECLARE (component_vao);
ECS_COMPONENT_DECLARE (component_va);
ECS_COMPONENT_DECLARE (component_gl_tex2darray);

static void trigger_vao_onadd (ecs_iter_t *it)
{
	printf ("[ECS_TRIGGER] trigger_vao_onadd: ");
	ECS_COLUMN (it, component_vao, vao, 1);
	glGenVertexArrays (it->count, vao);
	for (int32_t i = 0; i < it->count; ++i)
	{
		printf ("%i, ", vao[i]);
	}
	printf ("\n");
}


static void trigger_gl_tex2darray_onadd (ecs_iter_t *it)
{
	printf ("[ECS_TRIGGER] trigger_tbo_onadd: ");
	ECS_COLUMN (it, component_gl_tex2darray, t, 1);
	glGenTextures (it->count, t);
	for (int32_t i = 0; i < it->count; ++i)
	{
		printf ("%i, ", t[i]);
	}
	printf ("\n");
}



static void system_opengl_init (ecs_world_t * world)
{
	ECS_COMPONENT_DEFINE (world, component_va);
	ECS_COMPONENT_DEFINE (world, component_vao);
	ECS_COMPONENT_DEFINE (world, component_vbo);
	ECS_COMPONENT_DEFINE (world, component_gl_tex2darray);
	ECS_COMPONENT_DEFINE (world, component_gl_program);
	ECS_TRIGGER (world, trigger_vao_onadd, EcsOnAdd, component_vao);
	ECS_TRIGGER (world, trigger_gl_tex2darray_onadd, EcsOnAdd, component_gl_tex2darray);
}






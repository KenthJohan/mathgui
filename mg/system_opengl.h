#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"


#include "mg_comp.h"
#include "mg_compdef.h"
#include "systems.h"




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


static void system_gl_shader_onset (ecs_iter_t *it)
{
	printf ("[ECS_SYSTEM] system_gl_shader_onset: ");
	ECS_COLUMN (it, component_gl_program, p, 1);
	ECS_COLUMN (it, component_filename, filename, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
		ASSERT (filename[i]);
		p[i] = csc_gl_shader_from_file (filename[i]);
		printf ("%i, ", p[i]);
	}
	printf ("\n");
}


static void system_gl_program_onset (ecs_iter_t *it)
{
	printf ("[ECS_SYSTEM] system_gl_program_onset: \n");
	ECS_COLUMN (it, component_gl_program, p, 1);
	ECS_COLUMN (it, component_gl_shader, s, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
		glAttachShader (p[0], s[i]);
		printf ("%i: %i, %i\n", i, p[0], s[i]);
	}
	printf ("\n");
}

static void trigger_gl_program_onadd (ecs_iter_t *it)
{
	printf ("[ECS_TRIGGER] trigger_gl_program_onadd: ");
	ECS_COLUMN (it, component_gl_program, p, 1);
	for (int32_t i = 0; i < it->count; ++i)
	{
		p[i] = glCreateProgram();
		printf ("%i, ", p[i]);
	}
	printf ("\n");

	/*
	ecs_query_t *query = ecs_query_new (it->world, "SHARED:component_gl_program, component_gl_shader");
	ecs_iter_t it1 = ecs_query_iter (query);
	while (ecs_query_next(&it1))
	{
		system_gl_program_onset (&it1);
	}
	*/
}



static void system_opengl_init (ecs_world_t * world)
{
	ECS_TRIGGER (world, trigger_vao_onadd, EcsOnAdd, component_vao);
	ECS_TRIGGER (world, trigger_gl_tex2darray_onadd, EcsOnAdd, component_gl_tex2darray);
	ECS_TRIGGER (world, trigger_gl_program_onadd, EcsOnAdd, component_gl_program);
	ECS_SYSTEM (world, system_gl_shader_onset, EcsOnSet, component_gl_shader, component_filename);
	ECS_SYSTEM (world, system_gl_program_onset, EcsOnUpdate, component_gl_program, CASCADE:component_gl_shader);

/*
	ecs_entity_t shader1 = ecs_new (world, 0);
	ecs_entity_t shader2 = ecs_new (world, 0);
	ecs_entity_t shader3 = ecs_new (world, 0);
	ecs_entity_t program = ecs_new (world, 0);

	ecs_add (world, program, component_gl_program);
	ecs_add (world, shader1, component_gl_shader);
	ecs_add (world, shader2, component_gl_shader);
	ecs_add (world, shader3, component_gl_shader);
	ecs_set (world, shader1, component_filename, {CSC_SRCDIR"shader_line.glvs"});
	ecs_set (world, shader2, component_filename, {CSC_SRCDIR"shader_line.glfs"});
	ecs_set (world, shader3, component_filename, {CSC_SRCDIR"shader_line.glfs"});

	ecs_add_entity (world, program, ECS_CHILDOF | shader1);
	ecs_add_entity (world, program, ECS_CHILDOF | shader2);
	ecs_add_entity (world, program, ECS_CHILDOF | shader3);


	ecs_progress (world, 0);


	ecs_set (world, img, component_rectangle, {1.0f, 1.0f});

	ecs_entity_t const * e2 = ecs_bulk_new (world, component_transform, 4);
	ecs_add_entity (world, e2[0], ECS_INSTANCEOF | mytexture1);
	ecs_add_entity (world, e2[1], ECS_INSTANCEOF | mytexture1);
	ecs_add_entity (world, e2[2], ECS_INSTANCEOF | mytexture2);
	ecs_add_entity (world, e2[3], ECS_INSTANCEOF | mytexture3);
	*/
}






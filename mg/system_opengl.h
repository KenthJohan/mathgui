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
	XLOG(XLOG_INF, XLOG_ECS, "glGenVertexArrays n=%i", it->count);
	ECS_COLUMN (it, GL_VertexArrayObject, vao, 1);
	glGenVertexArrays (it->count, vao);
	for (int32_t i = 0; i < it->count; ++i)
	{
		//printf ("%i, ", vao[i]);
	}
	//printf ("\n");
}


static void trigger_gl_tex2darray_onadd (ecs_iter_t *it)
{
	XLOG(XLOG_INF, XLOG_ECS, "glGenTextures n=%i", it->count);
	ECS_COLUMN (it, GL_Tex2DArray, t, 1);
	glGenTextures (it->count, t);
	for (int32_t i = 0; i < it->count; ++i)
	{
		//printf ("%i, ", t[i]);
	}
	//printf ("\n");
}


static void system_gl_shader_onset (ecs_iter_t *it)
{
	XLOG(XLOG_INF, XLOG_ECS, "csc_gl_shader_from_file n=%i", it->count);
	ECS_COLUMN (it, GL_Program, p, 1);
	ECS_COLUMN (it, Filename, filename, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
		ASSERT (filename[i]);
		p[i] = csc_gl_shader_from_file (filename[i]);
		//printf ("%i, ", p[i]);
	}
	//printf ("\n");
}


static void system_gl_program_onset (ecs_iter_t *it)
{
	printf ("[ECS_SYSTEM] system_gl_program_onset: \n");
	ECS_COLUMN (it, GL_Program, p, 1);
	ECS_COLUMN (it, GL_Shader, s, 2);
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
	ECS_COLUMN (it, GL_Program, p, 1);
	for (int32_t i = 0; i < it->count; ++i)
	{
		p[i] = glCreateProgram();
		printf ("%i, ", p[i]);
	}
	printf ("\n");

	/*
	ecs_query_t *query = ecs_query_new (it->world, "SHARED:Program_OpenGL, Shader_OpenGL");
	ecs_iter_t it1 = ecs_query_iter (query);
	while (ecs_query_next(&it1))
	{
		system_gl_program_onset (&it1);
	}
	*/
}



static void system_opengl_init (ecs_world_t * world)
{
	ECS_TRIGGER (world, trigger_vao_onadd, EcsOnAdd, GL_VertexArrayObject);
	ECS_TRIGGER (world, trigger_gl_tex2darray_onadd, EcsOnAdd, GL_Tex2DArray);
	ECS_TRIGGER (world, trigger_gl_program_onadd, EcsOnAdd, GL_Program);
	ECS_SYSTEM (world, system_gl_shader_onset, EcsOnSet, GL_Shader, Filename);
	ECS_SYSTEM (world, system_gl_program_onset, EcsOnUpdate, GL_Program, CASCADE:GL_Shader);

/*
	ecs_entity_t shader1 = ecs_new (world, 0);
	ecs_entity_t shader2 = ecs_new (world, 0);
	ecs_entity_t shader3 = ecs_new (world, 0);
	ecs_entity_t program = ecs_new (world, 0);

	ecs_add (world, program, Program_OpenGL);
	ecs_add (world, shader1, Shader_OpenGL);
	ecs_add (world, shader2, Shader_OpenGL);
	ecs_add (world, shader3, Shader_OpenGL);
	ecs_set (world, shader1, Filename, {CSC_SRCDIR"shader_line.glvs"});
	ecs_set (world, shader2, Filename, {CSC_SRCDIR"shader_line.glfs"});
	ecs_set (world, shader3, Filename, {CSC_SRCDIR"shader_line.glfs"});

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






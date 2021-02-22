#pragma once

#include "csc/csc_math.h"
#include <flecs.h>
#include <GL/glew.h>

#include "component_vao.h"

typedef GLuint component_vbo;
typedef struct component_va
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	uint32_t offset;
} component_va;



ECS_COMPONENT_DECLARE (component_vbo);
ECS_COMPONENT_DECLARE (component_va);


static void component_vbo_onadd (ecs_iter_t *it)
{
	printf ("component_vbo_onadd\n");
	ECS_COLUMN (it, component_vbo, vao, 1);
	glGenBuffers (it->count, vao);
	for (int32_t i = 0; i < it->count; ++i){}
}



static void component_vbo_onset (ecs_iter_t *it)
{
	printf ("component_vbo_onset\n");
}

static void component_va_onset (ecs_iter_t *it)
{
	printf ("component_va_onset\n");
	ECS_COLUMN (it, component_va, va, 1);
	ECS_COLUMN (it, component_vbo, vbo, 2);
	ECS_COLUMN (it, component_vao, vao, 3); //SHARED
	glBindVertexArray (vao[0]);
	for (int32_t i = 0; i < it->count; ++i)
	{
		glBindBuffer (GL_ARRAY_BUFFER, vbo[i]);
		glVertexAttribPointer (va[i].index, va[i].size, va[i].type, va[i].normalized, va[i].stride, (void*)(intptr_t)va[i].offset);
		glEnableVertexAttribArray (0);
	}
}


static void component_vbo_init (ecs_world_t * world)
{
	printf ("component_vbo_init\n");
	ECS_COMPONENT_DEFINE (world, component_vbo);
	ECS_COMPONENT_DEFINE (world, component_va);
	ECS_TRIGGER (world, component_vbo_onadd, EcsOnAdd, component_vbo);
	ECS_SYSTEM (world, component_vbo_onset, EcsOnSet, component_vbo);
	ECS_SYSTEM (world, component_va_onset, EcsOnSet, component_va, component_vbo, SHARED:component_vao);
}




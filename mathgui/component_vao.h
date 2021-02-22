#pragma once

#include "csc/csc_math.h"
#include <flecs.h>
#include <GL/glew.h>

typedef GLuint component_vao;



ECS_COMPONENT_DECLARE (component_vao);


static void component_vao_onadd (ecs_iter_t *it)
{
	printf ("component_vao_onadd\n");
	ECS_COLUMN (it, component_vao, vao, 1);
	glGenVertexArrays (it->count, vao);
	for (int32_t i = 0; i < it->count; ++i){}
}



static void component_vao_onset (ecs_iter_t *it)
{
	printf ("component_vao_onset\n");
}




static void component_vao_init (ecs_world_t * world)
{
	printf ("component_vao_init\n");
	ECS_COMPONENT_DEFINE (world, component_vao);
	ECS_TRIGGER (world, component_vao_onadd, EcsOnAdd, component_vao);
	ECS_SYSTEM (world, component_vao_onset, EcsOnSet, component_vao);
}




#pragma once

#include "csc/csc_math.h"
#include <flecs.h>
#include <GL/glew.h>
#include "components.h"
#include "component_tbo.h"
#include "component_vao.h"
#include "component_vbo.h"


typedef uint32_t component_imglayer;
typedef uint32_t component_imgvbop;
typedef uint32_t component_imgvbot;



ECS_COMPONENT_DECLARE (component_imglayer);
ECS_COMPONENT_DECLARE (component_imgvbop);
ECS_COMPONENT_DECLARE (component_imgvbot);




static void component_img_onset (ecs_iter_t *it)
{
	printf ("component_img_onset\n");
	ECS_COLUMN (it, component_position, position, 1);
	ECS_COLUMN (it, component_imglayer, imglayer, 2);
	//ECS_COLUMN (it, component_imgvbop, vbop, 3); //SHARED
	for (int32_t i = 0; i < it->count; ++i)
	{

	}
}


static void component_img_onupdate (ecs_iter_t *it)
{
	printf ("component_img_onupdate\n");
	ECS_COLUMN (it, component_position, position, 1);
	ECS_COLUMN (it, component_imglayer, imglayer, 2);
	ECS_COLUMN (it, component_vao, vao, 3); //SHARED
	glBindVertexArray (vao[0]);
	for (int32_t i = 0; i < it->count; ++i)
	{

	}
}

static void component_img_init (ecs_world_t * world)
{
	printf ("component_img_init\n");
	ECS_COMPONENT_DEFINE (world, component_imglayer);
	ECS_COMPONENT_DEFINE (world, component_imgvbop);
	ECS_COMPONENT_DEFINE (world, component_imgvbot);

	//ECS_SYSTEM (world, component_img_onset, EcsOnSet, component_position, component_imglayer, SHARED:component_imgvbop);
	ECS_SYSTEM (world, component_img_onset, EcsOnSet, component_position, component_imglayer);
	//ECS_SYSTEM (world, component_img_onupdate, EcsOnUpdate, component_position, component_imglayer, SHARED:component_vao);
}



static void component_img_test (ecs_world_t * world)
{
	ecs_defer_begin (world);
	ecs_defer_end (world);
	ecs_entity_t const * e = ecs_bulk_new (world, component_tbo, 2);


	ECS_ENTITY (world, e_vao, component_vao);
	ECS_ENTITY (world, e_vbop, component_vbo);
	ECS_ENTITY (world, e_vbot, component_vbo);
	/*
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	uint32_t offset;
	*/
	ecs_set (world, e_vbop, component_va, {.index = 0, .size = 4, .type = GL_FLOAT, .normalized = GL_FALSE, 0});
	ecs_set (world, e_vbot, component_va, {.index = 0, .size = 2, .type = GL_FLOAT, .normalized = GL_FALSE, 0});
	ecs_add_entity (world, e_vbop, ECS_INSTANCEOF | e_vao);
	ecs_add_entity (world, e_vbot, ECS_INSTANCEOF | e_vao);

	ecs_defer_begin (world);
	ecs_defer_end (world);

	//ecs_set (world, e[0], component_imgvbop, {0});
	//ecs_set (world, e[1], component_imgvbop, {0});
	ecs_set (world, e[0], component_imglayer, {0});
	ecs_set (world, e[1], component_imglayer, {0});
	ecs_set (world, e[0], component_texture, {.unit = 0, .width = 100, .height = 100, .depth = 4});
	ecs_set (world, e[1], component_texture, {.unit = 0, .width = 200, .height = 200, .depth = 4});

	//ecs_set (world, e[0], component_position, {0.0f, 0.0f, 0.0f, 1.0f});
	//ecs_set (world, e[1], component_position, {0.0f, 1.0f, 0.0f, 1.0f});

}

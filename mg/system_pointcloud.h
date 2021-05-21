#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"
#include "csc/csc_vu32.h"


#include "mg_comp.h"
#include "mg_compdef.h"
#include "systems.h"






static void system_pointcloud_set (ecs_iter_t *it)
{
	printf ("[ECS_SYSTEM] system_pointcloud_set\n");
	ECS_COLUMN (it, component_pointcloud, pc, 1);
	ECS_COLUMN (it, component_capacity, c, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
		printf ("system_pointcloud_set component_capacity %i\n", c[i]);
		void * data;
		glGenVertexArrays (1, &pc[i].vao);
		glGenBuffers (1, &pc[i].vbop);
		glGenBuffers (1, &pc[i].vboc);

		data = malloc(c[i] * sizeof (component_position));
		v4f32_repeat_random (c[i], data);
		v4f32_set_w_repeat (c[i], data, 10.0f);
		glBindVertexArray (pc[i].vao);
		glBindBuffer (GL_ARRAY_BUFFER, pc[i].vbop);
		glBufferData (GL_ARRAY_BUFFER, c[i] * sizeof (component_position), data, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray (0);
		glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)0);
		free (data);

		data = malloc(c[i] * sizeof (component_color));
		vu32_repeat_random_mask (c[i], data, 0xFFFFFFFF);
		glBindBuffer (GL_ARRAY_BUFFER, pc[i].vboc);
		glBufferData (GL_ARRAY_BUFFER, c[i] * sizeof (component_color), data, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray (1);
		glVertexAttribPointer (1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)(intptr_t)0);
		free (data);
	}
}


static void system_pointcloud_draw (ecs_iter_t *it)
{
	ECS_COLUMN (it, component_pointcloud, pc, 1);
	ECS_COLUMN (it, component_offset, o, 2);
	ECS_COLUMN (it, component_count, c, 3);
	glUseProgram (global_glprogram[GLPROGRAM_POINT]);
	for (int32_t i = 0; i < it->count; ++i)
	{
		GLint first = o[i];
		GLsizei count = c[i];
		glBindVertexArray (pc[i].vao);
		glUniformMatrix4fv (global_gluniform[GLUNIFORM_POINT_MVP], 1, GL_FALSE, (const GLfloat *) &global_gcam.mvp);
		count = 5914;
		glDrawArrays (GL_POINTS, first, count);
	}
}


static void system_pointcloud_init (ecs_world_t * world)
{
	ECS_SYSTEM (world, system_pointcloud_set, EcsOnSet, component_pointcloud, component_capacity);
	ECS_SYSTEM (world, system_pointcloud_draw, EcsOnUpdate, component_pointcloud, component_offset, component_count);
}






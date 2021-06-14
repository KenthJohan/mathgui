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



static void system_lines_onset (ecs_iter_t *it)
{
	XLOG (XLOG_INF, XLOG_ECS, "EcsOnSet %i", it->count);
	ECS_COLUMN (it, GL_Lines, lines, 1);
	ECS_COLUMN (it, Capacity, count, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
		XLOG (XLOG_INF, XLOG_ECS, "capacity %i", count[i]);
		void * data;
		glGenVertexArrays (1, &lines[i].vao);
		glGenBuffers (1, &lines[i].vbop);
		glGenBuffers (1, &lines[i].vboc);
		glBindVertexArray (lines[i].vao);

		glEnableVertexAttribArray (0);
		glEnableVertexAttribArray (1);

		data = malloc(count[i] * sizeof (Position4));
		v4f32_repeat_random (count[i], data);
		v4f32_set_w_repeat (count[i], data, 10.0f);
		glBindBuffer (GL_ARRAY_BUFFER, lines[i].vbop);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (Position4), data, GL_DYNAMIC_DRAW);
		glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)0);
		free (data);

		data = malloc(count[i] * sizeof (Color));
		vu32_repeat_random_mask (count[i], data, 0xFFFFFFFF);
		glBindBuffer (GL_ARRAY_BUFFER, lines[i].vboc);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (Color), data, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray (1);
		glVertexAttribPointer (1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)(intptr_t)0);
		free (data);

		ASSERT (glIsBuffer(lines[i].vbop) == GL_TRUE);
		ASSERT (glIsBuffer(lines[i].vboc) == GL_TRUE);
	}
}



static void system_lines_draw (ecs_iter_t *it)
{
	ECS_COLUMN (it, GL_Lines, lines, 1);
	ECS_COLUMN (it, Offset, o, 2);
	ECS_COLUMN (it, Count, c, 3);
	glUseProgram (global_glprogram[GLPROGRAM_LINE]);
	for (int32_t i = 0; i < it->count; ++i)
	{
		glBindVertexArray (lines[i].vao);
		glUniformMatrix4fv (global_gluniform[GLUNIFORM_LINE_MVP], 1, GL_FALSE, (const GLfloat *) &global_gcam.mvp);
		glDrawArrays (GL_LINES, o[i], c[i]);
	}
}


static void system_lines_init (ecs_world_t * world)
{
	ECS_SYSTEM (world, system_lines_draw, EcsOnUpdate, GL_Lines, Offset, Count);
	ECS_SYSTEM (world, system_lines_onset, EcsOnSet, GL_Lines, Capacity);
}






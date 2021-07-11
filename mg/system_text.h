#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"
#include "csc/csc_xlog.h"


#include "mg_comp.h"
#include "mg_compdef.h"
#include "systems.h"
#include "text.h"






static void system_text_draw (ecs_iter_t *it)
{
	ECS_COLUMN (it, Text, text, 1);
	ECS_COLUMN (it, Position3, p, 2);
	ECS_COLUMN (it, Scale2, s, 3);
	//XLOG (XLOG_INF, XLOG_ECS, "%i", it->count);
	v4f32 pos[6 * 100];
	v2f32 uv[6 * 100];
	uint32_t n = 0;
	for (int32_t i = 0; i < it->count; ++i)
	{
		n += gtext2_gen (pos + n, uv + n, text[i].value, gtext_ctx.c, 6*100, gtext_ctx.atlas.w, gtext_ctx.atlas.h, p[i].x, p[i].y, p[i].z, s[i].x, s[i].y);
		//gtext_context_draw (&gtext_ctx, text[i].value, 0.0f, 0.0f, 2.0f / 400, 2.0f / 400, global_gcam.mvp.m);
	}
	glBindVertexArray (gtext_ctx.vao);
	glUseProgram (gtext_ctx.program);
	glBindTexture (GL_TEXTURE_2D, gtext_ctx.tex);
	glUniform1i (gtext_ctx.uniform_tex, 0);
	glUniformMatrix4fv (gtext_ctx.uniform_mvp, 1, GL_FALSE, (const GLfloat *) global_gcam.mvp.m);
	glBindBuffer (GL_ARRAY_BUFFER, gtext_ctx.vbo_pos);
	glBufferData (GL_ARRAY_BUFFER, sizeof(v4f32)*n, pos, GL_DYNAMIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, gtext_ctx.vbo_uv);
	glBufferData (GL_ARRAY_BUFFER, sizeof(v2f32)*n, uv, GL_DYNAMIC_DRAW);
	glDrawArrays (GL_TRIANGLES, 0, n);
}



static void system_text_init (ecs_world_t * world)
{
	ECS_SYSTEM (world, system_text_draw, EcsOnUpdate, Text, Position3, Scale2);
}











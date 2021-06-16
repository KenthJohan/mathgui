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
	//XLOG (XLOG_INF, XLOG_ECS, "%i", it->count);
	for (int32_t i = 0; i < it->count; ++i)
	{
		//gtext_context_draw (&gtext_ctx, text[i].value, 0.0f, 0.0f, 2.0f / 400, 2.0f / 400, global_gcam.mvp.m);
	}
	//
}



static void system_text_init (ecs_world_t * world)
{
	ECS_SYSTEM (world, system_text_draw, EcsOnUpdate, Text, Position3);
}











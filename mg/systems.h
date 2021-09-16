#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"
#include "csc/csc_xlog.h"
#include "csc/csc_gft.h"
#include "csc/experiment/gtext2.h"



#include "text.h"
#include "mg_compdef.h"
#include "mg_comp.h"






enum glprogram_type
{
	GLPROGRAM_POINT,
	GLPROGRAM_LINE,
	GLPROGRAM_MESH,
	GLPROGRAM_TEXT,
	GLPROGRAM_COUNT,
};

enum gluniform_type
{
	GLUNIFORM_POINT_MVP,
	GLUNIFORM_LINE_MVP,
	GLUNIFORM_MESH_MVP,
	GLUNIFORM_MESH_TEX0,
	GLUNIFORM_TEXT_MVP,
	GLUNIFORM_TEXT_TEX0,
	GLUNIFORM_COUNT,
};

static GLint global_glprogram[GLPROGRAM_COUNT];
static GLint global_gluniform[GLUNIFORM_COUNT];
static struct csc_gcam global_gcam;


static struct gtext2_context gtext_ctx;





static void trigger_transform (ecs_iter_t *it)
{
	XLOG (XLOG_INF, XLOG_ECS, " EcsOnAdd: %i", it->count);
	Transform * t = ecs_term (it, Transform, 1);
	for (int32_t i = 0; i < it->count; ++i)
	{
		m4f32_identity (t + i);
	}
}



static void system_apply_rotation (ecs_iter_t *it)
{
	//XLOG (XLOG_INF, XLOG_ECS, " OnUpdate: %i\n", it->count);
	Quaternion   * q = ecs_term (it, Quaternion  , 1);
	SDL_Keyboard * c = ecs_term (it, SDL_Keyboard, 2);

	for (int32_t i = 0; i < it->count; ++i)
	{
		float x = c->keyboard[SDL_SCANCODE_1];
		float y = c->keyboard[SDL_SCANCODE_2];
		float z = c->keyboard[SDL_SCANCODE_3];
		qf32_rotate2_xyza (q + i, x, y, z, 0.01f);
	}
}


static void system_transform_onset (ecs_iter_t *it)
{
	XLOG (XLOG_INF, XLOG_ECS, " EcsOnSet: %i", it->count);
	Position4  * p = ecs_term (it, Position4  , 1);
	Scale4     * s = ecs_term (it, Scale4     , 2);
	Quaternion * q = ecs_term (it, Quaternion , 3);
	Transform  * t = ecs_term (it, Transform  , 4);

	for (int32_t i = 0; i < it->count; ++i)
	{
		m4f32 * m = t + i;
		m4f32 ms;
		m4f32 mt;
		m4f32 mr;
		m4f32_identity (m);
		m4f32_identity (&ms);
		m4f32_identity (&mr);
		m4f32_identity (&mt);
		m4f32_scale (&ms, s + i);
		qf32_m4 (&mr, q + i);
		m4f32_translation4 (&mt, p + i);
		m4f32_mul (m, &ms, m); //Apply scale
		m4f32_mul (m, &mr, m); //Apply rotation
		m4f32_mul (m, &mt, m); //Apply translation
	}
}


static void systems_init (ecs_world_t * world)
{
	ecs_set_component_actions(world, Text,
	{
	.ctor = ecs_ctor(Text),
	.dtor = ecs_dtor(Text),
	.copy = ecs_copy(Text),
	.move = ecs_move(Text)
	});


	//Testing text system:
	/*
	ecs_entity_t e1 = ecs_new (world, 0);
	ecs_set (world, e1, Text, {"Lemon1"});
	ecs_set (world, e1, Position3, {{0.0f, 0.0f, 0.0f}});
	ecs_set (world, e1, Scale2, {{0.01f, 0.01f}});
	ecs_entity_t e2 = ecs_new (world, 0);
	ecs_set (world, e2, Text, {"Lemon2"});
	ecs_set (world, e2, Position3, {{0.0f, 0.5f, 1.0f}});
	ecs_set (world, e2, Scale2, {{0.01f, 0.01f}});
	printf ("e: %s\n", ecs_get (world, e1, Text)->value);
	*/





	ECS_TRIGGER (world, trigger_transform, EcsOnAdd, Transform);
	ECS_SYSTEM (world, system_transform_onset, EcsOnSet, Position4, Scale4, Quaternion, Transform);
	ECS_SYSTEM (world, system_apply_rotation, EcsOnUpdate, Quaternion, $SDL_Keyboard);
	//ECS_SYSTEM (world, component_tbo_onadd, EcsMonitor, component_tbo);

	global_glprogram[GLPROGRAM_POINT] = csc_gl_program_from_files1 (CSC_SRCDIR"shader_pointcloud.glvs;"CSC_SRCDIR"shader_pointcloud.glfs");
	global_glprogram[GLPROGRAM_LINE] = csc_gl_program_from_files1 (CSC_SRCDIR"shader_line.glvs;"CSC_SRCDIR"shader_line.glfs");
	global_glprogram[GLPROGRAM_MESH] = csc_gl_program_from_files1 (CSC_SRCDIR"shader_image.glvs;"CSC_SRCDIR"shader_image.glfs");
	global_glprogram[GLPROGRAM_TEXT] = csc_gl_program_from_files1 (CSC_SRCDIR"shader_text.glvs;"CSC_SRCDIR"shader_text.glfs");
	glLinkProgram (global_glprogram[GLPROGRAM_POINT]);
	glLinkProgram (global_glprogram[GLPROGRAM_LINE]);
	glLinkProgram (global_glprogram[GLPROGRAM_MESH]);
	glLinkProgram (global_glprogram[GLPROGRAM_TEXT]);

	global_gluniform[GLUNIFORM_POINT_MVP] = glGetUniformLocation (global_glprogram[GLPROGRAM_POINT], "mvp");
	global_gluniform[GLUNIFORM_LINE_MVP] = glGetUniformLocation (global_glprogram[GLPROGRAM_LINE], "mvp");
	global_gluniform[GLUNIFORM_MESH_MVP] = glGetUniformLocation (global_glprogram[GLPROGRAM_MESH], "mvp");
	global_gluniform[GLUNIFORM_MESH_TEX0] = glGetUniformLocation (global_glprogram[GLPROGRAM_MESH], "tex0");
	global_gluniform[GLUNIFORM_TEXT_MVP] = glGetUniformLocation (global_glprogram[GLPROGRAM_MESH], "mvp");
	global_gluniform[GLUNIFORM_TEXT_TEX0] = glGetUniformLocation (global_glprogram[GLPROGRAM_MESH], "tex0");

	csc_gcam_init (&global_gcam);
	global_gcam.p.z = -1.0f;





	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		XLOG (XLOG_ERR, XLOG_GENERAL, "Could not init FreeType Library");
		ASSERT(0);
	}


	if (FT_New_Face(ft, "consola.ttf", 0, &gtext_ctx.face))
	{
		XLOG (XLOG_ERR, XLOG_GENERAL, "Failed to load font");
		ASSERT(0);
	}
	FT_Set_Pixel_Sizes (gtext_ctx.face, 0, 24);
	gtext2_context_init (&gtext_ctx, global_glprogram[GLPROGRAM_TEXT]);
}






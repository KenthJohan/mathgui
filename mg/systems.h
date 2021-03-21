#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"


#include "mg_comp.h"






enum glprogram_type
{
	GLPROGRAM_POINT,
	GLPROGRAM_LINE,
	GLPROGRAM_MESH,
	GLPROGRAM_COUNT,
};

enum gluniform_type
{
	GLUNIFORM_POINT_MVP,
	GLUNIFORM_LINE_MVP,
	GLUNIFORM_MESH_MVP,
	GLUNIFORM_MESH_TEX0,
	GLUNIFORM_COUNT,
};

static GLint global_glprogram[GLPROGRAM_COUNT];
static GLint global_gluniform[GLUNIFORM_COUNT];
static struct csc_gcam global_gcam;





static void trigger_transform (ecs_iter_t *it)
{
	printf ("[ECS_TRIGGER] trigger_transform_onadd: ");
	ECS_COLUMN (it, component_transform, t, 1);
	for (int32_t i = 0; i < it->count; ++i)
	{
		m4f32_identity (t + i);
	}
	printf ("\n");
}


static void system_apply_rotation (ecs_iter_t *it)
{
	ECS_COLUMN (it, component_quaternion, q, 1);
	ECS_COLUMN (it, component_controller, c, 2);//Singleton
	for (int32_t i = 0; i < it->count; ++i)
	{
		qf32_rotate2_xyza (q + i, c->keyboard[SDL_SCANCODE_1], c->keyboard[SDL_SCANCODE_2], c->keyboard[SDL_SCANCODE_3], 0.01f);
	}
}


static void system_transform_onset (ecs_iter_t *it)
{
	printf ("system_transform_onset\n");
	ECS_COLUMN (it, component_position, p, 1);
	ECS_COLUMN (it, component_scale, s, 2);
	ECS_COLUMN (it, component_quaternion, q, 3);
	ECS_COLUMN (it, component_transform, t, 4);
	for (int32_t i = 0; i < it->count; ++i)
	{
		struct m4f32 * m = t + i;
		struct m4f32 ms;
		struct m4f32 mt;
		struct m4f32 mr;
		m4f32_identity (m);
		m4f32_identity (&ms);
		m4f32_identity (&mr);
		m4f32_identity (&mt);
		m4f32_scale (&ms, s + i);
		qf32_m4 (&mr, q + i);
		m4f32_translation (&mt, p + i);
		m4f32_mul (m, &ms, m); //Apply scale
		m4f32_mul (m, &mr, m); //Apply rotation
		m4f32_mul (m, &mt, m); //Apply translation
	}
}


static void systems_init (ecs_world_t * world)
{
	ECS_TRIGGER (world, trigger_transform, EcsOnAdd, component_transform);
	ECS_SYSTEM (world, system_transform_onset, EcsOnSet, component_position, component_scale, component_quaternion, component_transform);
	ECS_SYSTEM (world, system_apply_rotation, EcsOnUpdate, component_quaternion, $component_controller);
	//ECS_SYSTEM (world, component_tbo_onadd, EcsMonitor, component_tbo);

	global_glprogram[GLPROGRAM_POINT] = csc_gl_program_from_files1 (CSC_SRCDIR"shader_pointcloud.glvs;"CSC_SRCDIR"shader_pointcloud.glfs");
	global_glprogram[GLPROGRAM_LINE] = csc_gl_program_from_files1 (CSC_SRCDIR"shader_line.glvs;"CSC_SRCDIR"shader_line.glfs");
	global_glprogram[GLPROGRAM_MESH] = csc_gl_program_from_files1 (CSC_SRCDIR"shader_image.glvs;"CSC_SRCDIR"shader_image.glfs");
	glLinkProgram (global_glprogram[GLPROGRAM_POINT]);
	glLinkProgram (global_glprogram[GLPROGRAM_LINE]);
	glLinkProgram (global_glprogram[GLPROGRAM_MESH]);

	global_gluniform[GLUNIFORM_POINT_MVP] = glGetUniformLocation (global_glprogram[GLPROGRAM_POINT], "mvp");
	global_gluniform[GLUNIFORM_LINE_MVP] = glGetUniformLocation (global_glprogram[GLPROGRAM_LINE], "mvp");
	global_gluniform[GLUNIFORM_MESH_MVP] = glGetUniformLocation (global_glprogram[GLPROGRAM_MESH], "mvp");
	global_gluniform[GLUNIFORM_MESH_TEX0] = glGetUniformLocation (global_glprogram[GLPROGRAM_MESH], "tex0");

	csc_gcam_init (&global_gcam);
	global_gcam.p.z = -1.0f;
}






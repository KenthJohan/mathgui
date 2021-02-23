#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"


#include "mg_comp.h"

//OpenGL:
ECS_COMPONENT_DECLARE (component_gl_tex2darray);
ECS_COMPONENT_DECLARE (component_vbo);
ECS_COMPONENT_DECLARE (component_vao);
ECS_COMPONENT_DECLARE (component_va);
ECS_COMPONENT_DECLARE (component_pointcloud);
ECS_COMPONENT_DECLARE (component_mesh);

//Misc:
ECS_COMPONENT_DECLARE (component_stride);
ECS_COMPONENT_DECLARE (component_count);
ECS_COMPONENT_DECLARE (component_texture);
ECS_COMPONENT_DECLARE (component_color);
ECS_COMPONENT_DECLARE (component_position);
ECS_COMPONENT_DECLARE (component_scale);
ECS_COMPONENT_DECLARE (component_quaternion);
ECS_COMPONENT_DECLARE (component_uv);
ECS_COMPONENT_DECLARE (component_rectangle);
ECS_COMPONENT_DECLARE (component_transform);

//Input control:
ECS_COMPONENT_DECLARE (component_controller);

//Application:
ECS_COMPONENT_DECLARE (component_applyrotation);

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




static void trigger_gl_tex2darray_onadd (ecs_iter_t *it)
{
	printf ("[ECS_TRIGGER] trigger_tbo_onadd: ");
	ECS_COLUMN (it, component_gl_tex2darray, t, 1);
	glGenTextures (it->count, t);
	for (int32_t i = 0; i < it->count; ++i)
	{
		printf ("%i, ", t[i]);
	}
	printf ("\n");
}


static void trigger_vao_onadd (ecs_iter_t *it)
{
	printf ("[ECS_TRIGGER] trigger_vao_onadd: ");
	ECS_COLUMN (it, component_vao, vao, 1);
	glGenVertexArrays (it->count, vao);
	for (int32_t i = 0; i < it->count; ++i)
	{
		printf ("%i, ", vao[i]);
	}
	printf ("\n");
}


static void trigger_mesh_vbo_onadd (ecs_iter_t *it)
{
	printf ("[ECS_TRIGGER] trigger_mesh_vbo_onadd: ");
	ECS_COLUMN (it, component_mesh, mesh, 1);
	for (int32_t i = 0; i < it->count; ++i)
	{
		glGenBuffers (1, &mesh[i].vbop);
		glGenBuffers (1, &mesh[i].vbot);
		glBindBuffer (GL_ARRAY_BUFFER, mesh[i].vbop);
		glBindBuffer (GL_ARRAY_BUFFER, mesh[i].vbot);
	}
	printf ("\n");
}


static void trigger_transform (ecs_iter_t *it)
{
	printf ("[ECS_TRIGGER] trigger_transform_onadd: ");
	ECS_COLUMN (it, component_transform, t, 1);
	for (int32_t i = 0; i < it->count; ++i)
	{
		m4f32_identity (t[i]);
	}
	printf ("\n");
}


static void fill_texture (uint8_t * data, int w, int h, uint32_t c, uint32_t n)
{
	memset (data, 0, w * h * c);
	/*
	for (int x = 0; x < width; ++x)
	for (int y = 0; y < height; ++y)
	{
		uint8_t * p = data + (x*4) + (y*width*4);
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		p[3] = 0;
		p[index] = 255;
	}
	*/
	//ASSERT (index < channels);
	int x = w/2;
	int y = h/2;
	int dx = 0;
	int dy = 0;
	for (uint32_t i = 0; i < n; ++i)
	{
		x += dx;
		y += dy;
		dx += (rand() % 3) - 1;
		dy += (rand() % 3) - 1;
		dx = CLAMP (dx, -4, 4);
		dy = CLAMP (dy, -4, 4);
		if (x < 0 || x >= w){dx = -dx/2;}
		if (y < 0 || y >= h){dy = -dy/2;}
		x = CLAMP (x, 0, w-1);
		y = CLAMP (y, 0, h-1);
		int i = (x*c) + (y*w*c);
		ASSERT (i >= 0);
		data[i + 0] = 255;
	}
}


static void system_texture_onset (ecs_iter_t *it)
{
	ECS_COLUMN (it, component_texture, texure, 1);
	ECS_COLUMN (it, component_gl_tex2darray, tex, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
		if (glIsTexture (tex[i]))
		{
			printf ("[ECS_SYSTEM] component_texture_onadd tex (%i) redefining\n", tex[i]);
			glDeleteTextures (1, tex + i);
			//continue;
		}
		else
		{
			printf ("[ECS_SYSTEM] component_texture_onadd tex (%i)\n", tex[i]);
		}
		uint32_t width = texure[i].width;
		uint32_t height = texure[i].height;
		uint32_t depth = texure[i].depth;
		uint32_t channels = 4;
		unsigned size = width * height * depth * channels * sizeof(uint8_t);
		uint8_t * data = calloc (size, 1);
		glActiveTexture (GL_TEXTURE0 + texure[i].unit);
		glBindTexture (GL_TEXTURE_2D_ARRAY, tex[i]);//Depends on glActiveTexture()
		glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//Depends on glBindTexture()
		glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//Depends on glBindTexture()
		glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//Depends on glBindTexture()
		glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//Depends on glBindTexture()
		glTexStorage3D (GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, depth);//Depends on glBindTexture()
		//fill_texture (data, width, height, channels, 0);
		//fill_texture2 (data, width, height, channels, 255);
		for (uint32_t j = 0; j < depth; ++j)
		{
			fill_texture (data + (j%3), width, height, channels, 200);
			glTexSubImage3D (GL_TEXTURE_2D_ARRAY, 0, 0, 0, j, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);//Depends on glBindTexture()
		}
	}
}


static void system_apply_rotation (ecs_iter_t *it)
{
	ECS_COLUMN (it, component_quaternion, q, 1);
	ECS_COLUMN (it, component_controller, c, 2);//Singleton
	for (int32_t i = 0; i < it->count; ++i)
	{
		qf32_rotate2_xyza (q[i], c->keyboard[SDL_SCANCODE_1], c->keyboard[SDL_SCANCODE_2], c->keyboard[SDL_SCANCODE_3], 0.01f);
	}
}


static void system_pointcloud_set (ecs_iter_t *it)
{
	printf ("[ECS_SYSTEM] system_pointcloud_set\n");
	ECS_COLUMN (it, component_pointcloud, pc, 1);
	ECS_COLUMN (it, component_count, c, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
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
	ECS_COLUMN (it, component_count, c, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
		glBindVertexArray (pc[i].vao);
		glUseProgram (global_glprogram[GLPROGRAM_POINT]);
		glUniformMatrix4fv (global_gluniform[GLUNIFORM_POINT_MVP], 1, GL_FALSE, (const GLfloat *) global_gcam.mvp);
		glDrawArrays (GL_POINTS, 0, c[i]);
	}
}


static void system_mesh_set_rectangle (ecs_iter_t *it)
{
	printf ("[ECS_SYSTEM] system_mesh_set_rectangle\n");
	ECS_COLUMN (it, component_mesh, img, 1);
	ECS_COLUMN (it, component_count, count, 2);
	ECS_COLUMN (it, component_rectangle, wh, 3);
	for (int32_t i = 0; i < it->count; ++i)
	{
		ASSERT (count[i] == 6);
		//printf ("[ECS_SYSTEM] system_mesh_set_rectangle %i:%i\n", img[i].vbop, count[i]);
		glBindBuffer (GL_ARRAY_BUFFER, img[i].vbop);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (component_position), NULL, GL_DYNAMIC_DRAW);
		component_position p[6];
		csc_gl_make_rectangle_pos ((void*)p, 0.0f, 0.0f, 0.0f, 0.0f, wh[i][0], wh[i][1], 4);
		glBufferSubData (GL_ARRAY_BUFFER, 0, 6 * sizeof (component_position), p);
		//printf ("[ECS_SYSTEM] system_mesh_set_rectangle %i:%i\n", img[i].vbot, count[i]);
		glBindBuffer (GL_ARRAY_BUFFER, img[i].vbot);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (component_uv), NULL, GL_DYNAMIC_DRAW);
		component_uv uv[6];
		csc_gl_make_rectangle_uv ((void*)uv, 1, 2);
		glBufferSubData (GL_ARRAY_BUFFER, 0, 6 * sizeof (component_uv), uv);
	}
}


static void system_mesh_set (ecs_iter_t *it)
{
	printf ("[ECS_SYSTEM] system_mesh_set\n");
	ECS_COLUMN (it, component_mesh, mesh, 1);
	ECS_COLUMN (it, component_count, count, 2);
	ECS_COLUMN (it, component_vao, vao, 3);
	for (int32_t i = 0; i < it->count; ++i)
	{
		glBindVertexArray (vao[i]);

		ASSERT (glIsBuffer(mesh[i].vbop) == GL_TRUE);
		ASSERT (glIsBuffer(mesh[i].vbot) == GL_TRUE);

		glEnableVertexAttribArray (0);
		glEnableVertexAttribArray (1);

		glBindBuffer (GL_ARRAY_BUFFER, mesh[i].vbop);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (component_position), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)0);

		glBindBuffer (GL_ARRAY_BUFFER, mesh[i].vbot);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (component_uv), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)0);
	}
}


static void system_mesh_draw (ecs_iter_t *it)
{
	ECS_COLUMN (it, component_mesh, img, 1);//Shared
	ECS_COLUMN (it, component_count, count, 2);//Shared
	ECS_COLUMN (it, component_vao, vao, 3);//Shared
	ECS_COLUMN (it, component_gl_tex2darray, tbo, 4);//Shared
	ECS_COLUMN (it, component_position, p, 5);
	ECS_COLUMN (it, component_scale, s, 6);
	ECS_COLUMN (it, component_quaternion, q, 7);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D_ARRAY, tbo[0]);
	glBindVertexArray (vao[0]);
	glUseProgram (global_glprogram[GLPROGRAM_MESH]);
	glUniform1i (global_gluniform[GLUNIFORM_MESH_TEX0], 0);

	for (int32_t i = 0; i < it->count; ++i)
	{
		float m[4*4];
		float ms[4*4];
		float mt[4*4];
		float mr[4*4];
		m4f32_identity (m);
		m4f32_identity (ms);
		m4f32_identity (mr);
		m4f32_identity (mt);
		m4f32_scale (ms, s[i]);
		qf32_m4 (mr, q[i]);
		m4f32_translation (mt, p[i]);
		m4f32_mul (m, ms, m); //Apply scale
		m4f32_mul (m, mr, m); //Apply rotation
		m4f32_mul (m, mt, m); //Apply translation
		//m4f32_print (mt, stdout);
		m4f32_mul (m, global_gcam.mvp, m);
		glUniformMatrix4fv (global_gluniform[GLUNIFORM_MESH_MVP], 1, GL_FALSE, (const GLfloat *) m);
		glDrawArrays (GL_TRIANGLES, 0, count[0]);
	}
}


static void system_mesh_draw1 (ecs_iter_t *it)
{
	ECS_COLUMN (it, component_mesh, img, 1);//Shared
	ECS_COLUMN (it, component_count, count, 2);//Shared
	ECS_COLUMN (it, component_vao, vao, 3);//Shared
	ECS_COLUMN (it, component_gl_tex2darray, tbo, 4);//Shared
	ECS_COLUMN (it, component_transform, t, 5);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D_ARRAY, tbo[0]);
	glBindVertexArray (vao[0]);
	glUseProgram (global_glprogram[GLPROGRAM_MESH]);
	glUniform1i (global_gluniform[GLUNIFORM_MESH_TEX0], 0);

	for (int32_t i = 0; i < it->count; ++i)
	{
		float m[4*4];
		//m4f32_print (mt, stdout);
		m4f32_mul (m, global_gcam.mvp, t[i]);
		glUniformMatrix4fv (global_gluniform[GLUNIFORM_MESH_MVP], 1, GL_FALSE, (const GLfloat *) m);
		glDrawArrays (GL_TRIANGLES, 0, count[0]);
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
		float * m = t[i];
		float ms[4*4];
		float mt[4*4];
		float mr[4*4];
		m4f32_identity (m);
		m4f32_identity (ms);
		m4f32_identity (mr);
		m4f32_identity (mt);
		m4f32_scale (ms, s[i]);
		qf32_m4 (mr, q[i]);
		m4f32_translation (mt, p[i]);
		m4f32_mul (m, ms, m); //Apply scale
		m4f32_mul (m, mr, m); //Apply rotation
		m4f32_mul (m, mt, m); //Apply translation
	}
}


static void systems_init (ecs_world_t * world)
{
	srand (1);

	ECS_COMPONENT_DEFINE (world, component_gl_tex2darray);
	ECS_COMPONENT_DEFINE (world, component_texture);
	ECS_COMPONENT_DEFINE (world, component_color);
	ECS_COMPONENT_DEFINE (world, component_position);
	ECS_COMPONENT_DEFINE (world, component_scale);
	ECS_COMPONENT_DEFINE (world, component_quaternion);
	ECS_COMPONENT_DEFINE (world, component_applyrotation);
	ECS_COMPONENT_DEFINE (world, component_uv);
	ECS_COMPONENT_DEFINE (world, component_rectangle);
	ECS_COMPONENT_DEFINE (world, component_controller);
	ECS_COMPONENT_DEFINE (world, component_va);
	ECS_COMPONENT_DEFINE (world, component_vao);
	ECS_COMPONENT_DEFINE (world, component_vbo);
	ECS_COMPONENT_DEFINE (world, component_count);
	ECS_COMPONENT_DEFINE (world, component_stride);
	ECS_COMPONENT_DEFINE (world, component_pointcloud);
	ECS_COMPONENT_DEFINE (world, component_mesh);
	ECS_COMPONENT_DEFINE (world, component_transform);

	ECS_TRIGGER (world, trigger_gl_tex2darray_onadd, EcsOnAdd, component_gl_tex2darray);
	ECS_TRIGGER (world, trigger_vao_onadd, EcsOnAdd, component_vao);
	ECS_TRIGGER (world, trigger_mesh_vbo_onadd, EcsOnAdd, component_mesh);
	ECS_TRIGGER (world, trigger_transform, EcsOnAdd, component_transform);

	ECS_SYSTEM (world, system_mesh_set, EcsOnSet, component_mesh, component_count, component_vao);
	ECS_SYSTEM (world, system_mesh_set_rectangle, EcsOnSet, component_mesh, component_count, component_rectangle);
	//ECS_SYSTEM (world, system_mesh_draw, EcsOnUpdate, SHARED:component_mesh, SHARED:component_count, SHARED:component_vao, SHARED:component_gl_tex2darray, component_position, component_scale, component_quaternion);
	ECS_SYSTEM (world, system_mesh_draw1, EcsOnUpdate, SHARED:component_mesh, SHARED:component_count, SHARED:component_vao, SHARED:component_gl_tex2darray, component_transform);
	ECS_SYSTEM (world, system_transform_onset, EcsOnSet, component_position, component_scale, component_quaternion, component_transform);

	ECS_SYSTEM (world, system_apply_rotation, EcsOnUpdate, component_quaternion, $component_controller);
	//ECS_SYSTEM (world, component_tbo_onadd, EcsMonitor, component_tbo);
	ECS_SYSTEM (world, system_texture_onset, EcsOnSet, component_texture, component_gl_tex2darray);
	ECS_SYSTEM (world, system_pointcloud_set, EcsOnSet, component_pointcloud, component_count);
	ECS_SYSTEM (world, system_pointcloud_draw, EcsOnUpdate, component_pointcloud, component_count);

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
	v4f32_set_xyzw (global_gcam.p, 0.0f, 0.0f, -1.0f, 1.0f);


}






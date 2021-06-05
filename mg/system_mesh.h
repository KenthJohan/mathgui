#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"


#include "mg_comp.h"
#include "mg_compdef.h"
#include "systems.h"





static void system_mesh_set_rectangle (ecs_iter_t *it)
{
	XLOG (XLOG_INF, XLOG_ECS_ONSET, "Set rectangles n=%i", it->count);
	Mesh_OpenGL * img = ecs_term(it, Mesh_OpenGL, 1);
	Capacity * count = ecs_term(it, Capacity, 2);
	Rectangle2f * wh = ecs_term(it, Rectangle2f, 3);
	for (int32_t i = 0; i < it->count; ++i)
	{
		ASSERT (count[i] == 6);
		//printf ("[ECS_SYSTEM] system_mesh_set_rectangle %i:%i\n", img[i].vbop, count[i]);
		glBindBuffer (GL_ARRAY_BUFFER, img[i].vbop);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (Position4), NULL, GL_DYNAMIC_DRAW);
		Position4 p[6];
		csc_gl_make_rectangle_pos ((void*)p, 0.0f, 0.0f, 0.0f, 0.0f, wh[i].x, wh[i].y, 4);
		glBufferSubData (GL_ARRAY_BUFFER, 0, 6 * sizeof (Position4), p);
		//printf ("[ECS_SYSTEM] system_mesh_set_rectangle %i:%i\n", img[i].vbot, count[i]);
		glBindBuffer (GL_ARRAY_BUFFER, img[i].vbot);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (TextureUV), NULL, GL_DYNAMIC_DRAW);
		TextureUV uv[6];
		csc_gl_make_rectangle_uv ((void*)uv, 1, 2);
		glBufferSubData (GL_ARRAY_BUFFER, 0, 6 * sizeof (TextureUV), uv);
	}
}


static void system_mesh_set (ecs_iter_t *it)
{
	XLOG (XLOG_INF, XLOG_ECS_ONSET, "Set meshes n=%i", it->count);
	ECS_COLUMN (it, Mesh_OpenGL, mesh, 1);
	ECS_COLUMN (it, Capacity, count, 2);
	ECS_COLUMN (it, VAO_OpenGL, vao, 3);
	for (int32_t i = 0; i < it->count; ++i)
	{
		glBindVertexArray (vao[i]);

		ASSERT (glIsBuffer(mesh[i].vbop) == GL_TRUE);
		ASSERT (glIsBuffer(mesh[i].vbot) == GL_TRUE);

		glEnableVertexAttribArray (0);
		glEnableVertexAttribArray (1);

		glBindBuffer (GL_ARRAY_BUFFER, mesh[i].vbop);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (Position4), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)0);

		glBindBuffer (GL_ARRAY_BUFFER, mesh[i].vbot);
		glBufferData (GL_ARRAY_BUFFER, count[i] * sizeof (TextureUV), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)0);
	}
}


static void system_mesh_draw (ecs_iter_t *it)
{
	ECS_COLUMN (it, Mesh_OpenGL, img, 1);//Shared
	ECS_COLUMN (it, Capacity, count, 2);//Shared
	ECS_COLUMN (it, VAO_OpenGL, vao, 3);//Shared
	ECS_COLUMN (it, Tex2DArray_OpenGL, tbo, 4);//Shared
	ECS_COLUMN (it, Position4, p, 5);
	ECS_COLUMN (it, Scale4, s, 6);
	ECS_COLUMN (it, Quaternion, q, 7);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D_ARRAY, tbo[0]);
	glBindVertexArray (vao[0]);
	glUseProgram (global_glprogram[GLPROGRAM_MESH]);
	glUniform1i (global_gluniform[GLUNIFORM_MESH_TEX0], 0);

	for (int32_t i = 0; i < it->count; ++i)
	{
		m4f32 m;
		m4f32 ms;
		m4f32 mt;
		m4f32 mr;
		m4f32_identity (&m);
		m4f32_identity (&ms);
		m4f32_identity (&mr);
		m4f32_identity (&mt);
		m4f32_scale (&ms, s + i);
		qf32_m4 (&mr, q + i);
		m4f32_translation4 (&mt, p + i);
		m4f32_mul (&m, &ms, &m); //Apply scale
		m4f32_mul (&m, &mr, &m); //Apply rotation
		m4f32_mul (&m, &mt, &m); //Apply translation
		//m4f32_print (mt, stdout);
		m4f32_mul (&m, &global_gcam.mvp, &m);
		glUniformMatrix4fv (global_gluniform[GLUNIFORM_MESH_MVP], 1, GL_FALSE, (const GLfloat *) &m);
		glDrawArrays (GL_TRIANGLES, 0, count[0]);
	}
}


static void system_mesh_draw1 (ecs_iter_t *it)
{
	ECS_COLUMN (it, Mesh_OpenGL, img, 1);//Shared
	ECS_COLUMN (it, Capacity, count, 2);//Shared
	ECS_COLUMN (it, VAO_OpenGL, vao, 3);//Shared
	ECS_COLUMN (it, Tex2DArray_OpenGL, tbo, 4);//Shared
	ECS_COLUMN (it, Transform, t, 5);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D_ARRAY, tbo[0]);
	glBindVertexArray (vao[0]);
	glUseProgram (global_glprogram[GLPROGRAM_MESH]);
	glUniform1i (global_gluniform[GLUNIFORM_MESH_TEX0], 0);

	for (int32_t i = 0; i < it->count; ++i)
	{
		m4f32 m;
		//m4f32_print (mt, stdout);
		m4f32_mul (&m, &global_gcam.mvp, t + i);
		glUniformMatrix4fv (global_gluniform[GLUNIFORM_MESH_MVP], 1, GL_FALSE, (const GLfloat *) &m);
		glDrawArrays (GL_TRIANGLES, 0, count[0]);
	}
}


static void trigger_mesh_vbo_onadd (ecs_iter_t *it)
{
	XLOG (XLOG_INF, XLOG_ECS_ONADD, "glGenBuffers and glBindBuffer n=%i", it->count);
	ECS_COLUMN (it, Mesh_OpenGL, mesh, 1);
	for (int32_t i = 0; i < it->count; ++i)
	{
		glGenBuffers (1, &mesh[i].vbop);
		glGenBuffers (1, &mesh[i].vbot);
		glBindBuffer (GL_ARRAY_BUFFER, mesh[i].vbop);
		glBindBuffer (GL_ARRAY_BUFFER, mesh[i].vbot);
	}
	//printf ("\n");
}


static void system_mesh_init (ecs_world_t * world)
{
	ECS_TRIGGER (world, trigger_mesh_vbo_onadd, EcsOnAdd, Mesh_OpenGL);
	ECS_SYSTEM (world, system_mesh_set, EcsOnSet, Mesh_OpenGL, Capacity, VAO_OpenGL);
	ECS_SYSTEM (world, system_mesh_set_rectangle, EcsOnSet, Mesh_OpenGL, Capacity, Rectangle2f);
	ECS_SYSTEM (world, system_mesh_draw1, EcsOnUpdate, SHARED:Mesh_OpenGL, SHARED:Capacity, SHARED:VAO_OpenGL, SHARED:Tex2DArray_OpenGL, Transform);
	//ECS_SYSTEM (world, system_mesh_draw, EcsOnUpdate, SHARED:component_mesh, SHARED:Count, SHARED:component_vao, SHARED:Tex2DArray_OpenGL, component_position, component_scale, component_quaternion);
}






#pragma once

#include <flecs.h>
#include <GL/glew.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"


#include "mg_comp.h"
#include "systems.h"



ECS_COMPONENT_DECLARE (component_texture);


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


static void system_texture_init (ecs_world_t * world)
{
	ECS_COMPONENT_DEFINE (world, component_texture);
	ECS_SYSTEM (world, system_texture_onset, EcsOnSet, component_texture, component_gl_tex2darray);
}






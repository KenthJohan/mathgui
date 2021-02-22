#pragma once

#include "csc/csc_math.h"
#include <flecs.h>
#include <GL/glew.h>

typedef GLuint component_tbo;

typedef struct component_texture
{
	uint32_t unit;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
} component_texture;


ECS_COMPONENT_DECLARE (component_tbo);
ECS_COMPONENT_DECLARE (component_texture);


static void component_tbo_onadd (ecs_iter_t *it)
{
	printf ("component_tbo_onadd: ");
	ECS_COLUMN (it, component_tbo, t, 1);
	glGenTextures (it->count, t);
	for (int32_t i = 0; i < it->count; ++i)
	{
		printf ("%i, ", t[i]);
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


static void component_texture_onset (ecs_iter_t *it)
{
	printf ("component_texture_onadd\n");
	ECS_COLUMN (it, component_texture, tex, 1);
	ECS_COLUMN (it, component_tbo, tbo, 2);
	for (int32_t i = 0; i < it->count; ++i)
	{
		uint32_t width = tex[i].width;
		uint32_t height = tex[i].height;
		uint32_t depth = tex[i].depth;
		uint32_t channels = 4;
		unsigned size = width * height * depth * channels * sizeof(uint8_t);
		uint8_t * data = calloc (size, 1);
		glActiveTexture (GL_TEXTURE0 + tex[i].unit);
		glBindTexture (GL_TEXTURE_2D_ARRAY, tbo[i]);//Depends on glActiveTexture()
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


static void component_tbo_init (ecs_world_t * world)
{
	printf ("component_tbo_init\n");
	srand (1);
	ECS_COMPONENT_DEFINE (world, component_tbo);
	ECS_COMPONENT_DEFINE (world, component_texture);
	ECS_TRIGGER (world, component_tbo_onadd, EcsOnAdd, component_tbo);
	//ECS_SYSTEM (world, component_tbo_onadd, EcsMonitor, component_tbo);
	ECS_SYSTEM (world, component_texture_onset, EcsOnSet, component_texture, component_tbo);
}




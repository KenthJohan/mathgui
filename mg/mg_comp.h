#pragma once

#include "csc/csc_math.h"
#include "csc/csc_qf32.h"


typedef uint32_t component_color;
typedef v4f32 component_position;
typedef v4f32 component_scale;
typedef qf32 component_quaternion;
typedef v4f32 component_applyrotation;
typedef v2f32 component_uv;
typedef v2f32 component_rectangle;
typedef m4f32 component_transform;
typedef struct component_texture
{
	uint32_t unit;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
} component_texture;
typedef struct component_pointcloud
{
	uint32_t vao;
	uint32_t vboc;
	uint32_t vbop;
} component_pointcloud;
typedef struct component_mesh
{
	uint32_t vbop;
	uint32_t vbot;
} component_mesh;
typedef struct component_lines
{
	uint32_t vao;
	uint32_t vbop;
	uint32_t vboc;
} component_lines;
typedef uint32_t component_stride;
typedef uint32_t component_count;
typedef char component_filename[256];



//SDL:
typedef struct component_controller
{
	const uint8_t * keyboard; //SDL Keyboard
} component_controller;

//OpenGL:
typedef uint32_t component_vbo;
typedef uint32_t component_va;
typedef uint32_t component_vao;
typedef uint32_t component_gl_tex2darray;
typedef int32_t component_gl_program;
typedef int32_t component_gl_shader;

















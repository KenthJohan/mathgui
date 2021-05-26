#pragma once

#include "csc/csc_math.h"
#include "csc/csc_qf32.h"

//Spatial
typedef uint32_t Color;
typedef v4f32 Position4;
typedef v4f32 Scale4;
typedef qf32 Quaternion;
typedef qf32 QuaternionDelta;
typedef m4f32 Transform;

//Geometry
typedef v2f32 TextureUV;
typedef v2f32 Rectangle2f;


//OpenGL
typedef struct Texture_OpenGL
{
	uint32_t unit;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
} Texture_OpenGL;

typedef struct Pointcloud_OpenGL
{
	uint32_t vao;
	uint32_t vboc;
	uint32_t vbop;
} Pointcloud_OpenGL;

typedef struct Mesh_OpenGL
{
	uint32_t vbop;
	uint32_t vbot;
} Mesh_OpenGL;

typedef struct Lines_OpenGL
{
	uint32_t vao;
	uint32_t vbop;
	uint32_t vboc;
} Lines_OpenGL;

typedef uint32_t VBO_OpenGL;
typedef uint32_t VA_OpenGL;
typedef uint32_t VAO_OpenGL;
typedef uint32_t Tex2DArray_OpenGL;
typedef int32_t Program_OpenGL;
typedef int32_t Shader_OpenGL;


//Array
typedef uint32_t Stride;
typedef uint32_t Capacity;
typedef uint32_t Count;
typedef uint32_t Offset;
typedef char Filename[256];



//SDL:
typedef struct Keyboard_SDL
{
	const uint8_t * keyboard; //SDL Keyboard
} Keyboard_SDL;



















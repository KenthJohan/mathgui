#pragma once

#include "csc/csc_math.h"
#include "csc/csc_qf32.h"

//Spatial
typedef uint32_t Color;
typedef v3f32 Position3;
typedef v4f32 Position4;
typedef v4f32 Scale4;
typedef qf32 Quaternion;
typedef qf32 QuaternionDelta;
typedef m4f32 Transform;

//Geometry
typedef v2f32 TextureUV;
typedef v2f32 Rectangle2f;


//OpenGL
typedef struct GL_Texture
{
	uint32_t unit;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
} GL_Texture;

typedef struct GL_Pointcloud
{
	uint32_t vao;
	uint32_t vboc;
	uint32_t vbop;
} GL_Pointcloud;

typedef struct GL_Mesh
{
	uint32_t vbop;
	uint32_t vbot;
} GL_Mesh;

typedef struct GL_Lines
{
	uint32_t vao;
	uint32_t vbop;
	uint32_t vboc;
} GL_Lines;

typedef uint32_t GL_VertexBufferObject;
typedef uint32_t GL_VertexArrayObject;
typedef uint32_t GL_Tex2DArray;//glGenTextures, GL_TEXTURE_2D_ARRAY
typedef int32_t GL_Program;
typedef int32_t GL_Shader;


//Array
typedef uint32_t Stride;
typedef uint32_t Capacity;
typedef uint32_t Count;
typedef uint32_t Offset;
typedef char Filename[256];
typedef struct Text
{
	char const * value;
} Text;



//SDL:
typedef struct SDL_Keyboard
{
	const uint8_t * keyboard; //SDL Keyboard
} SDL_Keyboard;

















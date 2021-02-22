#pragma once

#include <flecs.h>

#include "csc/csc_math.h"
#include "csc/csc_gl.h"
#include "csc/csc_gcam.h"
#include "csc/csc_qf32.h"


typedef uint32_t component_tbo;
typedef uint32_t component_color;
typedef v4f32 component_position;
typedef v4f32 component_scale;
typedef qf32 component_quaternion;
typedef v4f32 component_applyrotation;
typedef v2f32 component_uv;
typedef v2f32 component_rectangle;
typedef struct component_controller
{
	const uint8_t * keyboard; //SDL Keyboard
} component_controller;
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
typedef uint32_t component_vbo;
typedef uint32_t component_va;
typedef uint32_t component_stride;
typedef uint32_t component_count;
typedef uint32_t component_vao;


//OpenGL:
ECS_COMPONENT_DECLARE (component_tbo);
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

#define MYGL_MAX_POINTS 10000
#define MYGL_MAX_TRIANGLES 10000
#define MYGL_MAX_IMGS 100





enum myattr
{
	ATTR_COUNT,
	ATTR_POINTCLOUD,
	ATTR_POINTCLOUD_POS,
	ATTR_POINTCLOUD_COL,
	ATTR_MESH,
	ATTR_ADD_INSTANCEOF,
	ATTR_TEXTURE,
	ATTR_SCALE,
	ATTR_QUATERNION,
	ATTR_POSITION,
	ATTR_RECTANGLE,
};

struct mynet_eav
{
	uint32_t entity;
	uint32_t attribute;
	uint8_t value[0];
};

struct mynet_eav_u32
{
	uint32_t entity;
	uint32_t attribute;
	uint32_t value;
};

struct mynet_eav_component_texture
{
	uint32_t entity;
	uint32_t attribute;
	component_texture value;
};

struct mynet_eav_component_rectangle
{
	uint32_t entity;
	uint32_t attribute;
	component_rectangle value;
};

struct mynet_eav_component_scale
{
	uint32_t entity;
	uint32_t attribute;
	component_scale value;
};

struct mynet_eav_component_quaternion
{
	uint32_t entity;
	uint32_t attribute;
	component_quaternion value;
};

struct mynet_eav_component_position
{
	uint32_t entity;
	uint32_t attribute;
	component_position value;
};















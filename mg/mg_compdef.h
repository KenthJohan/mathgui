#pragma once

#include <flecs.h>
#include "mg_comp.h"

#include "csc/csc_xlog.h"


ECS_COMPONENT_DECLARE (Stride);
ECS_COMPONENT_DECLARE (Capacity);
ECS_COMPONENT_DECLARE (Count);
ECS_COMPONENT_DECLARE (Offset);
ECS_COMPONENT_DECLARE (Text);

ECS_COMPONENT_DECLARE (Color);
ECS_COMPONENT_DECLARE (Position4);
ECS_COMPONENT_DECLARE (Position3);
ECS_COMPONENT_DECLARE (Scale4);
ECS_COMPONENT_DECLARE (Scale3);
ECS_COMPONENT_DECLARE (Scale2);
ECS_COMPONENT_DECLARE (Quaternion);
ECS_COMPONENT_DECLARE (TextureUV);
ECS_COMPONENT_DECLARE (Rectangle2f);
ECS_COMPONENT_DECLARE (Transform);
ECS_COMPONENT_DECLARE (Filename);
ECS_COMPONENT_DECLARE (QuaternionDelta);




//SDL
ECS_COMPONENT_DECLARE (SDL_Keyboard);

//OpenGL
ECS_TAG_DECLARE (GL_ProgramLinked);
ECS_COMPONENT_DECLARE (GL_Program);
ECS_COMPONENT_DECLARE (GL_Shader);
ECS_COMPONENT_DECLARE (GL_Tex2DArray);
ECS_COMPONENT_DECLARE (GL_VertexBufferObject);
ECS_COMPONENT_DECLARE (GL_VertexArrayObject);

ECS_COMPONENT_DECLARE (GL_Lines);
ECS_COMPONENT_DECLARE (GL_Texture);
ECS_COMPONENT_DECLARE (GL_Pointcloud);
ECS_COMPONENT_DECLARE (GL_Mesh);

















// Component constructor
ECS_CTOR(Text, ptr, {
//XLOG (XLOG_INF, XLOG_ECS, "ptr: %s", ptr->value);
ptr->value = NULL;
});

// Component destructor
ECS_DTOR(Text, ptr, {
XLOG (XLOG_INF, XLOG_ECS, "ptr: %s", ptr->value);
free((void*)ptr->value);
});

// Component copy
ECS_COPY(Text, dst, src, {
XLOG (XLOG_INF, XLOG_ECS, "dst: %s src: %s", dst->value, src->value);
free((void*)dst->value);
dst->value = strdup(src->value);
});

// Component move
ECS_MOVE(Text, dst, src, {
XLOG (XLOG_INF, XLOG_ECS, "dst: %s src: %s", dst->value, src->value);
dst->value = src->value;
src->value = NULL;
});

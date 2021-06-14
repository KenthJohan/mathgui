#include "csc/csc_crossos.h"
#include "csc/csc_basic.h"
#include "csc/csc_debug.h"
#include "csc/csc_malloc_file.h"
#include "csc/csc_sdl_motion.h"
#include "csc/csc_gcam.h"
#include "csc/csc_gl.h"
#include "csc/csc_math.h"
#include "csc/csc_sdlglew.h"
#include "csc/csc_xlog.h"

//#include "api.h"

#include <flecs.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>




#include "mg_comp.h"
#include "mg_compdef.h"

#include "systems.h"
#include "system_opengl.h"
#include "system_mesh.h"
#include "system_pointcloud.h"
#include "system_lines.h"
#include "system_texture.h"
#include "system_camera.h"

#include "mg_attr.h"
#include "eavnet.h"
#include "eavnet_recv.h"


#define WIN_X SDL_WINDOWPOS_UNDEFINED
#define WIN_Y SDL_WINDOWPOS_UNDEFINED
#define WIN_W 640
#define WIN_H 480
#define WIN_TITLE "Mattegrafik v0.4"







static void test_ecs_addents (ecs_world_t * world)
{

	ECS_ENTITY (world, mytexture1, GL_Tex2DArray);
	ECS_ENTITY (world, mytexture2, GL_Tex2DArray);
	ECS_ENTITY (world, mytexture3, GL_Tex2DArray);
	ecs_set (world, mytexture1, GL_Texture, {.unit = 0, .width = 100, .height = 100, .depth = 4});
	ecs_set (world, mytexture2, GL_Texture, {.unit = 0, .width = 50, .height = 50, .depth = 4});
	ecs_set (world, mytexture3, GL_Texture, {.unit = 0, .width = 200, .height = 200, .depth = 4});

	ECS_ENTITY (world, img, GL_Mesh, GL_VertexArrayObject);
	ecs_set (world, img, Capacity, {6});
	ecs_set (world, img, Rectangle2f, {{1.0f, 1.0f}});

	ecs_entity_t const * e2 = ecs_bulk_new (world, Transform, 4);
	ecs_add_entity (world, e2[0], ECS_INSTANCEOF | mytexture1);
	ecs_add_entity (world, e2[1], ECS_INSTANCEOF | mytexture1);
	ecs_add_entity (world, e2[2], ECS_INSTANCEOF | mytexture2);
	ecs_add_entity (world, e2[3], ECS_INSTANCEOF | mytexture3);

	ecs_add_entity (world, e2[0], ECS_INSTANCEOF | img);
	ecs_add_entity (world, e2[1], ECS_INSTANCEOF | img);
	ecs_add_entity (world, e2[2], ECS_INSTANCEOF | img);
	ecs_add_entity (world, e2[3], ECS_INSTANCEOF | img);
	for (int i = 0; i < 4; ++i)
	{
		ecs_set (world, e2[i], Scale4, {{(float)(i+1)/10.0f, (float)(i+1)/10.0f, 1.0f, 1.0f}});
		ecs_set (world, e2[i], Quaternion, {{1.0f, 0.0f, 0.0f, 0.0f}});
		ecs_set (world, e2[i], Position4, {{0.0f, 0.0f, (float)i/2.0f, 0.0f}});
	}
}


static void test_eavnet (struct eavnet_context * ctx)
{

	enum myent
	{
		MYENT_MESH_RECTANGLE,
		MYENT_TEXTURE1,
		MYENT_TEXTURE2,

		MYENT_DRAW_CLOUD,
		MYENT_DRAW_IMG1,
		MYENT_DRAW_IMG2,
	};

	{
		uint32_t count = 1000;
		eavnet_receiver (ctx, MYENT_DRAW_CLOUD, MG_POINTCLOUD, NULL, 0);
		eavnet_receiver (ctx, MYENT_DRAW_CLOUD, MG_CAPACITY, &(Capacity){count}, 0);
		uint32_t size = count * sizeof (Position4);
		Position4 * p = malloc (size);
		for (uint32_t i = 0; i < count; ++i)
		{
			p[i].x = 10.0f * (float)i / rand();
			p[i].y = 1.0f * (float)i / rand();
			p[i].z = 10.0f * (float)i / rand();
			p[i].w = 100.0f;
		}
		eavnet_receiver (ctx, MYENT_DRAW_CLOUD, MG_POINTCLOUD_POS, p, size);
		free (p);
	}

	eavnet_receiver (ctx, MYENT_TEXTURE1, MG_TEXTURE, &(GL_Texture){0, 100, 100, 1}, 0);
	eavnet_receiver (ctx, MYENT_TEXTURE2, MG_TEXTURE, &(GL_Texture){0, 300, 300, 1}, 0);
	eavnet_receiver (ctx, MYENT_MESH_RECTANGLE, MG_MESH, NULL, 0);
	eavnet_receiver (ctx, MYENT_MESH_RECTANGLE, MG_CAPACITY, &(Capacity){6}, 0);
	eavnet_receiver (ctx, MYENT_MESH_RECTANGLE, MG_RECTANGLE, &(Rectangle2f){{1.0f, 1.0f}}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG1, MG_POSITION,&(Position4){{3.0f, 1.0f, 0.0f, 1.0f}}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG1, MG_SCALE, &(Position4){{0.3f, 0.3f, 0.0f, 1.0f}}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG1, MG_QUATERNION, &(Position4){{0.0f, 0.0f, 0.0f, 1.0f}}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG1, MG_ADD_INSTANCEOF, &(uint32_t){MYENT_MESH_RECTANGLE}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG1, MG_ADD_INSTANCEOF, &(uint32_t){MYENT_TEXTURE1}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG2, MG_POSITION, &(Position4){{4.0f, 1.0f, 0.0f, 1.0f}}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG2, MG_SCALE, &(Position4){{0.3f, 0.3f, 0.0f, 1.0f}}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG2, MG_QUATERNION, &(Position4){{0.0f, 0.0f, 0.0f, 1.0f}}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG2, MG_ADD_INSTANCEOF, &(uint32_t){MYENT_MESH_RECTANGLE}, 0);
	eavnet_receiver (ctx, MYENT_DRAW_IMG2, MG_ADD_INSTANCEOF, &(uint32_t){MYENT_TEXTURE2}, 0);
	//mynet_send_ptr(NULL, 0, 0, &(component_position){1.0f, 2.0f, 3.0f, 1.0f}, sizeof (component_position));
}


static void test_ecs_onset (ecs_world_t * world)
{
	ecs_entity_t e =  ecs_new (world, 0);
	ecs_add (world, e, Transform);
	ecs_add (world, e, Quaternion);
	ecs_add (world, e, Position4);
	ecs_add (world, e, Scale4);


	ecs_set (world, e, Scale4, {{1.0f, 2.0f, 1.0f, 1.0f}});
}


static void test_ecs_addlines (ecs_world_t * world)
{
	XLOG (XLOG_INF, XLOG_ECS, "Adding origin. (x,y,z) = (red, green, blue)");
	ecs_entity_t e = ecs_new (world, 0);
	ecs_add (world, e, GL_Lines);
	ecs_set (world, e, Capacity, {6});
	ecs_set (world, e, Offset, {0});
	ecs_set (world, e, Count, {6});
	GL_Lines const * lines = ecs_get (world, e, GL_Lines);

	Position4 p[6] =
	{
	{{0.0f, 0.0f, 0.0f, 1.0f}},
	{{10.0f, 0.0f, 0.0f, 1.0f}},
	{{0.0f, 0.0f, 0.0f, 1.0f}},
	{{0.0f, 10.0f, 0.0f, 1.0f}},
	{{0.0f, 0.0f, 0.0f, 1.0f}},
	{{0.0f, 0.0f, 10.0f, 1.0f}},
	};
	glBindBuffer (GL_ARRAY_BUFFER, lines->vbop);
	glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(p), p);

	Color c[6] =
	{
	0xFF0000FF,
	0xFF0000FF,
	0xFF00FF00,
	0xFF00FF00,
	0xFFFF0000,
	0xFFFF0000,
	};
	glBindBuffer (GL_ARRAY_BUFFER, lines->vboc);
	glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof(c), c);
}


int main (int argc, char * argv[])
{
	csc_crossos_enable_ansi_color ();
	ASSERT (argc);
	ASSERT (argv);
	srand (1);
	setbuf (stdout, NULL);

	uint32_t main_flags = CSC_SDLGLEW_RUNNING;

	SDL_Window * window;
	SDL_GLContext context;
	csc_sdlglew_create_window (&window, &context, WIN_TITLE, WIN_X, WIN_Y, WIN_W, WIN_H, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

	glEnable (GL_VERTEX_PROGRAM_POINT_SIZE);
	glLineWidth (4.0f);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);


	ecs_world_t * world = ecs_init();
	//Spatial
	ECS_COMPONENT_DEFINE (world, Color);
	ECS_COMPONENT_DEFINE (world, Position4);
	ECS_COMPONENT_DEFINE (world, Scale4);
	ECS_COMPONENT_DEFINE (world, Quaternion);
	ECS_COMPONENT_DEFINE (world, QuaternionDelta);
	ECS_COMPONENT_DEFINE (world, Transform);
	//Geometry
	ECS_COMPONENT_DEFINE (world, TextureUV);
	ECS_COMPONENT_DEFINE (world, Rectangle2f);
	//SDL
	ECS_COMPONENT_DEFINE (world, SDL_Keyboard);
	//Arrays
	ECS_COMPONENT_DEFINE (world, Capacity);
	ECS_COMPONENT_DEFINE (world, Offset);
	ECS_COMPONENT_DEFINE (world, Count);
	ECS_COMPONENT_DEFINE (world, Stride);
	ECS_COMPONENT_DEFINE (world, Filename);
	//OpenGL
	ECS_COMPONENT_DEFINE (world, GL_Texture);
	ECS_COMPONENT_DEFINE (world, GL_Pointcloud);
	ECS_COMPONENT_DEFINE (world, GL_VertexArrayObject);
	ECS_COMPONENT_DEFINE (world, GL_VertexBufferObject);
	ECS_COMPONENT_DEFINE (world, GL_Tex2DArray);
	ECS_COMPONENT_DEFINE (world, GL_Program);
	ECS_COMPONENT_DEFINE (world, GL_Shader);
	ECS_COMPONENT_DEFINE (world, GL_Lines);
	ECS_COMPONENT_DEFINE (world, GL_Mesh);
	ECS_TAG_DEFINE (world, GL_ProgramLinked);



	//system_opengl_init (world);
	//return 0;

	systems_init (world);
	system_opengl_init (world);
	system_texture_init (world);
	system_lines_init (world);
	system_mesh_init (world);
	system_pointcloud_init (world);
	system_camera_init (world);
	//test_ecs_onset (world);
	//test_ecs_addents (world);//For testing
	test_ecs_addlines (world);



	struct eavnet_context eavcontext = {0};
	eavcontext.world = world;
	eavnet_context_init (&eavcontext, "tcp://:9002");
	//test_eavnet (&eavcontext);

	//ecs_entity_t e3 = e2[0];
	const uint8_t * keyboard = SDL_GetKeyboardState (NULL);
	ecs_singleton_set (world, SDL_Keyboard, {keyboard});


	//SDL_Thread * t = SDL_CreateThread (eavnet_thread_recv, "mythread", &eavcontext);
	//ASSERT (t);

	int mouse_x;
	int mouse_y;

	while (main_flags & CSC_SDLGLEW_RUNNING)
	{
		SDL_Event event;
		while (SDL_PollEvent (&event))
		{
			csc_sdlglew_event_loop (window, &event, &main_flags, &global_gcam);

			switch (event.type)
			{
			case SDL_MOUSEWHEEL:{
				global_gcam.fov += event.wheel.y * f32_deg_to_rad (10.0f);
				global_gcam.fov = CLAMP(global_gcam.fov, f32_deg_to_rad (1.0f), f32_deg_to_rad (90.0f));
				XLOG (XLOG_INF, XLOG_GENERAL, "fov %f", f32_rad_to_deg (global_gcam.fov));
			}break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.state == SDL_PRESSED)
				{
					//event.motion.x
					XLOG(XLOG_INF, XLOG_SDL, "SDL_SetRelativeMouseMode SDL_TRUE");
					SDL_SetRelativeMouseMode (SDL_TRUE);
					//SDL_ShowCursor(SDL_DISABLE);
					SDL_SetWindowGrab (window, SDL_TRUE);
					SDL_GetMouseState (&mouse_x, &mouse_y);
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.state == SDL_RELEASED)
				{
					XLOG(XLOG_INF, XLOG_SDL, "SDL_SetRelativeMouseMode SDL_FALSE");
					SDL_SetRelativeMouseMode (SDL_FALSE);
					SDL_SetWindowGrab (window, SDL_FALSE);
					//SDL_ShowCursor(SDL_ENABLE);
					int w;
					int h;
					SDL_GetWindowSize (window, &w, &h);
					//SDL_WarpMouseInWindow (window, w / 2, h / 2);
					SDL_WarpMouseInWindow (window, mouse_x, mouse_y);
				}
				break;
			}

		}

		{
			//Control graphics camera
			csc_sdl_motion_wasd (keyboard, &global_gcam.d);
			csc_sdl_motion_pyr (keyboard, &global_gcam.pyr_delta);

			int mdltx = 0, mdlty = 0;
			SDL_GetRelativeMouseState(&mdltx, &mdlty);

			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				global_gcam.pyr_delta.x = (float)mdlty * -0.1f;
				global_gcam.pyr_delta.y = (float)mdltx * 0.1f;
				//global_gcam.
				//XLOG(XLOG_INF, "SDL_GetRelativeMouseState %i %i\n", mdltx, mdlty);
				//SDL_ShowCursor(SDL_DISABLE);
				/*
				int w;
				int h;
				SDL_GetWindowSize (window, &w, &h);
				SDL_WarpMouseInWindow (window, w / 2, h / 2);
				*/
			}

			if (SDL_GetModState() & KMOD_CAPS)
			{
				v3f32_mul (&global_gcam.d, &global_gcam.d, 0.001f);
				v3f32_mul (&global_gcam.pyr_delta, &global_gcam.pyr_delta, 0.001f);
			}
			else
			{
				v3f32_mul (&global_gcam.d, &global_gcam.d, 0.01f);
				v3f32_mul (&global_gcam.pyr_delta, &global_gcam.pyr_delta, 0.01f);
			}
			csc_gcam_update (&global_gcam);
		}

		glClearColor (0.3f, 0.3f, 0.3f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		eavnet_receiver1 (&eavcontext);





		if (keyboard[SDL_SCANCODE_1])
		{
			//qf32_rotate2_xyza (*ecs_get_mut (global_world, e3, component_quaternion, NULL), keyboard[SDL_SCANCODE_1], keyboard[SDL_SCANCODE_2], keyboard[SDL_SCANCODE_3], 0.01f);
		}

		//if (keyboard[SDL_SCANCODE_2])
		{
			//qf32_rotate2_xyza (*ecs_get_mut (world, e3, component_quaternion, NULL), 0.0f, 1.0f, 0.0f, 0.01f);
		}

		//if (keyboard[SDL_SCANCODE_3])
		{
			//qf32_rotate2_xyza (*ecs_get_mut (world, e3, component_quaternion, NULL), 0.0f, 0.0f, 1.0f, 0.01f);
		}

		/*
		glUniform1i (uniform_texture1, 0);
		glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, (const GLfloat *) gcam.mvp);
		glDrawArrays (GL_TRIANGLES, 0, img.cap * CSC_GLIMAGE_VERTS_COUNT);
		*/

		ecs_progress (world, 0);
		SDL_Delay (10);
		SDL_GL_SwapWindow (window);
	}

	SDL_GL_DeleteContext (context);
	SDL_DestroyWindow (window);
	SDL_Quit ();
	ecs_fini (world);
	return 0;
}

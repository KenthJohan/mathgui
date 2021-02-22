#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <flecs.h>
#include <stdio.h>

#include "csc/csc_crossos.h"
#include "csc/csc_basic.h"
#include "csc/csc_debug.h"
#include "csc/csc_math.h"

#include "components.h"
#include "eavnet.h"


static void eavnet_test (TCPsocket ctx)
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
		uint32_t count = 55000;
		SDLNet_TCP_Send (ctx, &(struct mynet_eav){MYENT_DRAW_CLOUD, ATTR_POINTCLOUD}, 0);
		SDLNet_TCP_Send (ctx, &(struct mynet_eav_u32){MYENT_DRAW_CLOUD, ATTR_COUNT, count}, 0);
		printf ("sizeof (struct mynet_eav): %i\n", sizeof (struct mynet_eav));
		uint32_t size = count * sizeof (component_position) + sizeof (struct mynet_eav);
		struct mynet_eav * pc = malloc (size);
		pc->entity = MYENT_DRAW_CLOUD;
		pc->attribute = ATTR_POINTCLOUD_POS;
		component_position * p = (void*)pc->value;
		for (uint32_t i = 0; i < count; ++i)
		{
			p[i][0] = 10.0f * (float)i / rand();
			p[i][1] = 1.0f * (float)i / rand();
			p[i][2] = 10.0f * (float)i / rand();
			p[i][3] = 100.0f;
		}
		printf ("SDLNet_TCP_Send %i\n", size);
		SDLNet_TCP_Send (ctx, pc, size);
	}

	/*
	EAVNET_SEND (ctx, struct mynet_eav_component_texture, {MYENT_TEXTURE1, 1});
	EAVNET_SEND (ctx, struct mynet_eav_component_texture, {MYENT_TEXTURE2, ATTR_TEXTURE, {0, 300, 300, 1}});
	EAVNET_SEND (ctx, struct mynet_eav, {MYENT_MESH_RECTANGLE, ATTR_MESH}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_u32, {MYENT_MESH_RECTANGLE, ATTR_COUNT, 6}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_component_rectangle, {MYENT_MESH_RECTANGLE, ATTR_RECTANGLE, {1.0f, 1.0f}}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_component_position, {MYENT_DRAW_IMG1, ATTR_POSITION, {3.0f, 1.0f, 0.0f, 1.0f}}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_component_position, {MYENT_DRAW_IMG1, ATTR_SCALE, {0.3f, 0.3f, 0.0f, 1.0f}}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_component_position, {MYENT_DRAW_IMG1, ATTR_QUATERNION, {0.0f, 0.0f, 0.0f, 1.0f}}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_u32, {MYENT_DRAW_IMG1, ATTR_ADD_INSTANCEOF, MYENT_MESH_RECTANGLE}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_u32, {MYENT_DRAW_IMG1, ATTR_ADD_INSTANCEOF, MYENT_TEXTURE1}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_component_position, {MYENT_DRAW_IMG2, ATTR_POSITION, {4.0f, 1.0f, 0.0f, 1.0f}}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_component_position, {MYENT_DRAW_IMG2, ATTR_SCALE, {0.3f, 0.3f, 0.0f, 1.0f}}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_component_position, {MYENT_DRAW_IMG2, ATTR_QUATERNION, {0.0f, 0.0f, 0.0f, 1.0f}}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_u32, {MYENT_DRAW_IMG2, ATTR_ADD_INSTANCEOF, MYENT_MESH_RECTANGLE}, 0);
	EAVNET_SEND (ctx, struct mynet_eav_u32, {MYENT_DRAW_IMG2, ATTR_ADD_INSTANCEOF, MYENT_TEXTURE2}, 0);
	*/


	//mynet_send_ptr(NULL, 0, 0, &(component_position){1.0f, 2.0f, 3.0f, 1.0f}, sizeof (component_position));

}




int main (int argc, char * argv[])
{
	csc_crossos_enable_ansi_color ();
	ASSERT (argc);
	ASSERT (argv);

	if (SDLNet_Init() == -1)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(2);
	}


	IPaddress ip;
	TCPsocket client;

	if (SDLNet_ResolveHost (&ip, "localhost", 9999) == -1)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(1);
	}

	client = SDLNet_TCP_Open (&ip);

	if (!client)
	{
		printf ("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit (2);
	}

	int i = 0;
	while(1)
	{
		/*
		char msg[100];
		snprintf (msg, 100, "Hello banasns %i\n", i);
		int len = strlen (msg) + 1;
		i++;
		int result = SDLNet_TCP_Send (client, msg, len);

		if (result < len)
		{
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			// It may be good to disconnect sock because it is likely invalid now.
			break;
		}
		*/
		eavnet_test (client);
		SDL_Delay (2000);

	}
	SDLNet_TCP_Close (client);

	SDLNet_Quit();

	return 0;
}

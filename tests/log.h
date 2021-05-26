#pragma once

#include <flecs.h>

#include "mg_compdef.h"

static ecs_world_t * log_world = NULL;
static void ecslog_init()
{
	log_world = ecs_init();
}


static void ecslog_newcomp(ecs_world_t * world, char const * name, int size)
{
	ecs_entity_t e = ecs_component_init(world, &(ecs_component_desc_t)
	{
	.entity.name = "Position",
	.size = size,
	.alignment = 8
	});
	ecs_type_t t = ecs_type_from_entity(world, name);

	ecs_set_component_actions(world, ecs_typeid(String),
	&(EcsComponentLifecycle){
	.ctor = ecs_ctor(String),
	.dtor = ecs_dtor(String),
	.copy = ecs_copy(String),
	.move = ecs_move(String)
	});

}






static void ecslog(char const * path, int line, char const * function)
{
	ecs_world_t * world = log_world;
	ecs_entity_t e = ecs_new (world, 0);
	ecs_add (world, e, String512);
	ecs_set (world, e, LineNumber, {line});
	String512 * message = ecs_get_mut (world, e, String512, NULL);
	//snprintf(message, );
}




static void assert_format_va
(
int id,
char const * file,
int line,
char const * fn,
char const * exp,
char const * fmt,
va_list va
)
{
	fprintf (stderr, ASSERT_TCOL0 "ASSERT%04i" TCOL_RST " ", id);
	fprintf (stderr, TCOL (TCOL_NORMAL, TCOL_WHITE, TCOL_DEFAULT) "%s:%04i" TCOL_RST " in ", file, line);
	fprintf (stderr, ASSERT_TCOLID "%s() " TCOL_RST, fn);
	if (errno != 0) {fprintf (stderr, "[%04i:" TCOL (TCOL_BOLD, TCOL_RED , TCOL_DEFAULT) "%s" TCOL_RST "]: ", errno, strerror (errno));}
	if (exp){fprintf (stderr, TCOL (TCOL_NORMAL, TCOL_WHITE, TCOL_DEFAULT) "%s" TCOL_RST " ", exp);}
	if (fmt)
	{
		vfprintf (stderr, fmt, va);
	}
	fprintf (stderr, "\n");
	fflush (stderr);
}

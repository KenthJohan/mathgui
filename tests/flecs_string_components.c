#include <flecs.h>

typedef struct {
	float x;
	float y;
} Position, Velocity;

typedef struct {
	char value[0];
} String;


void Move(ecs_iter_t *it)
{
	Position *p = ecs_term(it, Position, 1);
	Velocity *v = ecs_term(it, Velocity, 2);
	String *s = ecs_term(it, String, 3);

	for (int i = 0; i < it->count; i ++)
	{
		p[i].x += v[i].x * it->delta_time;
		p[i].y += v[i].y * it->delta_time;
		printf("Entity %s moved! %s\n", ecs_get_name(it->world, it->entities[i]), s[i].value);
	}
}

int main(int argc, char *argv[])
{
	ecs_world_t *ecs = ecs_init();
	printf ("sizeof(String): %i\n", sizeof(String));
	// Register the Position component
	ecs_entity_t pos = ecs_component_init(ecs, &(ecs_component_desc_t){
	.entity.name = "Position",
	.size = sizeof(Position),
	.alignment = ECS_ALIGNOF(Position)
	});

	// Register the Velocity component
	ecs_entity_t vel = ecs_component_init(ecs, &(ecs_component_desc_t){
	.entity.name = "Velocity",
	.size = sizeof(Velocity),
	.alignment = ECS_ALIGNOF(Velocity)
	});

	// Register the Velocity component
	ecs_entity_t str = ecs_component_init(ecs, &(ecs_component_desc_t){
	.entity.name = "Message",
	.size = sizeof(String) + 64,
	.alignment = ECS_ALIGNOF(String)
	});

	bool added = false;
	EcsComponent *ptr = ecs_get_mut(ecs, str, EcsComponent, &added);
	printf ("added %i\n", added);
	printf ("size %i\n", ptr->size);
	printf ("alignment %i\n", ptr->alignment);

	// Create the Move system
	ecs_system_init(ecs, &(ecs_system_desc_t){
	.entity = { .name = "Move", .add = {EcsOnUpdate} },
	.query.filter.terms = {{pos, .inout = EcsInOut}, {vel, .inout = EcsIn}, {str, .inout = EcsIn}},
	.callback = Move,
	});

	// Create a named entity
	ecs_entity_t e = ecs_entity_init(ecs, &(ecs_entity_desc_t)
	{
	.name = "MyEntity"
	});

	char msg[128] = "Hello";
	// Set components
	ecs_set_id(ecs, e, pos, sizeof(Position), &(Position){0, 0});
	ecs_set_id(ecs, e, vel, sizeof(Velocity), &(Velocity){1, 1});
	ecs_set_id(ecs, e, str, sizeof(String) + 128, msg);

	// Run the main loop
	//while (ecs_progress(ecs, 0)) { }
}

#include <flecs.h>


typedef struct DynamicBuffer
{
	uint32_t size;
	uint8_t * data;
} DynamicBuffer;




int main(int argc, char *argv[])
{
	ecs_world_t *ecs = ecs_init();

	// Register the Position component
	ecs_entity_t pos = ecs_component_init(ecs, &(ecs_component_desc_t)
	{
	.entity.name = "CE30_Pos",
	.size = sizeof(DynamicBuffer),
	.alignment = ECS_ALIGNOF(DynamicBuffer)
	});

	// Register the Velocity component
	ecs_entity_t vel = ecs_component_init(ecs, &(ecs_component_desc_t)
	{
	.entity.name = "CE30_Amp",
	.size = sizeof(DynamicBuffer),
	.alignment = ECS_ALIGNOF(DynamicBuffer)
	});




	ECS_COMPONENT(ecs, DynamicBuffer);

	// Create the Move system
	ecs_system_init(ecs, &(ecs_system_desc_t){
	.entity = { .name = "Move", .add = {EcsOnUpdate} },
	.query.filter.terms = {{pos}, {vel, .inout = EcsIn}},
	.callback = Move,
	});

	// Create a named entity
	ecs_entity_t e = ecs_entity_init(ecs, &(ecs_entity_desc_t){
	.name = "MyEntity"
	});

	// Set components
	ecs_set_id(ecs, e, pos, sizeof(Position), &(Position){0, 0});
	ecs_set_id(ecs, e, vel, sizeof(Velocity), &(Velocity){1, 1});

	// Run the main loop
	while (ecs_progress(ecs, 0)) { }
}
}

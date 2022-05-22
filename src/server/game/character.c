#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "include/types.h"
#include "include/character.h"

static float distance_between(struct character_position *a, struct character_position *b)
{
	float dx = 0;
	float dy = 0;
	float dz = 0;

	assert(a);
	assert(b);

	dx = a->x - b->x;
	dy = a->y - b->y;
	dz = a->z - b->z;
	
	return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
}

static void periodic_position_update(struct character *src, seconds delta)
{
	struct character_position velocity = { 0 };
	float distance = 0;

	assert(src);

	TODO("remove hardcoded run speed.");
	src->run_speed = 300;

	distance = distance_between(&src->position, &src->moving_to);

	// If we are close enough, finish the movement.
	if (distance < 10) {
		printf("reached destination.\n");
		src->position = src->moving_to;
		src->state = IDLE;
		return;
	}

	printf("moving to new position.\n");

	// Normalized velocity.
	velocity.x = ((float) (src->position.x - src->moving_to.x) / distance);
	velocity.x = ((float) (src->position.y - src->moving_to.y) / distance);
	velocity.z = ((float) (src->position.z - src->moving_to.z) / distance);

	velocity.x *= (((float) src->run_speed) * delta);
	velocity.y *= (((float) src->run_speed) * delta);
	velocity.z *= (((float) src->run_speed) * delta);

	printf("moving to: %d, %d, %d.\n", src->moving_to.x, src->moving_to.y, src->moving_to.z);
	printf("prev position: %d, %d, %d\n", src->position.x, src->position.y, src->position.z);

	src->position.x += velocity.x;
	src->position.y += velocity.y;
	src->position.z += velocity.z;

	printf("new  position: %d, %d, %d\n", src->position.x, src->position.y, src->position.z);
}

void character_start_movement(struct character *src, struct character_position *position)
{
	assert(src);
	assert(position);
	src->state = MOVING;
	src->moving_to = *position;
}

void character_tick(struct character *src, seconds delta)
{
	assert(src);

	switch (src->state) {
	case IDLE:
		printf("idle.\n");
		break;
	case MOVING:
		periodic_position_update(src, delta);
		break;
	default:
		break;
	}
}

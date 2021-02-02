#include "character/handle.h"
#include "in_world.h"

void state_machine_in_world(request_t *request)
{
        assert_valid_request(request);
        state_machine_character_handle(request);
}

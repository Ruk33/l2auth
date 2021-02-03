#include <headers.h>
#include <request.h>
#include "spawn.h"
#include "idle.h"
#include "handle.h"

void state_machine_character_handle(request_t *request)
{
        assert_valid_request(request);

        character_t *character = NULL;

        character = storage_character_active_from_session(&request->storage->character_storage, request->session);

        if (!character) {
                printf("In world but no character? Ignoring request.\n");
                return;
        }

        switch (character->state) {
        case SPAWN:
                state_machine_character_spawn(request, character);
                break;
        case IDLE:
                state_machine_character_idle(request, character);
                break;
        case RUNNING:
                break;
        case WALKING:
                break;
        default:
                printf("Invalid state.\n");
                break;
        }
}

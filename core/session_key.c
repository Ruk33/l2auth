#include <assert.h>
#include <stdlib.h>
#include "session_key.h"

void l2_session_key_init(struct L2SessionKey* session)
{
        assert(session);
        session->playOK1 = rand();
        session->playOK2 = rand();
        session->loginOK1 = rand();
        session->loginOK2 = rand();
}

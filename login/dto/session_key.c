#ifndef L2AUTH_LOGIN_SESSION_KEY_C
#define L2AUTH_LOGIN_SESSION_KEY_C

#include <stdlib.h>
#include <login/dto/session_key.h>

struct LoginDtoSessionKey* login_session_key_create()
{
        struct LoginDtoSessionKey* session_key = calloc(1, sizeof(struct LoginDtoSessionKey));

        session_key->playOK1 = rand();
        session_key->playOK2 = rand();
        session_key->loginOK1 = rand();
        session_key->loginOK2 = rand();

        return session_key;
}

#endif

#ifndef L2AUTH_LOGIN_SESSION_KEY_H
#define L2AUTH_LOGIN_SESSION_KEY_H

struct LoginDtoSessionKey
{
        char login_name[20];
        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;
};

void login_session_key_init(struct LoginDtoSessionKey* session);

#endif

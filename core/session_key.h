#ifndef L2AUTH_SESSION_KEY_H
#define L2AUTH_SESSION_KEY_H

struct L2SessionKey
{
        char login_name[20];
        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;
};

void l2_session_key_init(struct L2SessionKey* session);

#endif

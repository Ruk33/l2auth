#ifndef INCLUDE_LS_SESSION_H
#define INCLUDE_LS_SESSION_H

#include "os_io.h"
#include "ls_blowfish.h"
#include "ls_rsa.h"

typedef struct {
        os_io_t *socket;
        ls_blowfish_t *blowfish;
        ls_rsa_t *rsa;
        int playOK1;
        int playOK2;
} ls_session_t;

ls_session_t *ls_session_new(os_io_t *socket);

ls_session_t *ls_session_find(os_io_t *socket);

#endif

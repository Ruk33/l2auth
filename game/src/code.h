#ifndef CODE_H
#define CODE_H

#include <sys/types.h>
#include "../shared/client_id.h"
#include "../shared/l2_server.h"
#include "../shared/request.h"

struct Code {
        time_t last_changed;
        void* library;
        void* (* new_conn)(struct L2Server *l2_server, client_id id);
        void  (* handle_request)(struct Request *request);
        void  (* handle_disconnect)(struct Request *request);
};

void code_load(struct Code *code);

#endif

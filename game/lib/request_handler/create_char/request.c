#include <assert.h>
#include <stdlib.h>
#include <core/byte_reader.h>
#include <core/l2_packet.h>
#include <core/l2_raw_packet.h>
#include <core/l2_string.h>
#include <string.h>
#include "../../client.h"
#include "request.h"

struct CreateCharRequest *create_char_request
(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        struct CreateCharRequest *request = NULL;
        unsigned char *content = NULL;
        size_t name_length = 0;

        content = l2_packet_content(packet);
        request = client_alloc_mem(client, sizeof(*request));

        name_length = l2_string_len((l2_string *) content) + 1;
        memset(request->name, 0, sizeof(request->name));
        l2_string_from_l2(request->name, (l2_string *) content, name_length);

        content = content + l2_string_calculate_space_from_char(name_length);
        content = byte_reader_cpy_int_n_mv(content, &request->race);
        content = byte_reader_cpy_int_n_mv(content, &request->sex);
        content = byte_reader_cpy_int_n_mv(content, &request->class_id);
        content = byte_reader_cpy_int_n_mv(content, &request->_int);
        content = byte_reader_cpy_int_n_mv(content, &request->str);
        content = byte_reader_cpy_int_n_mv(content, &request->con);
        content = byte_reader_cpy_int_n_mv(content, &request->men);
        content = byte_reader_cpy_int_n_mv(content, &request->dex);
        content = byte_reader_cpy_int_n_mv(content, &request->wit);
        content = byte_reader_cpy_int_n_mv(content, &request->hair_style);
        content = byte_reader_cpy_int_n_mv(content, &request->hair_color);
        content = byte_reader_cpy_int_n_mv(content, &request->face);

        return request;
}

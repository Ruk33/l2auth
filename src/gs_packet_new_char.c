#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_character_template.h"
#include "include/gs_packet_new_char.h"

void gs_packet_new_char_add_template(
        gs_packet_new_char_t *dest,
        gs_character_template_t *src)
{
        assert(dest);
        assert(src);
        assert((size_t) dest->count < arr_size(dest->templates));

        dest->templates[dest->count].race   = src->race;
        dest->templates[dest->count]._class = src->_class;
        dest->templates[dest->count].con    = src->con;
        dest->templates[dest->count].dex    = src->dex;
        dest->templates[dest->count].men    = src->men;
        dest->templates[dest->count].str    = src->str;
        dest->templates[dest->count].wit    = src->wit;
        dest->templates[dest->count]._int   = src->_int;

        dest->count += 1;
}

void gs_packet_new_char_pack(packet_t *dest, gs_packet_new_char_t *src)
{
        byte_t type = 0x0;

        assert(dest);
        assert(src);

        type = 0x17;

        packet_append_val(dest, type);
        packet_append_val(dest, src->count);

        for (int i = 0; i < src->count; i += 1) {
                packet_append_val(dest, src->templates[i].race);
                packet_append_val(dest, src->templates[i]._class);
                packet_append_val(dest, src->templates[i].separator_46[0]);

                packet_append_val(dest, src->templates[i].str);
                packet_append_val(dest, src->templates[i].separator_0a[1]);
                packet_append_val(dest, src->templates[i].separator_46[1]);

                packet_append_val(dest, src->templates[i].dex);
                packet_append_val(dest, src->templates[i].separator_0a[2]);
                packet_append_val(dest, src->templates[i].separator_46[2]);

                packet_append_val(dest, src->templates[i].con);
                packet_append_val(dest, src->templates[i].separator_0a[3]);
                packet_append_val(dest, src->templates[i].separator_46[3]);

                packet_append_val(dest, src->templates[i]._int);
                packet_append_val(dest, src->templates[i].separator_0a[4]);
                packet_append_val(dest, src->templates[i].separator_46[4]);

                packet_append_val(dest, src->templates[i].wit);
                packet_append_val(dest, src->templates[i].separator_0a[5]);
                packet_append_val(dest, src->templates[i].separator_46[5]);

                packet_append_val(dest, src->templates[i].men);
                packet_append_val(dest, src->templates[i].separator_0a[0]);
        }
}

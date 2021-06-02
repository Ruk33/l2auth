#include "include/util.h"
#include "include/packet.h"
#include "include/character_template.h"
#include "include/packet_new_char.h"

void packet_new_char_add_template(packet_new_char_t *d, character_template_t *s)
{
        d->templates[d->count].race   = s->race;
        d->templates[d->count]._class = s->_class;
        d->templates[d->count].con    = s->con;
        d->templates[d->count].dex    = s->dex;
        d->templates[d->count].men    = s->men;
        d->templates[d->count].str    = s->str;
        d->templates[d->count].wit    = s->wit;
        d->templates[d->count]._int   = s->_int;
        d->count += 1;
}

void packet_new_char_pack(packet_t *dest, packet_new_char_t *src)
{
        byte_t type = 0x0;

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

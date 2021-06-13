#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/l2_string.h"
#include "include/gs_packet_char_select.h"

void gs_packet_char_select_set_playok(gs_packet_char_select_t *dest, int playOK1)
{
        assert(dest);
        dest->playOK1 = playOK1;
}

void gs_packet_char_select_set_char(
        gs_packet_char_select_t *dest,
        gs_character_t *src)
{
        assert(dest);
        assert(src);
        assert(src->session);

        l2_string_from_char(dest->name, src->name, sizeof(dest->name));
        l2_string_from_char(dest->title, src->title, sizeof(dest->title));

        dest->id        = src->session->id;
        dest->active    = 1;
        dest->race_id   = src->race;
        dest->class_id  = src->_class;
        dest->exp       = src->exp;
        dest->sp        = src->sp;
        dest->level     = src->level;
        dest->hp        = src->hp;
        dest->mp        = src->mp;
        dest->con       = src->con;
        dest->dex       = src->dex;
        dest->men       = src->men;
        dest->str       = src->str;
        dest->wit       = src->wit;
        dest->_int      = src->_int;
        dest->sex       = src->sex;
        dest->x         = src->x;
        dest->y         = src->y;
        dest->z         = src->z;
        dest->game_time = 42;
}

void gs_packet_char_select_pack(packet_t *dest, gs_packet_char_select_t *src)
{
        byte_t type = 0x00;

        size_t name_size  = 0;
        size_t title_size = 0;

        assert(dest);
        assert(src);

        type = 0x15;

        name_size  = l2_string_bytes(src->name);
        title_size = l2_string_bytes(src->title);

        packet_append_val(dest, type);
        packet_append_n(dest, (byte_t *) src->name, name_size);
        packet_append_val(dest, src->id);
        packet_append_n(dest, (byte_t *) src->title, title_size);
        packet_append_val(dest, src->playOK1);
        packet_append_val(dest, src->clan_id);
        packet_append_val(dest, src->empty[0]);
        packet_append_val(dest, src->sex);
        packet_append_val(dest, src->race_id);
        packet_append_val(dest, src->class_id);
        packet_append_val(dest, src->active);
        packet_append_val(dest, src->x);
        packet_append_val(dest, src->y);
        packet_append_val(dest, src->z);
        packet_append_val(dest, src->hp);
        packet_append_val(dest, src->mp);
        packet_append_val(dest, src->sp);
        packet_append_val(dest, src->exp);
        packet_append_val(dest, src->level);
        packet_append_val(dest, src->empty[0]);
        packet_append_val(dest, src->empty[0]);
        packet_append_val(dest, src->_int);
        packet_append_val(dest, src->str);
        packet_append_val(dest, src->con);
        packet_append_val(dest, src->men);
        packet_append_val(dest, src->dex);
        packet_append_val(dest, src->wit);
        packet_append_n(
                dest, (byte_t *) src->empty, sizeof(src->empty[0]) * 36);
        packet_append_val(dest, src->game_time);
        packet_append_n(
                dest, (byte_t *) src->empty, sizeof(src->empty[0]) * 15);
}

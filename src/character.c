#include "include/l2_string.h"
#include "include/packet_create_char_request.h"
#include "include/character.h"

void character_from_request(character_t *dest, packet_create_char_request_t *src)
{
        l2_string_to_char(dest->name, src->name, sizeof(dest->name));

        dest->race       = src->race;
        dest->_class     = src->_class;
        dest->face       = src->face;
        dest->hair_color = src->hair_color;
        dest->hair_style = src->hair_style;
        dest->con        = src->con;
        dest->dex        = src->dex;
        dest->men        = src->men;
        dest->sex        = src->sex;
        dest->str        = src->str;
        dest->wit        = src->wit;
        dest->_int       = src->_int;
}

#include "../headers.h"
#include "new_char.h"

static byte *write_template_to_buffer(byte *buf, struct ServerPacketNewCharCharTemplate *template)
{
        assert(buf);
        assert(template);

        BYTE_WRITE_VAL(buf, template->race);
        BYTE_WRITE_VAL(buf, template->_class);
        BYTE_WRITE_VAL(buf, template->separator_46[0]);

        BYTE_WRITE_VAL(buf, template->str);
        BYTE_WRITE_VAL(buf, template->separator_0a[0]);
        BYTE_WRITE_VAL(buf, template->separator_46[1]);

        BYTE_WRITE_VAL(buf, template->dex);
        BYTE_WRITE_VAL(buf, template->separator_0a[1]);
        BYTE_WRITE_VAL(buf, template->separator_46[2]);

        BYTE_WRITE_VAL(buf, template->con);
        BYTE_WRITE_VAL(buf, template->separator_0a[2]);
        BYTE_WRITE_VAL(buf, template->separator_46[3]);

        BYTE_WRITE_VAL(buf, template->_int);
        BYTE_WRITE_VAL(buf, template->separator_0a[3]);
        BYTE_WRITE_VAL(buf, template->separator_46[4]);

        BYTE_WRITE_VAL(buf, template->wit);
        BYTE_WRITE_VAL(buf, template->separator_0a[4]);
        BYTE_WRITE_VAL(buf, template->separator_46[5]);

        BYTE_WRITE_VAL(buf, template->men);
        BYTE_WRITE_VAL(buf, template->separator_0a[5]);

        return buf;
}

void server_packet_new_char(packet *dest)
{
        assert(dest);

        packet_type type = 0x17;

        struct ServerPacketNewChar new_char_packet = {0};

        byte buf[SERVER_PACKET_NEW_CHAR_SIZE] = {0};
        byte *p = buf;

        size_t human_figher = 0;
        size_t human_figher_copy = 1;
        size_t human_mage = 2;
        size_t elf_fighter = 3;
        size_t elf_mage = 4;
        size_t dark_elf_figher = 5;
        size_t dark_elf_mage = 6;
        size_t orc_figher = 7;
        size_t orc_mage = 8;
        size_t dwarf = 9;

        new_char_packet.char_templates_count = 10;
        BYTE_WRITE_VAL(p, new_char_packet.char_templates_count);

        new_char_packet.char_templates[human_figher].race = 0;
        new_char_packet.char_templates[human_figher]._class = 0;
        new_char_packet.char_templates[human_figher].str = 40;
        new_char_packet.char_templates[human_figher].dex = 30;
        new_char_packet.char_templates[human_figher].con = 43;
        new_char_packet.char_templates[human_figher]._int = 21;
        new_char_packet.char_templates[human_figher].wit = 11;
        new_char_packet.char_templates[human_figher].men = 25;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[human_figher]);

        new_char_packet.char_templates[human_figher_copy].race = 0;
        new_char_packet.char_templates[human_figher_copy]._class = 0;
        new_char_packet.char_templates[human_figher_copy].str = 40;
        new_char_packet.char_templates[human_figher_copy].dex = 30;
        new_char_packet.char_templates[human_figher_copy].con = 43;
        new_char_packet.char_templates[human_figher_copy]._int = 21;
        new_char_packet.char_templates[human_figher_copy].wit = 11;
        new_char_packet.char_templates[human_figher_copy].men = 25;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[human_figher_copy]);

        new_char_packet.char_templates[human_mage].race = 0;
        new_char_packet.char_templates[human_mage]._class = 10;
        new_char_packet.char_templates[human_mage].str = 22;
        new_char_packet.char_templates[human_mage].dex = 21;
        new_char_packet.char_templates[human_mage].con = 27;
        new_char_packet.char_templates[human_mage]._int = 41;
        new_char_packet.char_templates[human_mage].wit = 20;
        new_char_packet.char_templates[human_mage].men = 39;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[human_mage]);

        new_char_packet.char_templates[elf_fighter].race = 1;
        new_char_packet.char_templates[elf_fighter]._class = 18;
        new_char_packet.char_templates[elf_fighter].str = 36;
        new_char_packet.char_templates[elf_fighter].dex = 35;
        new_char_packet.char_templates[elf_fighter].con = 36;
        new_char_packet.char_templates[elf_fighter]._int = 23;
        new_char_packet.char_templates[elf_fighter].wit = 14;
        new_char_packet.char_templates[elf_fighter].men = 26;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[elf_fighter]);

        new_char_packet.char_templates[elf_mage].race = 1;
        new_char_packet.char_templates[elf_mage]._class = 25;
        new_char_packet.char_templates[elf_mage].str = 21;
        new_char_packet.char_templates[elf_mage].dex = 24;
        new_char_packet.char_templates[elf_mage].con = 25;
        new_char_packet.char_templates[elf_mage]._int = 37;
        new_char_packet.char_templates[elf_mage].wit = 23;
        new_char_packet.char_templates[elf_mage].men = 40;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[elf_mage]);

        new_char_packet.char_templates[dark_elf_figher].race = 2;
        new_char_packet.char_templates[dark_elf_figher]._class = 31;
        new_char_packet.char_templates[dark_elf_figher].str = 41;
        new_char_packet.char_templates[dark_elf_figher].dex = 34;
        new_char_packet.char_templates[dark_elf_figher].con = 32;
        new_char_packet.char_templates[dark_elf_figher]._int = 25;
        new_char_packet.char_templates[dark_elf_figher].wit = 12;
        new_char_packet.char_templates[dark_elf_figher].men = 26;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[dark_elf_figher]);

        new_char_packet.char_templates[dark_elf_mage].race = 2;
        new_char_packet.char_templates[dark_elf_mage]._class = 38;
        new_char_packet.char_templates[dark_elf_mage].str = 23;
        new_char_packet.char_templates[dark_elf_mage].dex = 23;
        new_char_packet.char_templates[dark_elf_mage].con = 24;
        new_char_packet.char_templates[dark_elf_mage]._int = 44;
        new_char_packet.char_templates[dark_elf_mage].wit = 19;
        new_char_packet.char_templates[dark_elf_mage].men = 37;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[dark_elf_mage]);

        new_char_packet.char_templates[orc_figher].race = 3;
        new_char_packet.char_templates[orc_figher]._class = 44;
        new_char_packet.char_templates[orc_figher].str = 40;
        new_char_packet.char_templates[orc_figher].dex = 26;
        new_char_packet.char_templates[orc_figher].con = 47;
        new_char_packet.char_templates[orc_figher]._int = 18;
        new_char_packet.char_templates[orc_figher].wit = 12;
        new_char_packet.char_templates[orc_figher].men = 27;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[orc_figher]);

        new_char_packet.char_templates[orc_mage].race = 3;
        new_char_packet.char_templates[orc_mage]._class = 49;
        new_char_packet.char_templates[orc_mage].str = 27;
        new_char_packet.char_templates[orc_mage].dex = 24;
        new_char_packet.char_templates[orc_mage].con = 31;
        new_char_packet.char_templates[orc_mage]._int = 31;
        new_char_packet.char_templates[orc_mage].wit = 15;
        new_char_packet.char_templates[orc_mage].men = 42;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[orc_mage]);

        new_char_packet.char_templates[dwarf].race = 4;
        new_char_packet.char_templates[dwarf]._class = 53;
        new_char_packet.char_templates[dwarf].str = 39;
        new_char_packet.char_templates[dwarf].dex = 29;
        new_char_packet.char_templates[dwarf].con = 45;
        new_char_packet.char_templates[dwarf]._int = 20;
        new_char_packet.char_templates[dwarf].wit = 10;
        new_char_packet.char_templates[dwarf].men = 27;
        p = write_template_to_buffer(p, &new_char_packet.char_templates[dwarf]);

        packet_build(dest, type, buf, (size_t) (p - buf));
}

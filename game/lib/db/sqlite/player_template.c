#include <headers.h>
#include <sqlite3.h>
#include <player_template.h>
#include <db/player_template.h>

static char find_by_race_and_class_query[] = "select "
                                             "class_id, race_id, str, "
                                             "con, dex, _int, "
                                             "wit, men, p_atk, "
                                             "p_def, m_atk, m_def, "
                                             "p_spd, m_spd, accuracy, "
                                             "critical, evasion, walk_spd, "
                                             "x, y, z, can_craft "
                                             "from player_templates "
                                             "where race_id = :race_id "
                                             "and class_id = :class_id "
                                             "limit 1;";

int storage_player_template_find_by(
        db_conn_t *        db,
        player_template_t *dest,
        race_id_t          race_id,
        class_id_t         class_id)
{
        assert(db);
        assert(dest);
}

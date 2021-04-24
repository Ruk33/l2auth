#ifndef DB_SQLITE_HELPER_H
#define DB_SQLITE_HELPER_H

#define helper_bind_text(stmt, param, val) \
        sqlite3_bind_text(                 \
                stmt, sqlite3_bind_parameter_index(stmt, param), val, -1, NULL)

#define helper_bind_int(stmt, param, val) \
        sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, param), val)

#define helper_bind_blob(stmt, param, val, n) \
        sqlite3_bind_blob(                    \
                stmt, sqlite3_bind_parameter_index(stmt, param), val, n, NULL)

#define helper_bind_blob_arr(stmt, param, val) \
        helper_bind_blob(stmt, param, val, sizeof(val))

#endif

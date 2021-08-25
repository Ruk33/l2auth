create table if not exists servers(
        id int,
        ip text,
        port int,
        age_limit int,
        pvp int,
        players int,
        max_players int,
        status int,
        extra int,
        brackets int,
        primary key (id)
);

create table if not exists characters(
        username text,
        name text,
        race int,
        sex int,
        _class int,
        _int int,
        str int,
        con int,
        men int,
        dex int,
        wit int,
        hair_style int,
        hair_color int,
        face int,
        level int,
        exp int,
        sp int,
        hp int,
        mp int,
        cp int,
        max_hp int,
        max_mp int,
        max_cp int,
        p_attack int,
        m_attack int,
        p_def int,
        m_def int,
        evasion_rate int,
        accuracy int,
        critical_hit int,
        run_speed int,
        walk_speed int,
        p_attack_speed int,
        m_attack_speed int,
        movement_speed_multiplier int,
        attack_speed_multiplier int,
        collision_radius int,
        collision_height int,
        name_color int,
        max_load int,
        x int,
        y int,
        z int,
        primary key (username, name)
);

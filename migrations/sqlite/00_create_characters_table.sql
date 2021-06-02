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
    primary key (username, name)
);

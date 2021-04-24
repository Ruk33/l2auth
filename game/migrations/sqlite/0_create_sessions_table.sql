create table sessions(
        id primary key,
        username text,
        encrypt_key blob,
        decrypt_key blob,
        playOK1 integer,
        playOK2 integer,
        loginOK1 integer,
        loginOK2 integer,
        char_index integer,
        state integer
);

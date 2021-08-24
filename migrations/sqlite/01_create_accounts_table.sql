create table if not exists accounts(
        username text,
        encrypted_password blob,
        primary key (username)
);

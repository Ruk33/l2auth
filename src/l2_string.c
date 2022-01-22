typedef unsigned char l2_string_t;

#define macro_l2_str_to_char_arr(dest, src, sn) \
    l2_string_to_char((char *) dest,            \
                      (l2_string_t *) src,      \
                      sizeof(dest),             \
                      (size_t) sn,              \
                      (size_t) sn)

#define macro_l2_str_arr_to_char_arr(dest, src) \
    l2_string_to_char((char *) dest,            \
                      (l2_string_t *) src,      \
                      sizeof(dest),             \
                      sizeof(src),              \
                      sizeof(src))

#define macro_l2_str_arr_from_char_arr(dest, src) \
    l2_string_from_char((l2_string_t *) dest,     \
                        (char *) src,             \
                        sizeof(dest),             \
                        sizeof(src),              \
                        sizeof(src))

// Copy n bytes or until NULL terminator from src to dest.
// Won't overflow and guarantees NULL terminator.
void l2_string_cpy(l2_string_t *dest,
                   l2_string_t *src,
                   size_t dn,
                   size_t sn,
                   size_t n)
{
    if (!dest || !src || !dn || !sn || !n) {
        return;
    }

    while (dn >= 2 && sn >= 2 && n >= 2 && *src) {
        *dest++ = *src++;
        *dest++ = *src++;

        dn -= 2;
        sn -= 2;
        n -= 2;
    }

    *dest = 0;

    // Try to add the secondary byte to the l2 string.
    // Todo: make this code better, maybe we can find a cleaner way?
    if (dn) {
        *(dest + 1) = 0;
    }
}

// Convert n bytes or until NULL terminator of l2_string to char.
// Won't overflow and guarantees NULL terminator.
void l2_string_to_char(char *dest,
                       l2_string_t *src,
                       size_t dn,
                       size_t sn,
                       size_t n)
{
    if (!dest || !src || !dn || !sn || !n) {
        return;
    }

    while (dn && sn >= 2 && n && *src) {
        *dest++ = (char) *src++;

        // Ignore the second byte of each l2 string character.
        src++;

        dn -= 1;
        sn -= 2;
        n -= 1;
    }

    *dest = 0;
}

// Convert n bytes or until NULL terminator of char to l2_string_t.
void l2_string_from_char(l2_string_t *dest,
                         char *src,
                         size_t dn,
                         size_t sn,
                         size_t n)
{
    if (!dest || !src || !dn || !sn || !n) {
        return;
    }

    while (dn >= 2 && sn && n && *src) {
        *dest++ = (unsigned char) *src++;
        *dest++ = 0;

        dn -= 2;
        sn -= 1;
        n -= 1;
    }

    *dest = 0;

    // Try to add the secondary byte to the l2 string.
    // Todo: make this code better, maybe we can find a cleaner way?
    if (dn) {
        *(dest + 1) = 0;
    }
}

size_t l2_string_len(l2_string_t *src, size_t n)
{
    size_t len = 0;

    if (!src || !n) {
        return 0;
    }

    while (n >= 2 && *src) {
        len += 1;
        src += 2;
        n -= 2;
    }

    return len;
}

// Get bytes used for string (including NULL terminator).
size_t l2_string_bytes(l2_string_t *src, size_t n)
{
    if (!src || !n) {
        return 0;
    }
    return (l2_string_len(src, n) + 1) * 2;
}

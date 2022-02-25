#include <assert.h>
#include <stddef.h>
#include "include/platform.h"
#include "include/util.h"

void util_set_zero(void *dest, size_t n)
{
    byte_t *byte_dest = 0;

    if (!dest || !n) {
        return;
    }

    byte_dest = dest;

    for (size_t i = 0; i < n; i += 1) {
        *byte_dest = 0;
        byte_dest += 1;
    }
}

void util_cpy_bytes(void *dest, void *src, size_t dn, size_t sn, size_t n)
{
    byte_t *byte_dest = 0;
    byte_t *byte_src  = 0;

    if (!dest || !src || !dn || !sn || !n) {
        return;
    }

    byte_dest = dest;
    byte_src  = src;

    while (dn && sn && n) {
        *byte_dest = *byte_src;
        byte_dest += 1;
        byte_src += 1;
        dn -= 1;
        sn -= 1;
        n -= 1;
    }
}

int util_cpy_all_bytes(void *dest, void *src, size_t dn, size_t sn, size_t n)
{
    if (!dest || !src || !dn || !sn) {
        return 0;
    }

    if (dn < n || sn < n) {
        return 0;
    }

    if (!n) {
        return 1;
    }

    util_cpy_bytes(dest, src, dn, sn, n);

    return 1;
}

void util_cpy_str(char *dest, char *src, size_t dn, size_t sn)
{
    if (!dest || !src || !dn || !sn) {
        return;
    }

    while (dn && sn && *src) {
        *dest = *src;
        dest += 1;
        src += 1;
        dn -= 1;
        sn -= 1;
    }

    *dest = 0;
}

int util_read_bytes(void *dest, void **src, size_t dn, size_t sn, size_t n)
{
    if (!util_cpy_all_bytes(dest, *src, dn, sn, n)) {
        return 0;
    }

    *((byte_t **) src) += n;
    return 1;
}

int util_list_add(struct list *src, size_t src_len, void *value)
{
    struct list **head     = 0;
    struct list *free_node = 0;

    if (!src || !src_len) {
        return 0;
    }

    head      = &src->head;
    free_node = src + 1;

    while (free_node->value) {
        if ((size_t) (free_node - src) >= src_len) {
            return 0;
        }

        free_node += 1;
    }

    free_node->value = value;
    free_node->next  = *head;

    *head = free_node;

    return 1;
}

int util_list_remove(struct list *src, void *value)
{
    struct list **match    = 0;
    struct list *to_remove = 0;

    if (!src) {
        return 0;
    }

    match = &src->head;

    while (*match && (*match)->value != value) {
        match = &(*match)->next;
    }

    if (!*match) {
        return 0;
    }

    to_remove  = *match;
    *match     = (*match)->next;
    *to_remove = (struct list){ 0 };

    return 1;
}

int util_decode32le(u32_t *dest, byte_t *src, size_t n)
{
    if (!dest || !src || n < 4) {
        return 0;
    }

    *dest = (u32_t) src[0] | ((u32_t) src[1] << 8) | ((u32_t) src[2] << 16) |
            ((u32_t) src[3] << 24);

    return 1;
}

int util_decode32be(u32_t *dest, byte_t *src, size_t n)
{
    if (!dest || !src || n < 4) {
        return 0;
    }

    *dest = (u32_t) src[3] | ((u32_t) src[2] << 8) | ((u32_t) src[1] << 16) |
            ((u32_t) src[0] << 24);

    return 1;
}

int util_encode32le(byte_t *dest, u32_t src, size_t n)
{
    if (!dest || n < 4) {
        return 0;
    }

    dest[0] = (byte_t) src;
    dest[1] = (byte_t) (src >> 8);
    dest[2] = (byte_t) (src >> 16);
    dest[3] = (byte_t) (src >> 24);

    return 1;
}

int util_encode32be(byte_t *dest, u32_t src, size_t n)
{
    if (!dest || n < 4) {
        return 0;
    }

    dest[3] = (byte_t) src;
    dest[2] = (byte_t) (src >> 8);
    dest[1] = (byte_t) (src >> 16);
    dest[0] = (byte_t) (src >> 24);

    return 1;
}

int util_recycle_id_get(size_t *dest, size_t *src)
{
    assert(src);

    *dest = src[0];

    if (src[*dest]) {
        src[0] = src[*dest];
        return 0;
    }

    src[0] = *dest + 1;

    return 1;
}

void util_recycle_id(size_t *src, size_t id)
{
    assert(src);
    src[id] = src[0];
    src[0]  = id;
}

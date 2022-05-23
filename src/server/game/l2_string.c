#include <assert.h>
#include "../../include/util.h"
#include "include/l2_string.h"

void l2_string_from_char(l2_string *dest, i8 *src, size_t n)
{
	assert(dest);
	assert(src);
	while (n && *src) {
		*((i8 *) dest) = *src;
		dest += 1;
		src += 1;
		n -= 1;
	}
	// null terminator
	*dest = 0;
}

void l2_string_to_char(char *dest, l2_string *src, size_t n)
{
	assert(dest);
	assert(src);
	for (size_t i = 0; *((i8 *) src) && i < n; i += 1) {
		*dest = *((i8 *) src);
		dest += 1;
		src += 1;
	}
	// null terminator
	*dest = 0;
}

size_t l2_string_len(l2_string *src)
{
	size_t len = 0;
	assert(src);
	while (*src) {
		src += 1;
		len += 1;
	}
	return len;
}

size_t l2_string_size(l2_string *src)
{
	assert(src);
	// +1 null terminator.
	return (l2_string_len(src) + 1) * 2;
}

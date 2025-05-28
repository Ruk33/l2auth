#define byte(x) "", "%b", (int) (x), ""
#define short(x) "", "%h", (int) (x), ""
#define int(x) "", "%d", (int) (x), ""
#define long(x) "", "%ld", (long long) (x), ""
#define str(x) "", "%s", (x), ""
#define strn(x, n) "", "%*s", (x), (long long) (n), ""
#define strn2(x) strn((x), sizeof(x))
#define packet(...) packet_(__VA_ARGS__, 0)

void packet_va(byte *dest, char *fmt, va_list va)
{
	byte *tail = dest + 2;

	do {
        if (fmt[0] == '%' && fmt[1] == 's') {
        	char *value = va_arg(va, char *);
        	u64 size = string_size(value);
        	copy_memory(tail, value, size);
        	tail += size;
        } else if (fmt[0] == '%' && fmt[1] == '*' && fmt[2] == 's') {
        	char *value = va_arg(va, char *);
        	u64 size = va_arg(va, u64);
        	copy_memory(tail, value, size);
        	tail += size;
        } else if (fmt[0] == '%' && fmt[1] == 'l' && fmt[2] == 'd') {
        	u64 value = va_arg(va, long long);
        	u64 size = sizeof(value);
        	copy_memory(tail, &value, size);
        	tail += size;
        } else if (fmt[0] == '%' && fmt[1] == 'd') {
        	u32 value = va_arg(va, int);
        	u64 size = sizeof(value);
        	copy_memory(tail, &value, size);
        	tail += size;
        } else if (fmt[0] == '%' && fmt[1] == 'h') {
        	u16 value = va_arg(va, int);
        	u64 size = sizeof(value);
        	copy_memory(tail, &value, size);
        	tail += size;
        } else if (fmt[0] == '%' && fmt[1] == 'b') {
        	byte value = va_arg(va, int);
        	*tail++ = value;
        }

        fmt = va_arg(va, char *);
    } while (fmt);

    u32 checksum = 0;

    copy_memory(tail, &checksum, sizeof(checksum));

    tail += sizeof(checksum);

    u16 size = tail - (dest + 2);
    u16 body_padded_size = ((size + 7) & (~7)) + 2;

    copy_memory(dest, &body_padded_size, sizeof(body_padded_size));
}

byte *packet_(char *fmt, ...)
{
	static byte buf[256 kb];

    va_list va;
    va_start(va, fmt);
    packet_va(buf, fmt, va);
    va_end(va);

    return buf;
}

u16 packet_size(byte *packet)
{
	u16 size = 0;
	copy_memory(&size, packet, sizeof(size));
	return size;
}

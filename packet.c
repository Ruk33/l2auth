#define byte(x) \
    "", "%b", (int) (x), ""

#define short(x) \
    "", "%h", (int) (x), ""

#define int(x) \
    "", "%d", (int) (x), ""

#define long(x) \
    "", "%D", (long long) (x), ""

#define str(x) \
    "", "%s", (x), ""

#define bytes(x, n) \
    "", "%S", (x), (u64) (n), ""

#define bytes2(x) \
    bytes((x), sizeof(x))

#define packet(...) \
    packet_(__VA_ARGS__, 0)

#define offset_of(type, field) \
    offsetof(type, field)

#define begin_array(variable, times) \
    "", "%a", (byte *) (variable), (int) (times), ""

#define end_array() \
    "", "%A", ""

#define str_in_array(type, field) \
    "", "%s", (u64) sizeof(type), offset_of(type, field), ""

#define bytes_in_array(type, field, n) \
    "", "%S", (u64) sizeof(type), offset_of(type, field), (u64) (n), ""

#define bytes2_in_array(type, field) \
    strn_in_array(type, field, sizeof(((type *) 0)->field))

#define byte_in_array(type, field) \
    "", "%b", (u64) sizeof(type), offset_of(type, field), ""

#define short_in_array(type, field) \
    "", "%h", (u64) sizeof(type), offset_of(type, field), ""

#define int_in_array(type, field) \
    "", "%d", (u64) sizeof(type), offset_of(type, field), ""

#define long_in_array(type, field) \
    "", "%D", sizeof(type), offset_of(type, field), ""

struct variable {
    union {
        char *string;
        byte _char;
        u16 _short;
        u32 _int;
        u64 _long;
        struct fixed_bytes { byte *buf; int size; } bytes;
    } value;
    enum variable_type {
        string_type,
        byte_type,
        short_type,
        int_type,
        long_type,
        fixed_bytes_type,
        null_type,
    } type;
};

void packet_with_variables(byte *dest, struct variable *variable)
{
    byte *tail = dest + 2;

    int completed = 0;

    while (!completed) {
        switch (variable->type) {
        case string_type: {
            int size = string_size(variable->value.string);
            copy_memory(tail, variable->value.string, size);
            tail += size;
        } break;

        case byte_type: {
            *tail++ = variable->value._char;
        } break;

        case short_type: {
            int size = sizeof(variable->value._short);
            copy_memory(tail, &variable->value._short, size);
            tail += size;
        } break;

        case int_type: {
            int size = sizeof(variable->value._int);
            copy_memory(tail, &variable->value._int, size);
            tail += size;
        } break;

        case long_type: {
            int size = sizeof(variable->value._long);
            copy_memory(tail, &variable->value._long, size);
            tail += size;
        } break;

        case fixed_bytes_type: {
            copy_memory(tail,
                        variable->value.bytes.buf,
                        variable->value.bytes.size);
            tail += variable->value.bytes.size;
        } break;

        case null_type:
        default: {
            completed = 1;
        } break;
        }

        variable++;
    }

    u32 checksum = 0;

    copy_memory(tail, &checksum, sizeof(checksum));

    tail += sizeof(checksum);

    u16 size = tail - (dest + 2);
    u16 body_padded_size = ((size + 7) & (~7)) + 2;

    copy_memory(dest, &body_padded_size, sizeof(body_padded_size));
}

void packet_va(byte *dest, char *fmt, va_list va)
{
	static struct variable variables[64];

    int variable_count = 0;

	do {
        if (fmt[0] == '%' && fmt[1] == 'a') {
            byte *variable = va_arg(va, byte *);
            u32 times = va_arg(va, int);
            u64 stride = 0;

            u64 offsets[16] = {0};
            char *formats[16] = {0};
            u64 sizes[16] = {0};
            u32 fields = 0;

            while (!(fmt[0] == '%' && fmt[1] == 'A')) {
                if (fmt[0] == '%' && fmt[1] == 'b') {
                    stride = va_arg(va, u64);
                    offsets[fields] = va_arg(va, u64);
                    formats[fields] = fmt;
                    fields++;
                } else if (fmt[0] == '%' && fmt[1] == 'h') {
                    stride = va_arg(va, u64);
                    offsets[fields] = va_arg(va, u64);
                    formats[fields] = fmt;
                    fields++;
                } else if (fmt[0] == '%' && fmt[1] == 'd') {
                    stride = va_arg(va, u64);
                    offsets[fields] = va_arg(va, u64);
                    formats[fields] = fmt;
                    fields++;
                } else if (fmt[0] == '%' && fmt[1] == 'D') {
                    stride = va_arg(va, u64);
                    offsets[fields] = va_arg(va, u64);
                    formats[fields] = fmt;
                    fields++;
                } else if (fmt[0] == '%' && fmt[1] == 's') {
                    stride = va_arg(va, u64);
                    offsets[fields] = va_arg(va, u64);
                    formats[fields] = fmt;
                    fields++;
                } else if (fmt[0] == '%' && fmt[1] == 'S') {
                    stride = va_arg(va, u64);
                    offsets[fields] = va_arg(va, u64);
                    formats[fields] = fmt;
                    sizes[fields]   = va_arg(va, u64);
                    fields++;
                }

                fmt = va_arg(va, char *);
            }

            for (u32 i = 0; i < times; i++) {
                for (u32 n = 0; n < fields; n++) {
                    char *fmt = formats[n];
                    u64 offset = (stride * i) + offsets[n];

                    if (fmt[0] == '%' && fmt[1] == 'b') {
                        byte value = 0;
                        copy_memory(&value, variable + offset, sizeof(value));
                        variables[variable_count++] = (struct variable) {
                            .value._char = value,
                            .type = byte_type,
                        };
                    } else if (fmt[0] == '%' && fmt[1] == 'h') {
                        u16 value = 0;
                        copy_memory(&value, variable + offset, sizeof(value));
                        variables[variable_count++] = (struct variable) {
                            .value._short = value,
                            .type = short_type,
                        };
                    } else if (fmt[0] == '%' && fmt[1] == 'd') {
                        u32 value = 0;
                        copy_memory(&value, variable + offset, sizeof(value));
                        variables[variable_count++] = (struct variable) {
                            .value._int = value,
                            .type = int_type,
                        };
                    } else if (fmt[0] == '%' && fmt[1] == 'D') {
                        u64 value = 0;
                        copy_memory(&value, variable + offset, sizeof(value));
                        variables[variable_count++] = (struct variable) {
                            .value._long = value,
                            .type = long_type,
                        };
                    } else if (fmt[0] == '%' && fmt[1] == 's') {
                        char *value = (char *) (variable + offset);
                        variables[variable_count++] = (struct variable) {
                            .value.string = value,
                            .type = string_type,
                        };
                    } else if (fmt[0] == '%' && fmt[1] == 'S') {
                        byte *value = variable + offset;
                        u64 size = sizes[n];
                        variables[variable_count++] = (struct variable) {
                            .value.bytes = { .buf = value, .size = size, },
                            .type = fixed_bytes_type,
                        };
                    }
                }
            }
        } else if (fmt[0] == '%' && fmt[1] == 's') {
        	char *value = va_arg(va, char *);

            variables[variable_count++] = (struct variable) {
                .value.string = value,
                .type = string_type,
            };
        } else if (fmt[0] == '%' && fmt[1] == 'S') {
        	byte *value = va_arg(va, byte *);
        	u64 size = va_arg(va, u64);

            variables[variable_count++] = (struct variable) {
                .value.bytes = { .buf = value, .size = size, },
                .type = fixed_bytes_type,
            };
        } else if (fmt[0] == '%' && fmt[1] == 'D') {
        	u64 value = va_arg(va, long long);

            variables[variable_count++] = (struct variable) {
                .value._long = value,
                .type = long_type,
            };
        } else if (fmt[0] == '%' && fmt[1] == 'd') {
        	u32 value = va_arg(va, int);

            variables[variable_count++] = (struct variable) {
                .value._int = value,
                .type = int_type,
            };
        } else if (fmt[0] == '%' && fmt[1] == 'h') {
        	u16 value = va_arg(va, int);

            variables[variable_count++] = (struct variable) {
                .value._short = value,
                .type = short_type,
            };
        } else if (fmt[0] == '%' && fmt[1] == 'b') {
        	byte value = va_arg(va, int);

            variables[variable_count++] = (struct variable) {
                .value._char = value,
                .type = byte_type,
            };
        }

        fmt = va_arg(va, char *);
    } while (fmt);

    variables[variable_count++] = (struct variable) {
        .value.string = 0,
        .type = null_type,
    };

    packet_with_variables(dest, variables);
}

byte *packet_(char *fmt, ...)
{
	static byte buf[64 kb];

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

u8 packet_type(byte *packet)
{
    return packet[2] & 0xff;
}

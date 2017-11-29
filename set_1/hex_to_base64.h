// hex_to_base64.h
#ifndef HEX_TO_BASE64_H
#define HEX_TO_BASE64_H

/*
 * Returns the int value represented by the hex digit
 * (case-insensitive)
 * e.g. 'a' => 10, 'B' => 11, etc.
 */ 
static inline int hex_val(char c)
{
    if (c >= 'a' && c <= 'f')
    {
        // 10 accounts for 0-9
        return 10 + c - 'a';
    }
    if (c >= 'A' && c <= 'F')
    {
        return 10 + c - 'A';
    }
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    return 0;
}


/*
 * Converts two hexadecimal characters to their int value.
 *
 * Assumes the first two characters of *hex are non-NULL
 * Takes the first character, returns the int value of it, 
 * left shifts by 4 bits.
 * Then, bitwise OR with the int value of the second character.
 * e.g.
 * *hex => '1f'
 * hex_val('1') => 1.
 * 1 << 4 => 10000  <-- left side of bitwise OR
 * 
 * hex_val('f') => 15 => 1111
 * 10000 | 1111 => 11111 => 31
 */
static inline int hex_byte(const char *hex)
{
    return (hex_val(hex[0]) << 4) | hex_val(hex[1]);
}

char* hex_to_base64(char*);

#endif
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "hex_to_base64.h"


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



void get_b64_quads(const char *hex, char *b64quads, int pad)
{  
    char *dec_2_base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char h0 = hex_byte(hex + 0);
    char h1 = hex_byte(hex + 2);
    char h2 = hex_byte(hex + 4);
    char b1 = h0 >> 2;
    char b2 = ((h0 & 0x03) << 4) + (h1 >> 4);
    char b3 = ((h1 & 0x0f) << 2) + (h2 >> 6);
    char b4 = h2 & 0x3f;

    b64quads[0] = dec_2_base64[b1];
    b64quads[1] = dec_2_base64[b2];
    b64quads[2] = pad == 2 && !b3 ? '=' : dec_2_base64[b3];
    b64quads[3] = pad && !b4 ? '=' : dec_2_base64[b4];

    return;
}


/*
 * Converts a hexadecimal formatted string to base64
 */
char* hex_to_base64(char *hex)
{
    int hexstring_size = strlen(hex);
    if (!hexstring_size)
    {
        return NULL;
    }

    if (hexstring_size % 2 != 0)
    {
        return NULL;
    }

    // remove whitespaces from hex string
    char *moveto = NULL;
    char *p = hex;
    while (*p)
    {
        if (*p == ' ')
        {
            if (moveto == NULL)
            {
                moveto = p;
            }
        }

        else
        {
            if (moveto)
            {
                memmove(moveto, p, strlen(p) + 1);
                moveto = NULL;
            }
        }
        p++;
    }

    // allocate memory for base64 output (must be freed by whoever calls this function)
    int b64length = (((hexstring_size + 2) / 3) * 4) + 1;
    char *base64 = calloc(1, b64length + 1);

    // walk through the byte array, converting each 6 hex characters to 4 base64 chars
    char *base64ptr = base64;
    while (hexstring_size > 6)
    {
        get_b64_quads(hex, base64ptr, 0);
        hexstring_size -= 6;
        hex += 6;
        base64ptr += 4;
    }

    if (hexstring_size > 0)
    {
        char temphex[6] = { '0', '0', '0', '0', '0', '0' };
        memcpy(temphex, hex, hexstring_size);
        get_b64_quads(temphex, base64ptr, hexstring_size);
    }

    return base64;
}


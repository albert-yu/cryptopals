// fixed_xor.h

#ifndef FIXED_XOR_H
#define FIXED_XOR_H

#include "hex_to_base64.h"


/*
 * Converts an integer 0-15 to its corresponding hex char.
 * If input is out of this range, we take modulo 16 
 */
static inline char digit_to_hex(int hex_val)
{
    char *hex_digits = "0123456789abcdef";
    const int modulo_val = 16;
    int index = hex_val % modulo_val;
    return hex_digits[index];
}


/*
 * Byte-by-byte XOR on two string inputs of same finite length 
 */
char* fixed_xor(char*, char*);


void prob2_test();

#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fixed_xor.h"
#include "hex_to_base64.h"

char* fixed_xor(char *input1, char *input2)
{
    int length1 = strlen(input1);
    int length2 = strlen(input2);
    if (length1 != length2)
    {
        return "0";
    }

    char *ret_val = calloc(length1, sizeof(char));
    for (int i = 0; i < length1; i++)
    {
        int val = hex_val(input1[i]) ^ hex_val(input2[i]);
        ret_val[i] = digit_to_hex(val);
    }

    return ret_val;
}
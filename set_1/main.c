#include <stdio.h>
#include <stdlib.h>
#include "hex_to_base64.h"
#include "fixed_xor.h"

int main()
{
    char *input1 = "1c0111001f010100061a024b53535009181c";
    char *input2 = "686974207468652062756c6c277320657965";
    char *result = fixed_xor(input1, input2);
    printf("%s", result);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "fixed_xor.h"
// #include "xor_cipher.h"

int main()
{

    char *input1 = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    char *input2 = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *result = fixed_xor(input1, input2);
    printf("%s", result);
    // xor_cipher_caller();
    printf("\n");
    fixed_xor_test();
    return 0;
}
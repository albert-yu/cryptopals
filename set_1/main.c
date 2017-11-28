#include <stdio.h>
#include <stdlib.h>
#include "hex_to_base64.h"

int main()
{
    //printf("Hello World!");
    char* input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char* result = hex_to_base64(input);
    // should produce:
    // SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t
    printf("%s", result);
    free(result);
    return 0;
}
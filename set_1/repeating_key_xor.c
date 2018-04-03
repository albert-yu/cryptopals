#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "repeating_key_xor.h"


/*
 * Convert an ASCII string to bytes, then to hex
 */
char* convert_to_hex(char *input)
{

}


void encrypt(char *input, const char *key, char *output)
{
    // check if output has been allocated at all
    if (!output)
    {
        perror("Need to allocate memory for output.\n");
        exit(EXIT_FAILURE);
    }

    // contains current index
    size_t iter = 0;

    // get the length of the key
    size_t key_len = strlen(key);

    char c;
    while (c = *input)
    {
        // take the modulus
        size_t key_offset = iter % key_len;

        // int val = hex_val(c) 
        char xord = c ^ key[key_offset];
        char xord_hex = digit_to_hex(xord);
        // printf("%c", xord_hex);
        // assuming that output has been allocated
        output[iter] = xord_hex;

        iter++;
        input++;
    }

    // terminate
    output[iter] = '\0';
}


void prob5_test()
{
    printf("Running test for problem 5...\n");
    char *key = "ICE";
    char *stanza = "Burning 'em, if you ain't quick and nimble \nI go crazy when I hear a cymbal";
    char *expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272 \na282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

    // alloc memory for output
    size_t stanza_len = strlen(stanza) * 2 + 1;
    char *actual = (char*) malloc(stanza_len * sizeof(*actual));
    encrypt(stanza, key, actual);
    printf("Actual: %s\n", actual);
}
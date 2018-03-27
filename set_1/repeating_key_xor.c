#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "repeating_key_xor.h"


/*
 * Convert an ASCII string to bytes, then to hex
 */
char* convert_to_hex(char* input)
{

}


void encrypt(char *input, const char *key, char *output)
{
    // contains current index
    size_t iter = 0;

    // get the length of the key
    size_t key_len = strlen(key);

    char c;
    while (c = *input)
    {
        // take the modulus
        size_t key_offset = iter % key_len;

        // assuming that output has been allocated
        output[iter] = c;

        iter++;
        input++;
    }
}


void prob5_test()
{
    printf("Running test for problem 5...\n");
    char *key = "ICE";
    char *stanza = "Burning 'em, if you ain't quick and nimble \nI go crazy when I hear a cymbal";
    char *expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272 \na282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "repeating_key_xor.h"


/*
 * Gets the left hex char for a char
 * e.g. 'B' = 0100 0010, so we would return the 
 * hex digit for 0100 => 4 (in dec) => '4'. 
 */
char get_left_hex_char(char input) {
    char no_lower_half = input >> 4;

    // convert to int because function 
    // accepts that as param
    int as_int = (int) no_lower_half;
    char hex_char = digit_to_hex(as_int);
    return hex_char;
}


/*
 * Gets the right hex char for a char
 * e.g. 'B' = 0100 0010, so we would return the 
 * hex digit for 0010 => 2 (in dec) => '2'. 
 */
char get_right_hex_char(char input) {
    char mask = 15; // 0000 1111
    char no_upper_half = mask & input;
    int as_int = (int) no_upper_half;
    char hex_char = digit_to_hex(as_int);
    return hex_char;
}


void encrypt(const char *input, const char *key, char *output) {
    // check if output has been allocated at all
    if (!output) {
        perror("Need to allocate memory for output.\n");
        exit(EXIT_FAILURE);
    }

    // contains current index
    size_t iter = 0;

    // get the length of the key
    size_t key_len = strlen(key);

    char c;
    while ((c = *input) != 0) {
        // take the modulus
        size_t key_offset = iter % key_len;

        char xord = c ^ key[key_offset];

        // assuming that output has been allocated
        // get the hex digits
        char left_part = get_left_hex_char(xord);
        char right_part = get_right_hex_char(xord);

        // output should be double the size of the input
        output[iter * 2] = left_part;
        output[iter * 2 + 1] = right_part;

        // move pointers
        iter++;
        input++;
    }

    // terminate
    output[iter * 2] = '\0';
}


void prob5_test() {
    printf("Running test for problem 5...\n");
    char *key = "ICE";
    char *stanza = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    char *expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

    // alloc memory for output
    // need double because need 2 hex digits for each 8-bit char
    // plus one for null byte
    size_t stanza_len = strlen(stanza) * 2 + 1;
    char *actual = (char*) malloc(stanza_len * sizeof(*actual));
    encrypt(stanza, key, actual);

    if (strcmp(expected, actual) == 0) {        
        printf("Repeating XOR key test passed.\n");
    }
    else {
        printf("Repeating XOR key failed.\n");
        printf("Expected: \t[%s]\n", expected);
        printf("Actual: \t[%s]\n", actual);
    }
    printf("\n");
    free(actual);
}
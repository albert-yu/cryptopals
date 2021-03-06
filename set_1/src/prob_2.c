#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "prob_2.h"


/*
 * Returns empty string if string lengths are not equal
 */
char* fixed_xor(char *input1, char *input2) {
    int length1 = strlen(input1);
    int length2 = strlen(input2);
    if (length1 != length2) {
        char *empty_str = calloc(1, sizeof(char));
        if (empty_str) {
            // return empty string
            return (char*)calloc(1, sizeof(char));
        }
        // out of memory
        fprintf(stderr, "Out of memory. Exiting...\n");
        exit(1);
    }
    char *ret_val = calloc(length1 + 1, sizeof(*ret_val));
    if (ret_val == NULL) {
        fprintf(stderr, "Out of memoery. Exiting...\n");
        exit(1);
    }
    for (int i = 0; i < length1; i++) {
        int val = hex_val(input1[i]) ^ hex_val(input2[i]);
        ret_val[i] = digit_to_hex(val);
    }
    return ret_val;
}


void fixed_xor_test() {
    char *input1 = "1c0111001f010100061a024b53535009181c";
    char *input2 = "686974207468652062756c6c277320657965";
    char *expected = "746865206b696420646f6e277420706c6179";
    char *result = fixed_xor(input1, input2);

    if (strcmp(expected, result) == 0) {        
        printf("Fixed XOR test passed.\n");
    }
    else {
        printf("Fixed XOR test failed.\n");
        printf("Expected: \t[%s]\n", expected);
        printf("Actual: \t[%s]\n", result);
    }
    if (result)
        free(result);
}

void prob2_test() {
    printf("Running test for problem 2...\n");
    fixed_xor_test();
    printf("\n");
}

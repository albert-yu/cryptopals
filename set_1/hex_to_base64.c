#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "hex_to_base64.h"


/*
 * Converts every 6 hex characters to 4 b64 characters
 */
void get_b64_quads(const char *hex, char *b64quads, int pad) {  
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
char* hex_to_base64(char *hex) {
    int hexstring_size = strlen(hex);
    if (!hexstring_size) {
        return NULL;
    }

    if (hexstring_size % 2 != 0) {
        return NULL;
    }

    // remove whitespaces from hex string
    char *moveto = NULL;
    char *p = hex;
    while (*p) {
        if (*p == ' ') {
            if (moveto == NULL) {
                moveto = p;
            }
        }

        else {
            if (moveto) {
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
    while (hexstring_size > 6) {
        get_b64_quads(hex, base64ptr, 0);
        hexstring_size -= 6;
        hex += 6;
        base64ptr += 4;
    }

    if (hexstring_size > 0) {
        char temphex[6] = { '0', '0', '0', '0', '0', '0' };
        memcpy(temphex, hex, hexstring_size);
        get_b64_quads(temphex, base64ptr, hexstring_size);
    }

    return base64;
}


void prob1_test() {
    printf("Running test for problem 1...\n");
    char *input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char *expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    char *actual = hex_to_base64(input);
    if (strcmp(expected, actual) == 0) {
        printf("Hex to Base64 test passed.\n");
    }
    else {
        printf("Hex to Base64 test failed.\n");
        printf("Expected: \t[%s]\n", expected);
        printf("Actual: \t[%s]\n", actual);
    }
    printf("\n");
}
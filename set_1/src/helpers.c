#include <stdio.h>
#include "helpers.h"

void print_bytes(char *byte_array, size_t arr_len) {
    size_t i;
    char ch;
    for (i = 0; i < arr_len; i++) {
        // printf("%c", blocks[i]);
        ch = byte_array[i];
        printf("%2.2x", ch & 0xff);
    }
}
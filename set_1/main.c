#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
// #include "fixed_xor.h"
#include "xor_cipher.h"
#include "detect_xor.h"

int main()
{
    // fixed_xor_test();
    // char *input1 = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    // char *input2 = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    // char *result = fixed_xor(input1, input2);
    // printf("%s", result);

    prob3_test();
    // char a = '1';
    // char c = 'J';
    // char result = a ^ c;
    // printf("%c\n", result);

    // char *input = "746865206b696420646f6e277420706c6179";
    // char *ascii = hex_to_bytes(input);
    // printf("%s\n", ascii);  // the kid don't play (length = 18)
    // printf("%d\n", strlen(ascii));

    // read_and_print();


    return 0;
}
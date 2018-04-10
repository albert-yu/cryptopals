#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "break_xor.h"

/*
 * Computes the Hamming distance between
 * two strings of equal length.
 * The Hamming distance is just the number of differing bits.
 */
unsigned int hamming(const char *str_1, const char *str_2)
{
    size_t len_1 = strlen(str_1);
    size_t len_2 = strlen(str_2);

    if (len_1 != len_2)
    {
        perror("Hamming distance not defined for strings of differing lengths.\n");
        exit(EXIT_FAILURE);
    }

    unsigned int total = 0;
    char c1, c2;
    while ((c1 = *str_1) && (c2 = *str_2))
    {
        char xord = c1 ^ c2; 

        // count the number of ones, Brian Kernighan's way
        unsigned int count;
        for (count = 0; xord; count++)
        {
            xord &= xord - 1;
        }

        total += count;

        str_1++;
        str_2++;
    }

    return total;
}


void prob6_test()
{
    printf("Running test for problem 6...\n");
    char *string1 = "this is a test";
    char *string2 = "wokka wokka!!!";
    unsigned int dist = hamming(string1, string2);
    unsigned int expected = 37;
    if (dist == expected)
    {
        printf("Hamming distance test passed.\n");
    }
    else
    {
        printf("Repeating XOR key failed.\n");
        printf("Expected: \t[%d]\n", expected);
        printf("Actual: \t[%d]\n", dist);
    }

    printf("\n");
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "break_xor.h"


/*
 * Determine if given integer is a power of 2
 */
bool is_power_of_2(size_t v)
{
    if (v == 0)
        return false;

    bool f;         // the result goes here 
    f = (v & (v - 1)) == 0;
    return f;
}


/*
 * Computes the Hamming distance between
 * two strings of equal length.
 * The Hamming distance is just the number of differing bits.
 */
size_t hamming(const char *str_1, const char *str_2)
{
    size_t len_1 = strlen(str_1);
    size_t len_2 = strlen(str_2);

    if (len_1 != len_2)
    {
        perror("Hamming distance not defined for strings of differing lengths.\n");
        exit(EXIT_FAILURE);
    }

    size_t total = 0;
    char c1, c2;
    while ((c1 = *str_1) && (c2 = *str_2))
    {
        char xord = c1 ^ c2; 

        // count the number of ones, Brian Kernighan's way
        size_t count;
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


/*
 * Read the input file into one long string
 * and remove newlines and carriage returns.
 * Stores the length as well.
 */
char* read_file_as_string(char *filename, size_t *length_out)
{
    // check to make sure length_out is not null
    if (!length_out)
    {
        perror("length_out param must not be null.\n");
        exit(EXIT_FAILURE);
    }

    // open the file
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // allocate memory for string
    size_t BUF_LEN = 256;
    char *ret_val = (char*)calloc(BUF_LEN, sizeof(*ret_val));

    // keep track of character count
    size_t count = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        bool is_escape_char = false;
        switch (ch)
        {
            case '\n':
            case '\r':
            case '\t':
                is_escape_char = true;
                break;

            default:
                break;
        }

        if (is_escape_char)
        {
            continue;
        }

        // check if we've reached the end of the buffer
        if (count == BUF_LEN)
        {
            // create a new buffer that is 2x size
            BUF_LEN *= 2;
            // char *temp_buf = (char*) calloc(BUF_LEN, sizeof(*temp_buf));

            // // copy contents
            // strcpy(temp_buf, ret_val);
            // char *old = ret_val;
            // ret_val = temp_buf;
            // free(old);

            ret_val = (char*) realloc(ret_val, BUF_LEN);
        }

        ret_val[count] = ch;

        count++;
    }

    // handle edge case where
    // the last character occupies
    // the last space in the buffer
    // (preventing null termination)
    if (is_power_of_2(count))
    {
        BUF_LEN *= 2;
        ret_val = (char*) realloc(ret_val, BUF_LEN);
        ret_val[count] = '\0';
    }

    *length_out = count;           

    return ret_val;
}


char* get_b64_lookup()
{
    const int B64_SIZE = 64;
    char *dec_2_base64 = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *decode_b64_lookup = malloc(B64_SIZE * sizeof(char));

    for (int i = 0; i < B64_SIZE; i++)
    {
        decode_b64_lookup[dec_2_base64[i]] = i;
    }

    return decode_b64_lookup;
}


/*
 * Converts a b64 string to byte array
 * @param b64str - the input string encoded in b64
 * @param b64lookup - the table/array that maps 
 *   a b64 char to int (e.g. A -> 0, B -> 1, etc.)
 */
// char* b64_to_bytes(char *b64str, char *b64lookup)
// {
    
// }


/*
 * Try a bunch of different keysizes and find out which yield 
 * the smallest N Hamming distances. Store the results in
 * the keysizelengths array
 */
void get_best_keysizes(char *encrypted, int *keysizelengths, size_t num_keys)
{

}


void prob6_test()
{
    printf("Running test for problem 6...\n");
    char *string1 = "this is a test";
    char *string2 = "wokka wokka!!!";
    size_t dist = hamming(string1, string2);
    size_t expected = 37;
    if (dist == expected)
    {
        printf("Hamming distance test passed.\n");
    }
    else
    {
        printf("Hamming distance test failed.\n");
        printf("Expected: \t[%d]\n", expected);
        printf("Actual: \t[%d]\n", dist);
    }


    // test reading file
    char *filename = "./data/6.txt";
    size_t *num_chars = (size_t*) malloc(sizeof(*num_chars));
    char *long_ass_string = 
        read_file_as_string(filename, num_chars);

    // printf("%s\n", long_ass_string);
    if (num_chars)
        printf("Number of characters: %d\n", *num_chars);
    else
        printf("Failed to read.\n");

    free(long_ass_string);
    free(num_chars);

    printf("\n");
}
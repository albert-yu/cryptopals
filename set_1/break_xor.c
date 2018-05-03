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


unsigned char* get_b64_lookup()
{
    const int B64_SIZE = 64;
    unsigned char *dec_2_base64 = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned char *decode_b64_lookup = 
        calloc(B64_SIZE, sizeof(*decode_b64_lookup));

    for (int i = 0; i < B64_SIZE; i++)
    {
        decode_b64_lookup[dec_2_base64[i]] = i;
    }

    return decode_b64_lookup;
}


/*
 * Converts a b64 string to byte array
 * @param b64str - the input string encoded in b64
 * @param b64length - the size of the b64 string
 * @param b64lookup - the table/array that maps 
 *   a b64 char to int (e.g. A -> 0, B -> 1, etc.)
 */
unsigned char* b64_to_bytes(char *b64str, size_t b64length, unsigned char *b64lookup)
{
    if (b64length % 4 != 0)
    {
        perror("Invalid base-64 string. Must be evenly divisible by 4.\n");
        exit(EXIT_FAILURE);
    }

    size_t outputlen = (b64length / 4) * 3 + 1;
    unsigned char* decoded = (unsigned char*) calloc(outputlen, sizeof(*decoded));

    size_t i = 0;
    size_t j = 0;
    while (i < b64length - 3)
    {
        // every 4 base-64 chars is 3 bytes
        unsigned char b64char1 = b64lookup[b64str[i]];
        unsigned char b64char2 = b64lookup[b64str[i + 1]];
        unsigned char b64char3 = b64lookup[b64str[i + 2]];
        unsigned char b64char4 = b64lookup[b64str[i + 3]];

        // byte 1 consists of all 6 bits of 
        // b64char1 and the first two bits 
        // b64char2
        decoded[j] = (b64char1 << 2) + (b64char2 >> 4);

        // byte 2 is the right 4 bits of b64char2 and
        // left 4 bits of b64char3
        decoded[j + 1] = (b64char2 << 4) + (b64char3 >> 2);

        // byte 3 is the right 2 bits of b64char3 and 
        // all 6 bits of b64 char4
        decoded[j + 2] = (b64char3 << 6) + b64char4;

        j += 3;
        i += 4;
    }
    return decoded;
}


/*
 * Try a bunch of different keysizes and find out which yield 
 * the smallest N Hamming distances. Store the results in
 * the keysizelengths array
 */
void get_best_keysizes(char *encrypted, int *keysizelengths, size_t num_keys)
{
    size_t keysize = 2;
    const size_t MAX_KEYSIZE = 40;
    for (;keysize < MAX_KEYSIZE; keysize++)
    {

    }
}


void prob6_test()
{
    printf("Running test for problem 6...\n");

    // test Hamming distance
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

    if (num_chars)
    {
        printf("Successfully read file.\n");
        printf("Number of characters: %d\n", *num_chars);
    }
    else
    {
        printf("Failed to read.\n");
    }

    // test b64 decoding
    char *b64_encoded = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhp"
                        "cyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Np"
                        "b24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1"
                        "c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFu"
                        "Y2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBp"
                        "bmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdl"
                        "LCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55"
                        "IGNhcm5hbCBwbGVhc3VyZS4=";

    char *expected_decode = "Man is distinguished, not only by his reason, "
                            "but by this singular passion from other animals, "
                            "which is a lust of the mind, that by a perseverance "
                            "of delight in the continued and indefatigable "
                            "generation of knowledge, exceeds the short vehemence "
                            "of any carnal pleasure.";

    unsigned char *b64lookup = get_b64_lookup();
    unsigned char* b64decoded = b64_to_bytes(b64_encoded, strlen(b64_encoded), b64lookup);

    if (strcmp(b64decoded, expected_decode) == 0)
    {
        printf("b64 decode test passed.\n");
    }
    else
    {
        printf("b64 test failed.\n");
        printf("Expected: \t%s\n", expected_decode);
        printf("Actual: \t%s\n", b64decoded);
    }

    // clean up
    free(long_ass_string);
    free(num_chars);

    free(b64lookup);
    free(b64decoded);

    printf("\n");
}
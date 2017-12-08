#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "xor_cipher.h"
#include "fixed_xor.h"
#include "hex_to_base64.h"


char* hex_to_bytes(char *hex_string)
{
    int hex_length = strlen(hex_string);
    // not handling the odd case--yet
    if ((hex_length % 2) != 0)
    {
        return "";
    }

    int ascii_length = hex_length / 2;
    char *byte_array = calloc(ascii_length, sizeof(*byte_array));
    int i = 0;
    int j = 0;
    while (i < hex_length)
    {
        int char_val = hex_to_int(hex_string[i], hex_string[i + 1]);
        byte_array[j] = (char)char_val;
        j += 1;
        i += 2;
    }
    return byte_array;
}


/*
 * Byte-by-byte xor
 */
char* byte_xor(char* string_1, char* string_2)
{
    int len_1 = strlen(string_1);
    int len_2 = strlen(string_2);
    if (len_1 != len_2)
    {
        return "";
    }
    // needs to be freed
    char *retval = malloc(len_1 * sizeof(*retval));
    for (int i = 0; i < len_1; i++)
    {
        retval[i] = string_1[i] ^ string_2[i];
    }

    return retval;
}


char* decode_with_key(char *message, int msg_length, char key)
{   
    // first create string with char repeatedly concatenated
    // with itself   
    char *key_string = malloc(msg_length * sizeof(char));
    for (int i = 0; i < msg_length; i++)
    {
        key_string[i] = key;
    }
    // printf("key:    \t%s\n", key_string);
    // now xor the two   
    char *decoded = byte_xor(message, key_string); 

    // garbage collection
    free(key_string);
    // free(decoded);
    return decoded;
}


/*
 * Calculates number of times a character appears
 * in a given string.
 */
int eval_frequency(char *input, int length)
{ 
    int freq = 0;
    for (int i = 0; i < length; i++)
    {
        // check for common characters
        if (input[i] == 'e' ||
            input[i] == 't' ||
            input[i] == 'a' ||
            input[i] == 'o' ||
            input[i] == 'i' ||
            input[i] == 'n' ||
            input[i] == 's' ||
            input[i] == 'h' ||
            input[i] == 'r' ||
            input[i] == 'd' ||
            input[i] == 'l' ||
            input[i] == 'u' ||
            input[i] == ' '
            )
        {
            freq++;
        }
    }
    printf("%d\n", freq);
    return freq;
}


char* unscramble(char *scrambled)
{   
    char *scrambled_bytes = hex_to_bytes(scrambled);
    int msg_length = strlen(scrambled_bytes);
    // printf("message length: %d\n", msg_length);
    char *candidate_ptr; 
    int max_freq = 0;
    // avoid the null terminator (c = 0)
    for (char c = 1; c != 0; c++)
    {
        // if (c < 0)
        // {          
        //     break; // reached overflow
        // }
        printf("Char: %c\n", c);
        char *decoded = decode_with_key(scrambled_bytes, msg_length, c);
        printf("%s\n", decoded);
        int freq = eval_frequency(decoded, msg_length);
        // printf("freq: %d\n", freq);
        if (freq > max_freq)
        {
            max_freq = freq;
            candidate_ptr = decoded;
            // printf(candidate_ptr);
            // printf("\n");
        }
        else
        {
            // printf("Executing free\n");
            // causes crash on c = 0
            free(decoded);
        }
        printf("\n");
    }

    // choose the one with the highest frequency score
    return candidate_ptr;
}


void prob3_test()
{
    char *scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *unscrambled = unscramble(scrambled);
    printf(unscrambled);
    printf("\n");
    free(unscrambled);
}

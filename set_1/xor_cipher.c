#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "xor_cipher.h"
#include "fixed_xor.h"


char* decode_with_key(char *message, int msg_length, char key)
{   
    // first create string with char repeatedly concatenated
    // with itself   
    char *key_string = malloc(msg_length * sizeof(char));
    for (int i = 0; i < msg_length; i++)
    {
        key_string[i] = key;
    }
    // printf(key_string);
    // printf("\n");
    // now xor the two   
    char *decoded = fixed_xor(message, key_string); 
    free(key_string);
    return decoded;
}


/*
 * Calculates number of times a character appears
 * in a give string.
 */
int eval_frequency(char* input, int length)
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
            input[i] == 'u'
            )
        {
            freq++;
        }
    }
    return freq;
}


char* unscramble(char *scrambled)
{   
    int msg_length = strlen(scrambled);
    char *candidate_ptr = malloc(msg_length * sizeof(*candidate_ptr)); 
    int max_freq = 0;
    for (char c = 1; c <= SCHAR_MAX; c++)
    {
        if (c < 0)
        {          
            break; // reached overflow
        }
        printf("%d\n", c);
        char *decoded = decode_with_key(scrambled, msg_length, c);
        printf("%s\n", decoded);
        int freq = eval_frequency(decoded, msg_length);
        // printf("freq: %d\n", freq);
        if (freq > max_freq)
        {
            max_freq = freq;
            strcpy(candidate_ptr, decoded);
            // printf(candidate_ptr);
            // printf("\n");
        }
        // causes crash on c = 0
        free(decoded);
    }

    // choose the one with the highest frequency score
    return candidate_ptr;
}


void xor_cipher_caller()
{
    char *scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *unscrambled = unscramble(scrambled);
    printf(unscrambled);
    free(unscrambled);
}

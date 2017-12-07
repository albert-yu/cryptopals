#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "xor_cipher.h"
#include "fixed_xor.h"
#include "hex_to_base64.h"


char* hex_to_ascii(char *hex_string)
{
    int hex_length = strlen(hex_string);
    if ((hex_length % 2) != 0)
    {
        return "";
    }

    int ascii_length = hex_length / 2 + 1;
    char *ascii_string = malloc(ascii_length * sizeof(*ascii_string));
    int i = 0;
    int j = 0;
    while (i < hex_length)
    {
        int char_val = hex_to_int(hex_string[i], hex_string[i + 1]);
        //printf("char_val: %d\tchar: %c\n", char_val, char_val);
        ascii_string[j] = (char)char_val;
        j += 1;
        i += 2;
    }
    return ascii_string;
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
    char *decoded = fixed_xor(message, key_string); 
    // printf("decoded:\t%s\n\n", decoded);
    free(key_string);

    return decoded;
}


/*
 * Calculates number of times a character appears
 * in a given string.
 */
int eval_frequency(char *input, int length)
{ 
    int freq = 0;
    // for (int i = 0; i < length; i++)
    // {
    //     // check for common characters
    //     if (input[i] == 'e' ||
    //         input[i] == 't' ||
    //         input[i] == 'a' ||
    //         input[i] == 'o' ||
    //         input[i] == 'i' ||
    //         input[i] == 'n' ||
    //         input[i] == 's' ||
    //         input[i] == 'h' ||
    //         input[i] == 'r' ||
    //         input[i] == 'd' ||
    //         input[i] == 'l' ||
    //         input[i] == 'u'
    //         )
    //     {
    //         freq++;
    //     }
    // }
    return freq;
}


char* unscramble(char *scrambled)
{   
    int msg_length = strlen(scrambled);
    char *candidate_ptr; 
    int max_freq = 0;
    // avoid the null terminator (c = 0)
    for (char c = 1; c != 0; c++)
    {
        if (c < 0)
        {          
            break; // reached overflow
        }
        printf("Char: %d\n", c);
        char *decoded = decode_with_key(scrambled, msg_length, c);
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
        // causes crash on c = 0
        else
        {
            free(decoded);
        }
        printf("\n");
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

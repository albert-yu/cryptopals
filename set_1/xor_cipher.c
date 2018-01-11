#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "xor_cipher.h"



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



/* 
 * Decodes a byte array with a given key with XOR
 */
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
 * Stores the frequency table as an array of long longs.
 * Since characters behave like integers, we can use them as
 * the array indices.
 */
long long* get_frequency_table()
{
    const int NUM_CHARS = 128;
    long long *freq_table = (long long*)malloc(NUM_CHARS * sizeof(*freq_table));

    // initialize everything to 0
    for (int i = 0; i < NUM_CHARS; i++)
    {
        freq_table[i] = 0;
    }

    freq_table[' '] = 700000000;
    freq_table['e'] = 390395169;
    freq_table['t'] = 282039486;
    freq_table['a'] = 248362256;
    freq_table['o'] = 235661502;
    freq_table['i'] = 214822972;
    freq_table['n'] = 214319386;
    freq_table['s'] = 196844692;
    freq_table['h'] = 193607737;
    freq_table['r'] = 184990759;
    freq_table['d'] = 134044565;
    freq_table['l'] = 125951672;
    freq_table['u'] = 88219598;
    freq_table['c'] = 79962026;
    freq_table['m'] = 79502870;
    freq_table['f'] = 72967175;
    freq_table['w'] = 69069021;
    freq_table['g'] = 61549736;
    freq_table['y'] = 59010696;
    freq_table['p'] = 55746578;
    freq_table['b'] = 47673928;
    freq_table['v'] = 30476191;
    freq_table['k'] = 22969448;
    freq_table['x'] = 5574077;
    freq_table['j'] = 4507165;
    freq_table['q'] = 3649838;
    freq_table['z'] = 2456495;

    return freq_table;
}


/*
 * Converts upper-case ASCII char to lower case
 * or leaves it alone if it's already lower case
 */
char lower_ascii(char c)
{
    return (c <= 'Z' && c >= 'A' ? c + 32: c);
}


/*
 * Scores a piece of text for the likelihood that it's English
 */
long long eval_frequency(long long *freq_table, char *input, int length)
{ 
    long long freq = 0;
    for (int i = 0; i < length; i++)
    {
        // check for common characters
        char c = input[i];
        // to lower case
        c = lower_ascii(c);
        // make sure it is available in the table
        if (isalpha(c) || c == ' ')
        {
            long long freq_val = freq_table[c];           
            freq += freq_val;
        }
    }

    // printf("%lli\n", freq);
    return freq;
}


/*
 * Unscrambles the hex string by first converting it to a byte
 * array and XOR'ing it against all possible keys, choosing
 * the one with the highest frequency score.
 * Returns the unscrambled string and stores the key in 
 * char pointer.
 */
char* unscramble(char *scrambled, char *the_key)
{   
    char *scrambled_bytes = hex_to_bytes(scrambled);
    int msg_length = strlen(scrambled_bytes);
    // printf("message length: %d\n", msg_length);
    char *candidate_ptr; 
    long long max_freq = 0;
    long long *freq_table = get_frequency_table();
    // avoid the null terminator (c = 0)
    for (char c = 1; c > 0; c++)
    {
        // printf("Char: %c\n", c);
        char *decoded = decode_with_key(scrambled_bytes, msg_length, c);
        // printf("%s\n", decoded);
        long long freq = eval_frequency(freq_table, decoded, msg_length);
        // printf("freq: %d\n", freq);
        if (freq > max_freq)
        {
            // printf("not calling free\n");
            max_freq = freq;
            candidate_ptr = decoded;
            // set key to the_key
            *the_key = c;
        }
        else
        {
            // printf("c: %c\n", c);
            // causing crash
            // printf("decoded: %s\n", decoded);
            // printf("calling free\n");
            free(decoded);
            // printf("freed\n");
        }
        // printf("\n");
    }

    free(freq_table);
    // choose the one with the highest frequency score
    return candidate_ptr;
}


void prob3_test()
{
    printf("Running test for problem 3...\n");
    char null_term = '\0';
    char *the_key = &null_term;
    char *scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *unscrambled = unscramble(scrambled, the_key);
    if (*the_key != '\0')
    {
        printf("Key: %c\n", *the_key);
        printf("Unscrambled: %s\n", unscrambled);
    }
    else
    {
        printf("Key not found!\n");
    }
    printf("\n");
    free(unscrambled);
}

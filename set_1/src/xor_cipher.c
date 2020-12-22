#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "xor_cipher.h"
#include "helpers.h"


char* empty_str() {
    char *empty = calloc(1, sizeof(*empty));
    empty[0]= '\0';
    return empty;
}


/*
 * Convert hex string to bytes by parsing two chars at a time
 * Need to free resulting pointer's memory
 */
char* hex_to_bytes(char *hex_string) {
    int hex_length = strlen(hex_string);
    // not handling the odd case--yet
    if ((hex_length % 2) != 0) {
        char *empty = empty_str();
        return empty;
    }

    int ascii_length = hex_length / 2;
    char *byte_array = calloc(ascii_length + 1, sizeof(*byte_array));
    if (!byte_array) {
        fprintf(stderr, "Failed to allocate memory. Exiting...\n");
        exit(1);
    }
    int i = 0;
    int j = 0;
    while (i < hex_length) {
        int char_val = hex_to_int(hex_string[i], hex_string[i + 1]);
        byte_array[j] = (char)char_val;
        j += 1;
        i += 2;
    }
    return byte_array;
}


/**
 * Byte-by-byte xor with fixed length
 * 
 * @param s1 byte array
 * @param s2 byte array
 * @param length `s1` and `s2` should be this length
 * 
 * @returns heap-allocated byte array
 */
char* byte_xor(char *s1, char *s2, size_t length) {
    // needs to be freed
    char *retval = malloc(length * sizeof(*retval));
    for (int i = 0; i < length; i++) {
        retval[i] = s1[i] ^ s2[i];
        // printf("\ti: %d\n", i);
    }
    return retval;
}


/*
 * Byte-by-byte xor for strings
 */
char* str_xor(char* string_1, char* string_2) {
    int len_1 = strlen(string_1);
    int len_2 = strlen(string_2);
    if (len_1 != len_2) {
        return empty_str();
    }
    // needs to be freed
    char *retval = malloc((len_1 + 1) * sizeof(*retval));
    for (int i = 0; i < len_1; i++) {
        retval[i] = string_1[i] ^ string_2[i];
    }

    retval[len_1] = '\0';

    return retval;
}


/* 
 * Decodes a byte array with a given key with XOR
 */
char* decode_with_key(char *message, size_t msg_length, char key) {   
    // first create string with char repeatedly concatenated
    // with itself   
    char *key_string = malloc((msg_length + 1) * sizeof(*key_string));
    for (int i = 0; i < msg_length; i++) {
        key_string[i] = key;
    }
    key_string[msg_length] = '\0';
    // printf("key:    \t%s\n", key_string);
    // now xor the two   
    // printf("message: ");
    // print_bytes(message, msg_length);
    char *decoded = byte_xor(message, key_string, msg_length); 
    
    // printf("decoded: ");
    // print_bytes(decoded, msg_length);

    free(key_string);
    return decoded;
}


/*
 * Stores the frequency table as an array of long longs.
 * Since characters behave like integers, we can use them as
 * the array indices.
 */
long long* get_frequency_table() {
    const int NUM_CHARS = 256;
    long long *freq_table = (long long*)calloc(NUM_CHARS, sizeof(*freq_table));

    // // initialize everything to 0
    // for (int i = 0; i < NUM_CHARS; i++)
    // {
    //     freq_table[i] = 0;
    // }

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
char lower_ascii(char c) {
    return (c <= 'Z' && c >= 'A' ? c + 32: c);
}


/*
 * Scores a piece of text for the likelihood that it's English
 */
long long eval_frequency(long long *freq_table, char *input) { 
    long long freq = 0;

    while (*input) {
        char c = *input;
        c = lower_ascii(c);

        if (isalpha(c) || c == ' ') {
            long long freq_val = freq_table[c];
            freq += freq_val;
        }

        input++;
    }

    // printf("%lli\n", freq);
    return freq;
}


long long unscramble_with_len(char* scrambled, char *unscrambled, char *the_key, size_t msg_length) {
    char *candidate_str = calloc(msg_length + 1, sizeof(*candidate_str)); 
    long long max_freq = 0;
    long long *freq_table = get_frequency_table();

    // avoid the null byte
    for (char c = 1; c != 0; c++) {        
        // printf("\tc: %x\n", c);
        char *decoded = decode_with_key(scrambled, msg_length, c);
        long long freq = eval_frequency(freq_table, decoded);
        if (freq > max_freq) {           
            max_freq = freq;
            strcpy(candidate_str, decoded);
            // set key to the_key
            *the_key = c;
        }

        if (decoded) {
            free(decoded);
        }            
    }

    if (freq_table)
        free(freq_table);

    // choose the one with the highest frequency score
    strcpy(unscrambled, candidate_str);
    free(candidate_str);
    return max_freq;
}


/*
 * Unscrambles the byte array by XOR'ing it against all 
 * possible keys, choosing the one with the highest 
 * frequency score.
 * Returns the score of the unscrambled string and stores 
 * the unscrambled string and key in pointers.
 */
long long unscramble(char *scrambled, char *unscrambled, char *the_key) {
    size_t msg_length = strlen(scrambled);
    return unscramble_with_len(scrambled, unscrambled, the_key, msg_length);
}


/*
 * Unscrambles the hex string by first converting it to a byte
 * array and applying the unscramble function
 */
long long hex_unscramble(char *scrambled, char *unscrambled, char *the_key) {   
    char *scrambled_bytes = hex_to_bytes(scrambled);
    long long high_score = unscramble(scrambled_bytes, unscrambled, the_key);
    free(scrambled_bytes);
    return high_score;
}



void prob3_test() {
    printf("Running test for problem 3...\n");
    char null_term = '\0';
    char *the_key = &null_term;
    char *scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *unscrambled = malloc(256 * sizeof(*unscrambled));
    long long score = hex_unscramble(scrambled, unscrambled, the_key);
    if (*the_key != '\0') {
        printf("Key: %c\n", *the_key);
        printf("Unscrambled: %s\n", unscrambled);
    }
    else {
        printf("Key not found!\n");
    }
    printf("\n");
    free(unscrambled);
}

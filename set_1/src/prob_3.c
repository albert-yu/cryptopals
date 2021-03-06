#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "prob_3.h"
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
 * Stores the frequency table as an array of freq_ts.
 * Since characters behave like integers, we can use them as
 * the array indices.
 */
freq_t* get_frequency_table() {
    const int NUM_CHARS = 256;
    freq_t *freq_table = (freq_t*)calloc(NUM_CHARS, sizeof(*freq_table));

    // // initialize everything to 0
    // for (int i = 0; i < NUM_CHARS; i++)
    // {
    //     freq_table[i] = 0;
    // }

    // freq_table[' '] = 700000000;
    // freq_table['e'] = 390395169;
    // freq_table['t'] = 282039486;
    // freq_table['a'] = 248362256;
    // freq_table['o'] = 235661502;
    // freq_table['i'] = 214822972;
    // freq_table['n'] = 214319386;
    // freq_table['s'] = 196844692;
    // freq_table['h'] = 193607737;
    // freq_table['r'] = 184990759;
    // freq_table['d'] = 134044565;
    // freq_table['l'] = 125951672;
    // freq_table['u'] = 88219598;
    // freq_table['c'] = 79962026;
    // freq_table['m'] = 79502870;
    // freq_table['f'] = 72967175;
    // freq_table['w'] = 69069021;
    // freq_table['g'] = 61549736;
    // freq_table['y'] = 59010696;
    // freq_table['p'] = 55746578;
    // freq_table['b'] = 47673928;
    // freq_table['v'] = 30476191;
    // freq_table['k'] = 22969448;
    // freq_table['x'] = 5574077;
    // freq_table['j'] = 4507165;
    // freq_table['q'] = 3649838;
    // freq_table['z'] = 2456495;

    freq_table[' '] = 0.1918182;
    freq_table['e'] = 0.1041442;
    freq_table['t'] = 0.0729357;
    freq_table['a'] = 0.0651738;
    freq_table['o'] = 0.0596302;
    freq_table['i'] = 0.0558094;
    freq_table['n'] = 0.0564513;
    freq_table['s'] = 0.0515760;
    freq_table['h'] = 0.0492888;
    freq_table['r'] = 0.0497563;
    freq_table['d'] = 0.0349835;
    freq_table['l'] = 0.0331490;
    freq_table['u'] = 0.0225134;
    freq_table['c'] = 0.0217339;
    freq_table['m'] = 0.0202124;
    freq_table['f'] = 0.0197881;
    freq_table['w'] = 0.0171272;
    freq_table['g'] = 0.0158610;
    freq_table['y'] = 0.0145984;
    freq_table['p'] = 0.0137645;
    freq_table['b'] = 0.0124248;
    freq_table['v'] = 0.0082903;
    freq_table['k'] = 0.0050529;
    freq_table['x'] = 0.0013692;
    freq_table['j'] = 0.0009033;
    freq_table['q'] = 0.0008606;
    freq_table['z'] = 0.0007836;
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
freq_t eval_frequency(freq_t *freq_table, char *input) { 
    freq_t freq = 0;

    while (*input) {
        char c = *input;
        c = lower_ascii(c);

        if (isalpha(c) || c == ' ') {
            freq_t freq_val = freq_table[c];
            freq += freq_val;
        }

        input++;
    }

    // printf("%lli\n", freq);
    return freq;
}


freq_t unscramble_bytes(char* scrambled, char *unscrambled, char *the_key, size_t msg_length) {
    char *candidate_bytes = calloc(msg_length + 1, sizeof(*candidate_bytes)); 
    freq_t max_freq = 0;
    freq_t *freq_table = get_frequency_table();

    // avoid the null byte
    for (char c = 1; c != 0; c++) {        
        // printf("\tc: %x\n", c);
        char *decoded = decode_with_key(scrambled, msg_length, c);
        freq_t freq = eval_frequency(freq_table, decoded);
        if (freq > max_freq) {           
            max_freq = freq;
            memcpy(candidate_bytes, decoded, msg_length);
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
    memcpy(unscrambled, candidate_bytes, msg_length);
    free(candidate_bytes);
    return max_freq;
}


/*
 * Unscrambles the hex string by first converting it to a byte
 * array and applying the unscramble function
 */
freq_t hex_unscramble(char *hex_scrambled, char *unscrambled, char *the_key) {   
    char *scrambled_bytes = hex_to_bytes(hex_scrambled);
    size_t msg_length = strlen(hex_scrambled) / 2;
    freq_t high_score = unscramble_bytes(scrambled_bytes, unscrambled, the_key, msg_length);
    free(scrambled_bytes);
    return high_score;
}


void prob3_test() {
    printf("Running test for problem 3...\n");
    char null_term = '\0';
    char *the_key = &null_term;
    char *scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *unscrambled = malloc(256 * sizeof(*unscrambled));
    freq_t score = hex_unscramble(scrambled, unscrambled, the_key);
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

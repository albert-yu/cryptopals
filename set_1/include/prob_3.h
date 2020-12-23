#ifndef PROB_3_H
#define PROB_3_H

#include "prob_2.h"

/**
 * Type for frequency score
 */
typedef long long freq_t;

/*
 * Runs the test for Challenge 3, Set 1
 */
void prob3_test();

/**
 * Converts an even-length hex string to raw byte array
 * 
 * @returns heap-allocated byte array
 */
char* hex_to_bytes(char *hex_string);

/**
 * Unscrambles single XOR-encrypted byte array.
 *
 * @param scrambled scrambled bytes
 * @param unscrambled user pointer to unscrambled buffer (same length as scrambled)
 * @param the_key user pointer to the key (single character)
 * @param msg_length the length of the byt array
 * @returns likeness to English text
 */
freq_t unscramble_bytes(char* scrambled, char *unscrambled, char *the_key, size_t msg_length);

/**
 * Unscrambles single XOR-encrypted string (in hex).
 *
 * @param scrambled scrambled bytes
 * @param unscrambled user pointer to unscrambled buffer (same length as scrambled)
 * @param the_key user pointer to the key (single character)
 * @returns likeness to English text
 */
freq_t hex_unscramble(char *scrambled, char *unscrambled, char *the_key);

#endif

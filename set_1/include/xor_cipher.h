#ifndef XOR_CIPHER_H
#define XOR_CIPHER_H

#include "prob_2.h"

/*
 * Runs the test for Challenge 3, Set 1
 */
void prob3_test();

/*
 * Converts an even-lengthed hex string to raw byte array
 */
char* hex_to_bytes(char *hex_string);

/*
 * Unscrambles single XOR-encrypted string.
 * Returns the score of the unscrambled string
 */
long long unscramble_with_len(char* scrambled, char *unscrambled, char *the_key, size_t msg_length);
long long unscramble(char *scrambled, char *unscrambled, char *the_key);

/*
 * Unscrambles single XOR-encrypted string (in hex).
 * Returns the score of the unscrambled string
 */
long long hex_unscramble(char *scrambled, char *unscrambled, char *the_key);


#endif

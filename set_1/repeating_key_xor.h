#ifndef REPEATING_KEY_XOR_H
#define REPEATING_KEY_XOR_H

#include "detect_xor.h"
/*
 * Runs the test for Challenge 5, Set 1
 */
void prob5_test();


/*
 * Encrypt a message with repeating-key XOR
 */
void encrypt(char *input, const char *key, char *output);


#endif
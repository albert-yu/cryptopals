#ifndef PROB_5_H
#define PROB_5_H

#include "prob_4.h"
/*
 * Runs the test for Challenge 5, Set 1
 */
void prob5_test();


/*
 * Encrypt a message with repeating-key XOR
 * and output in hex form
 */
void repeat_xor_encrypt_to_hex(const char *input, const char *key, char *output);


#endif
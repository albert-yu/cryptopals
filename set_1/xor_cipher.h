#ifndef XOR_CIPHER_H
#define XOR_CIPHER_H
/*
 * Runs the test for Challenge 3, Set 1
 */
void prob3_test();

/*
 * Converts an even-lengthed hex string to raw byte array
 */
char* hex_to_bytes(char *hex_string);

/*
 * Unscrambles single XOR-encrypted string
 */
char* unscramble(char *scrambled);

/*
 * Maps characters to their frequency in the English language
 */
long long* get_frequency_table();

/*
 * Scores a piece of text for the likelihood that it's English
 */
long long eval_frequency(long long *freq_table, char *input, int length);

#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "block_array.h"
#include "prob_6.h"


/*
 * Determine if given integer is a power of 2
 */
bool is_power_of_2(size_t v) {
    if (v == 0)
        return false;

    bool f;         // the result goes here 
    f = (v & (v - 1)) == 0;
    return f;
}


/*
 * Counts the number of ones in a byte
 */
size_t count_ones(char c) {
    // count the number of ones, Brian Kernighan's way
    size_t count;
    for (count = 0; c; count++) {
        c &= c - 1;
    }

    return count;
}


/*
 * Computes the hamming distance between two byte arrays.
 * Access violations totally possible.
 * 
 * @param str_1 pointer to start of first string
 * @param str_2 pointer to start of second string
 * @param length length of each string
 */
size_t hamming_with_len(
    const char *str_1, const char *str_2, size_t length) {
    size_t total = 0;
    char c1, c2;
    for (size_t i = 0; i < length; i++) {
        c1 = str_1[i];
        c2 = str_2[i];
        // use XOR to get the number of differing bits
        char xord = c1 ^ c2;
        size_t count = count_ones(xord);
        total += count;
    }

    return total;
}


/*
 * Read the input file into one long string
 * and remove newlines and carriage returns.
 * Stores the length as well.
 */
char* read_file_as_string(char *filename, size_t *length_out) {
    // check to make sure length_out is not null
    if (!length_out) {
        perror("length_out param must not be null.\n");
        exit(EXIT_FAILURE);
    }

    // open the file
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // allocate memory for string
    size_t buf_len = 256;
    char *ret_val = calloc(buf_len, sizeof(*ret_val));

    // keep track of character count
    size_t count = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        bool is_escape_char = false;
        switch (ch) {
            case '\n':
            case '\r':
            case '\t':
                is_escape_char = true;
                break;

            default:
                break;
        }

        if (is_escape_char) {
            continue;
        }

        // check if we've reached the end of the buffer
        if (count == buf_len) {
            buf_len *= 2;
            ret_val = realloc(ret_val, buf_len);
        }

        ret_val[count] = ch;

        count++;
    }

    // handle edge case where
    // the last character occupies
    // the last space in the buffer
    // (preventing null termination)
    if (is_power_of_2(count)) {
        buf_len *= 2;
        ret_val = realloc(ret_val, buf_len);        
    }
    ret_val[count] = '\0';

    *length_out = count;           
    fclose(fp);
    return ret_val;
}


char* get_b64_lookup() {
    const int B64_SIZE = 64;
    char *dec_2_base64 = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *decode_b64_lookup = 
        (char*) calloc(B64_SIZE * 2, sizeof(*decode_b64_lookup));

    for (int i = 0; i < B64_SIZE; i++) {
        decode_b64_lookup[dec_2_base64[i]] = i;
    }

    return decode_b64_lookup;
}


/**
 * Converts a b64 string to byte array
 * @param b64str the input string encoded in b64
 * @param b64length the size of the b64 string
 * @param b64lookup the table/array that maps 
 *    a b64 char to int (e.g. A -> 0, B -> 1, etc.)
 * @param len_ptr pointer to store the length of resulting array  
 * 
 * @returns heap-allocated byte array
 */
char* b64_to_bytes(
    char *b64str, size_t b64length, const char *b64lookup, size_t *len_ptr) {
    if (b64length % 4 != 0) {
        perror("Invalid base-64 string. Must be evenly divisible by 4.\n");
        exit(EXIT_FAILURE);
    }

    size_t outputlen = (b64length / 4) * 3;
    *len_ptr = outputlen;

    char* decoded = (char*) calloc(outputlen + 1, sizeof(*decoded));

    size_t i = 0;
    size_t j = 0;
    while (i < b64length - 3) {
        // every 4 base-64 chars is 3 bytes
        char b64char1 = b64lookup[b64str[i]];
        char b64char2 = b64lookup[b64str[i + 1]];
        char b64char3 = b64lookup[b64str[i + 2]];
        char b64char4 = b64lookup[b64str[i + 3]];

        // byte 1 consists of all 6 bits of 
        // b64char1 and the first two bits 
        // b64char2
        decoded[j] = (b64char1 << 2) + (b64char2 >> 4);

        // byte 2 is the right 4 bits of b64char2 and
        // left 4 bits of b64char3
        decoded[j + 1] = (b64char2 << 4) + (b64char3 >> 2);

        // byte 3 is the right 2 bits of b64char3 and 
        // all 6 bits of b64 char4
        decoded[j + 2] = (b64char3 << 6) + b64char4;

        j += 3;
        i += 4;
    }

    return decoded;
}


/*
 * Get the hamming distance between ith and jth block of size `keysize`
 */
double hamming_i_j(char *input, size_t i, size_t j, size_t keysize) {
    char *start_i, *start_j;
    start_i = input + i * keysize;
    start_j = input + j * keysize;
    
    size_t hamming_dist = hamming_with_len(start_i, start_j, keysize);
    return hamming_dist;
}


/*
 * Score for key size
 */
typedef struct keysize_candidate_t {
    size_t size;
    double score;
} KeySizeCandidate;


/*
 * Comparison function to be used with `qsort` (increasing order)
 */
int keysize_candidates_cmp(const void *a, const void *b) {
    KeySizeCandidate **cand_a, **cand_b;
    cand_a = (KeySizeCandidate**) a;
    cand_b = (KeySizeCandidate**) b;
    double x = (*cand_a)->score;
    double y = (*cand_b)->score;

    if (x < y) 
        return -1;
    else if (x > y) 
        return 1; 
    return 0;
}


/**
 * Free everything (including the key size candidates array itself)
 * 
 * @param candidates array of pointers to candidates
 * @param start start index (inclusive)
 * @param end end index (exclusive)
 */
void keysize_candidates_free(KeySizeCandidate **candidates, size_t start, size_t end) {
    for (size_t i = start; i < end; i++) {
        free(candidates[i]);
    }

    free(candidates);
}


double average_hamming(char *input, size_t keysize) {
    double hamming_0_1, hamming_0_2, hamming_0_3;
    double hamming_1_2, hamming_1_3;
    double hamming_2_3;

    hamming_0_1 = hamming_i_j(input, 0, 1, keysize);
    hamming_0_2 = hamming_i_j(input, 0, 2, keysize);
    hamming_0_3 = hamming_i_j(input, 0, 3, keysize);
    hamming_1_2 = hamming_i_j(input, 1, 2, keysize);
    hamming_1_3 = hamming_i_j(input, 1, 3, keysize);
    hamming_2_3 = hamming_i_j(input, 2, 3, keysize);

    double average = (hamming_0_1 + hamming_0_2 + hamming_0_3 + hamming_1_2 + hamming_1_3 + hamming_2_3) / 6.0;
    return average;
}


/*
 * Get the best keysizes with a different implementation 
 * (compare with four blocks' average Hamming distance instead of two)
 * 
 * @param encrypted the encrypted bytes
 * @param num_keys the number of key sizes to return
 */
size_t* get_best_keysizes(char *encrypted, size_t num_keys) {
    const size_t MIN_KEYSIZE = 2;
    const size_t MAX_KEYSIZE = 40;

    KeySizeCandidate **scores = malloc((MAX_KEYSIZE + 1) * sizeof(*scores));
    size_t keysize = MIN_KEYSIZE;
    for (; keysize <= MAX_KEYSIZE; keysize++) {
        double average = average_hamming(encrypted, keysize);
        KeySizeCandidate* candidate = malloc(sizeof(*candidate));
        candidate->score = average / keysize;
        candidate->size = keysize;
        scores[keysize] = candidate;
        // printf("size: %zu, score: %f\n", candidate->size, candidate->score);
    }

    KeySizeCandidate **arr_start = scores + MIN_KEYSIZE;

    qsort(
        arr_start,
        MAX_KEYSIZE - MIN_KEYSIZE + 1,
        sizeof(*scores),
        keysize_candidates_cmp
    );

    // store the results of the sort
    size_t *result = malloc(num_keys * sizeof(*result));
    for (size_t i = 0; i < num_keys; i++) {
        KeySizeCandidate *candidate = arr_start[i];
        result[i] = candidate->size;
    }

    keysize_candidates_free(scores, MIN_KEYSIZE, MAX_KEYSIZE + 1);

    return result;
}


/**
 * Takes the encrypted bytes and decrypts them with
 * the given key
 * 
 * @param encrypted input byte array
 * @param encrypted_len length of input
 * @param key the key to break it
 * @param output pointer to buffer for output string
 * (should be of size at least encrypted_len + 1) 
 */
void repeat_xor_decrypt(const char *encrypted, size_t encrypted_len, const char *key, char *output) {
    size_t key_len, key_offset;
    key_len = strlen(key);

    char c, key_c, xor_c;
    for (size_t i = 0; i < encrypted_len; i++) {
        c = encrypted[i];
        key_offset = i % key_len;
        key_c = key[key_offset];
        xor_c = c ^ key_c;
        output[i] = xor_c;
    }
    output[encrypted_len] = '\0';
}


/*
 * A key along with its score
 */
typedef struct key_score_t {
    char *key;
    long long score;
} KeyScore;


void key_score_free(KeyScore *key_score) {
    free(key_score->key);
    free(key_score);
}


/*
 * Caller must call `key_score_free()` on result
 */
KeyScore* solve_for_keysize(char *encrypted, size_t encrypted_len, size_t keysize) {
    BlockArray *blocks, *transposed_blocks;
    blocks = make_blocks(encrypted, encrypted_len, keysize);
    transposed_blocks = block_array_transpose(blocks);
    block_array_free(blocks);

    char *the_key, *key_ch_ptr;
    the_key = calloc(keysize, sizeof(*the_key));
    key_ch_ptr = the_key;
    long long score_for_key = 0;

    for (size_t i = 0; i < transposed_blocks->count; i++) {
        char *block = block_array_at(transposed_blocks, i);
        // printf("scrambled: ");
        // print_bytes(block, transposed_blocks->blocksize);
        char *unscrambled = calloc(transposed_blocks->blocksize, sizeof(*unscrambled));
        long long score = unscramble_bytes(
            block,
            unscrambled,
            key_ch_ptr,
            transposed_blocks->blocksize
        );

        // add to score for this key
        score_for_key += score;
        key_ch_ptr++;

        free(block);
        free(unscrambled);
    }
    // put the key together
    KeyScore *key_score = malloc(sizeof(*key_score));
    key_score->key = the_key;
    key_score->score = score_for_key;

    block_array_free(transposed_blocks);
    return key_score;
}


/**
 * Finds the key to the encrypted string
 * 
 * @returns heap-allocated null-terminated string with key
 */
char* break_xor(char *encrypted, size_t encrypted_len) {
    // get 3 best key sizes
    const size_t N_KEYS = 3;
    size_t *best_keysizes = get_best_keysizes(encrypted, N_KEYS);

    long long best_score = 0;

    // allocate memory to hold the key
    const int ARB_SIZE = 64;
    char *best_key = (char*) calloc(ARB_SIZE, sizeof(*best_key));

    for (size_t i = 0; i < N_KEYS; i++) {        
        size_t curr_keysize = best_keysizes[i];
        KeyScore *key_score = solve_for_keysize(encrypted, encrypted_len, curr_keysize);
        // check if we bested the best_score 
        if (key_score->score > best_score) {
            best_score = key_score->score;
            strcpy(best_key, key_score->key);
        }
        key_score_free(key_score);
    }       


    free(best_keysizes); 

    return best_key;
}


void hamming_test() {
    // test Hamming distance
    char *string1 = "this is a test";
    char *string2 = "wokka wokka!!!";
    size_t dist = hamming_with_len(string1, string2, strlen(string1));
    size_t expected = 37;
    if (dist != expected) {
        printf("Hamming distance test failed.\n");
        printf("Expected: \t[%zu]\n", expected);
        printf("Actual: \t[%zu]\n", dist);
        exit(EXIT_FAILURE);
    }
}


void b64_test(const char *b64lookup) {
    // test b64 decoding
    char *b64_encoded = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhp"
                        "cyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Np"
                        "b24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1"
                        "c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFu"
                        "Y2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBp"
                        "bmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdl"
                        "LCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55"
                        "IGNhcm5hbCBwbGVhc3VyZS4=";

    char *expected_decode = "Man is distinguished, not only by his reason, "
                            "but by this singular passion from other animals, "
                            "which is a lust of the mind, that by a perseverance "
                            "of delight in the continued and indefatigable "
                            "generation of knowledge, exceeds the short vehemence "
                            "of any carnal pleasure.";
    size_t bytes_len = 0;
    size_t *bytes_len_ptr = &bytes_len;
    char *b64decoded = b64_to_bytes(
        b64_encoded, strlen(b64_encoded), b64lookup, bytes_len_ptr);

    if (strcmp(b64decoded, expected_decode) != 0) {
        printf("b64 test failed.\n");
        printf("Expected: \t%s\n", expected_decode);
        printf("Actual: \t%s\n", b64decoded);
    }

    free(b64decoded); 
}


void prob6_test() {
    printf("Running test for problem 6...\n");
    hamming_test();

    char *filename = "./data/6.txt";
    size_t b64_len = 0;
    size_t *b64_len_ptr = &b64_len;
    char *file_contents_str = 
        read_file_as_string(filename, b64_len_ptr);
    
    if (!b64_len_ptr) {
        printf("Failed to read file at %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    char *b64lookup = get_b64_lookup();
    b64_test(b64lookup);

    // specify length of byte array
    size_t bytes_len = 0;
    size_t *bytes_len_ptr = &bytes_len;

    // decode input string
    char *all_the_bytes = b64_to_bytes(
        file_contents_str, *b64_len_ptr, b64lookup, bytes_len_ptr);
    free(file_contents_str);
    // free(b64_len_ptr);
    free(b64lookup);
    
    // do the breaking
    char *the_key = break_xor(all_the_bytes, bytes_len);

    // finally, use the key to unscramble
    // the original message
    printf("Key: %s\n", the_key);
    char decrypted [bytes_len + 1];
    repeat_xor_decrypt(all_the_bytes, bytes_len, the_key, decrypted);
    printf("Unscrambled: %s\n", decrypted);
    printf("length of decrypted: %zu", strlen(decrypted));
    printf("\n");
    free(all_the_bytes);
    free(the_key);
}

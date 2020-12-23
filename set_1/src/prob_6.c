#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "prob_6.h"


#ifndef EPSILON
#define EPSILON 0.00000000000000000001
#endif


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
 * Determine if two doubles are equal
 */
bool dbl_equals(double dbl1, double dbl2) {
    return (fabs(dbl1 - dbl2) < EPSILON);
}


/*
 * Returns the substring specified by the start and end indices.
 * End is not inclusive (e.g. 2 to 5 will just return 3 characters). 
 */
char* substring(char *input, size_t start, size_t end) {
    if (start >= end) {
        perror("Cannot have a starting index that is greater than the ending one.\n");
        exit(EXIT_FAILURE);
    }

    size_t size = end - start + 1;      
    char *substr = (char*) calloc(size, sizeof(*substr));    
    size_t substring_index = 0;
    for (size_t i = start; i < end; i++) {
        substr[substring_index] = input[i];
        substring_index++;
    }

    return substr;
}


/*
 * Copies substring to the buffer
 */
void substr_cpy(char *destination, const char *source, size_t start, size_t end) {
    if (start >= end) {
        perror("Cannot have a starting index that is greater than the ending one.\n");
        exit(EXIT_FAILURE);
    }

    size_t substr_i = 0;
    for (size_t i = start; i < end; i++) {
        destination[substr_i] = source[i];
        substr_i++;
    }
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
    size_t BUF_LEN = 256;
    char *ret_val = (char*)calloc(BUF_LEN, sizeof(*ret_val));

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
        if (count == BUF_LEN) {
            BUF_LEN *= 2;
            ret_val = (char*) realloc(ret_val, BUF_LEN);
        }

        ret_val[count] = ch;

        count++;
    }

    // handle edge case where
    // the last character occupies
    // the last space in the buffer
    // (preventing null termination)
    if (is_power_of_2(count)) {
        BUF_LEN *= 2;
        ret_val = (char*) realloc(ret_val, BUF_LEN);        
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


/*
 * Converts a b64 string to byte array
 * @param b64str the input string encoded in b64
 * @param b64length the size of the b64 string
 * @param b64lookup the table/array that maps 
 *    a b64 char to int (e.g. A -> 0, B -> 1, etc.)
 * @param len_ptr pointer to store the length of resulting array  
 */
char* b64_to_bytes(
    char *b64str, size_t b64length, char *b64lookup, size_t *len_ptr) {
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
 * Get the normalized hamming distance between ith and jth block of size `keysize`
 */
double hamming_i_j_normalized(char *encrypted, size_t i, size_t j, size_t keysize) {
    char *start_i, *start_j;
    start_i = encrypted + i;
    start_j = encrypted + j;
    
    size_t hamming_dist = hamming_with_len(start_i, start_j, keysize);
    return hamming_dist / ((double)keysize);
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


/*
 * Get the best keysizes with a different implementation 
 * (compare with four blocks' average Hamming distance instead of two)
 * 
 * @param encrypted the encrypted bytes
 * @param num_keys the number of key sizes to return
 */
size_t* get_best_keysizes2(char *encrypted, size_t num_keys) {
    const size_t MIN_KEYSIZE = 2;
    const size_t MAX_KEYSIZE = 15;

    KeySizeCandidate **scores = malloc((MAX_KEYSIZE + 1) * sizeof(*scores));
    size_t keysize = MIN_KEYSIZE;
    for (; keysize <= MAX_KEYSIZE; keysize++) {
        double hamming_0_1, hamming_0_2, hamming_0_3;
        double hamming_1_2, hamming_1_3;
        double hamming_2_3;

        hamming_0_1 = hamming_i_j_normalized(encrypted, 0, 1, keysize);
        hamming_0_2 = hamming_i_j_normalized(encrypted, 0, 2, keysize);
        hamming_0_3 = hamming_i_j_normalized(encrypted, 0, 3, keysize);
        hamming_1_2 = hamming_i_j_normalized(encrypted, 1, 2, keysize);
        hamming_1_3 = hamming_i_j_normalized(encrypted, 1, 3, keysize);
        hamming_2_3 = hamming_i_j_normalized(encrypted, 2, 3, keysize);

        double average = (hamming_0_1 + hamming_0_2 + hamming_0_3 + hamming_1_2 + hamming_1_3 + hamming_2_3) / 6.0;
        // printf("size: %zu, score: %f\n", keysize, average);

        KeySizeCandidate* candidate = malloc(sizeof(*candidate));
        candidate->score = average;
        candidate->size = keysize;
        scores[keysize] = candidate;
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
        printf("size: %zu, score: %f\n", candidate->size, candidate->score);

        result[i] = candidate->size;
    }

    keysize_candidates_free(scores, MIN_KEYSIZE, MAX_KEYSIZE + 1);

    return result;
}


/*
 * Takes the encrypted bytes and decrypts them with
 * the given key
 */
void decrypt_bytes(const char *encrypted, size_t encrypted_len, const char *key, char *decrypted_ptr) {
    size_t iter, key_len;
    iter = 0;
    key_len = strlen(key);

    char c;
    while (iter < encrypted_len) {
        c = encrypted[iter];
        size_t key_offset = iter % key_len;
        char xord_c = c ^ key[key_offset];
        decrypted_ptr[iter] = xord_c;
        iter++;
        encrypted++;
    }

    decrypted_ptr[iter] = '\0';
    // printf("iter: %zu\n", iter);
    // printf("Str Length: %lu\n", strlen(decrypted_ptr));
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
 * A block array (each byte array is of fixed size)
 */
typedef struct block_array_t {
    char **blocks;
    size_t bufsize;
    size_t count;
    size_t blocksize;
} BlockArray;


/**
 * Allocates memory for a new block array
 * 
 * @param buf_size the initial buffer size
 * @param block_size the width of each block
 * 
 * @returns heap-allocated block array
 */
BlockArray* block_array_alloc(size_t buf_size, size_t block_size) {
    BlockArray *block_arr = malloc(sizeof(*block_arr));
    block_arr->blocks = malloc(buf_size * sizeof(*(block_arr->blocks)));
    block_arr->bufsize = buf_size;
    block_arr->count = 0;
    block_arr->blocksize = block_size;
    return block_arr;
}


/**
 * Appends a copy of the byte array
 */
void block_array_append(BlockArray *block_arr, const char *s) {
    if (block_arr->count == block_arr->bufsize) {
        size_t new_bufsize = block_arr->bufsize * 2;
        block_arr->blocks = realloc(block_arr->blocks, new_bufsize);
        block_arr->bufsize = new_bufsize;
    }
    size_t index = block_arr->count;
    block_arr->blocks[index] = calloc(block_arr->blocksize, sizeof(char));
    memcpy(block_arr->blocks[index], s, block_arr->blocksize);
    block_arr->count++;
}


/**
 * Gets the block at index i
 *
 * @param block_arr the block array
 * @param i the index
 * 
 * @returns heap-allocated byte array (needs to be freed)
 */
char* block_array_at(BlockArray *block_arr, size_t i) {
    if (i >= block_arr->count) {
        printf("Index out of bounds (i: %zu, count: %zu)", i, block_arr->count);
        exit(EXIT_FAILURE);
    }

    char *block = malloc(block_arr->blocksize * sizeof(*block));
    memcpy(block, block_arr->blocks[i], block_arr->blocksize);
    return block;
}


/**
 * Returns the byte given at the jth character of the ith block
 */
char block_array_char_at(BlockArray *block_arr, size_t i, size_t j) {
    if (i >= block_arr->count) {
        printf("Index out of bounds (i: %zu, count: %zu)", i, block_arr->count);
        exit(EXIT_FAILURE);
    }
    if (j >= block_arr->blocksize) {
        printf("Index out of bounds (j: %zu, blocksize: %zu)", j, block_arr->blocksize);
        exit(EXIT_FAILURE);
    }

    return block_arr->blocks[i][j];
}


/**
 * Transposes the blocks
 * 
 * @returns the transposed blocks, heap-allocated
 */
BlockArray* block_array_transpose(BlockArray* orig_blocks) {
    size_t new_block_size = orig_blocks->count;
    size_t new_buf_size = orig_blocks->blocksize;
    BlockArray *new_blocks = block_array_alloc(
        new_buf_size, new_block_size);

    char *ptr;
    for (size_t j = 0; j < orig_blocks->blocksize; j++) {
        char buf[new_block_size];
        ptr = buf;
        for (size_t i = 0; i < orig_blocks->count; i++) {
            *ptr = block_array_char_at(orig_blocks, i, j);
            ptr++;
        }

        block_array_append(new_blocks, buf);
    }

    return new_blocks;
}


void block_array_free(BlockArray *block_arr) {
    if (!block_arr){
        return;
    }

    for (size_t i = 0; i < block_arr->count; i++) {
        free(block_arr->blocks[i]);
    }
    free(block_arr);
}


/**
 * Zeros out a buffer
 */
void zero_buffer(char *buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        buffer[i] = 0;
    }
}


/**
 * Partitions the encrypted string into blocks of fixed size
 * and pads the end with zeroes if not evenly divisible.
 */
BlockArray* make_blocks(char *encrypted, size_t encrypted_len, size_t block_size) {
    // determine number of blocks
    size_t num_blocks, remainder;
    remainder = encrypted_len % block_size;
    if (remainder == 0) {
        num_blocks = encrypted_len / block_size;
    } else {
        num_blocks = encrypted_len / block_size + 1;
    }
    BlockArray *blocks = block_array_alloc(num_blocks, block_size);

    char *encrypted_ptr = encrypted;
    for (size_t i = 0; i < num_blocks; i++) {
        char buf [blocks->blocksize];
        
        // handle special case of remainder
        if (remainder != 0 && i == num_blocks - 1) {
            zero_buffer(buf, blocks->blocksize);
            memcpy(buf, encrypted_ptr, remainder);
        } else {
            // copy as normal
            memcpy(buf, encrypted_ptr, blocks->blocksize);
        }

        block_array_append(blocks, buf);
        encrypted_ptr += block_size;
    }

    return blocks;
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
    printf("key: %s\n", the_key);
    char *decrypted_ptr = calloc(encrypted_len + 1, sizeof(*decrypted_ptr));
    decrypt_bytes(encrypted, encrypted_len, the_key, decrypted_ptr);
    printf("decrypted: %s\n", decrypted_ptr);

    KeyScore *key_score = malloc(sizeof(*key_score));
    key_score->key = the_key;
    key_score->score = score_for_key;
    printf("Score: %lli\n", score_for_key);

    block_array_free(transposed_blocks);
    return key_score;
}


void break_xor(char *encrypted, size_t encrypted_len) {
    // get 3 best key sizes
    const size_t N_KEYS = 3;
    size_t *best_keysizes = get_best_keysizes2(encrypted, N_KEYS);

    long long best_score = 0;

    // allocate memory to hold the key
    const int ARB_SIZE = 64;
    char *best_key = (char*) calloc(ARB_SIZE, sizeof(*best_key));

    for (size_t i = 0; i < N_KEYS; i++) {        
        size_t curr_keysize = best_keysizes[i];
        printf("current keysize: %zu\n", curr_keysize);
        KeyScore *key_score = solve_for_keysize(encrypted, encrypted_len, curr_keysize);
        // check if we bested the best_score 
        if (key_score->score > best_score) {
            best_score = key_score->score;
            strcpy(best_key, key_score->key);
        }
        key_score_free(key_score);
    }       

    if (strlen(best_key)) {
        printf("best key: %s\n", best_key);
    }

    // TODO: finally, use the best key to unscramble
    // the original message

    free(best_key);
    free(best_keysizes); 
}


void prob6_test() {
    printf("Running test for problem 6...\n");

    // test Hamming distance
    char *string1 = "this is a test";
    char *string2 = "wokka wokka!!!";
    size_t dist = hamming_with_len(string1, string2, strlen(string1));
    size_t expected = 37;
    if (dist == expected) {
        printf("Hamming distance test passed.\n");
    }
    else {
        printf("Hamming distance test failed.\n");
        printf("Expected: \t[%zu]\n", expected);
        printf("Actual: \t[%zu]\n", dist);
    }

    // test reading file
    char *filename = "./data/6.txt";
    size_t *b64_len_ptr = (size_t*)malloc(sizeof(*b64_len_ptr));
    char *long_ass_string = 
        read_file_as_string(filename, b64_len_ptr);
    
    if (b64_len_ptr) {
        printf("Successfully read file.\n");
        printf("\tNumber of characters: %zu\n", *b64_len_ptr);
    }
    else {
        printf("Failed to read.\n");
    }

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

    char *b64lookup = get_b64_lookup();

    // specify length of byte array
    size_t bytes_len = 0;
    size_t *bytes_len_ptr = &bytes_len;
    char *b64decoded = b64_to_bytes(
        b64_encoded, strlen(b64_encoded), b64lookup, bytes_len_ptr);

    if (strcmp(b64decoded, expected_decode) == 0) {
        printf("b64 decode test passed.\n");
    }
    else {
        printf("b64 test failed.\n");
        printf("Expected: \t%s\n", expected_decode);
        printf("Actual: \t%s\n", b64decoded);
    }

    // decode input string
    char *all_the_bytes = b64_to_bytes(
        long_ass_string, *b64_len_ptr, b64lookup, bytes_len_ptr);
    
    // print_bytes(all_the_bytes, *bytes_len_ptr);
    
    // do the breaking
    break_xor(all_the_bytes, *bytes_len_ptr);
 
    // clean up
    free(long_ass_string);
    free(b64_len_ptr);
    free(b64decoded); 
    free(b64lookup);
    free(all_the_bytes);

    printf("\n");
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "break_xor.h"


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
 * Computes the Hamming distance between
 * two strings of equal length.
 * The Hamming distance is just the number of differing bits.
 */
size_t hamming(const char *str_1, const char *str_2) {
    size_t len_1 = strlen(str_1);
    size_t len_2 = strlen(str_2);

    if (len_1 != len_2) {
        perror("Hamming distance not defined for strings of differing lengths.\n");
        exit(EXIT_FAILURE);
    }

    size_t total = 0;
    char c1, c2;
    while ((c1 = *str_1) && (c2 = *str_2)) {
        char xord = c1 ^ c2; 
        size_t count = count_ones(xord);
        total += count;

        str_1++;
        str_2++;
    }

    return total;
}


/*
 * Computes the hamming distance between two byte arrays.
 * ASSUMING both are of length @length. 
 * Access violations totally possible.
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
            // create a new buffer that is 2x size
            BUF_LEN *= 2;
            //char *temp_buf = (char*)calloc(BUF_LEN, sizeof(*temp_buf));

            // copy contents
            //strcpy(temp_buf, ret_val);
            //char *old = ret_val;
            //ret_val = temp_buf;
            //free(old);

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
 * @param b64str - the input string encoded in b64
 * @param b64length - the size of the b64 string
 * @param b64lookup - the table/array that maps 
 *    a b64 char to int (e.g. A -> 0, B -> 1, etc.)
 * @param len_ptr - pointer to store the length of resulting array  
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
 * Compare function used for sorting
 * https://stackoverflow.com/a/1791064/9555588
 */
int compare_function(const void *a, const void *b)  {
    double *x = (double *) a;
    double *y = (double *) b;

    if (*x < *y) 
        return -1;
    else if (*x > *y) 
        return 1; 
    return 0;
}


/*
 * Copies the contents of an array of doubles to another
 */
void dblcpy(double *destination, const double *source, size_t length) {
    for (size_t i = 0; i < length; i++) {
	    destination[i] = source[i];
    }
}

/*
 * Given a map from key size to hamming distance and 
 * a hamming distance, find the original key size that
 * is associated with that distance
 * @param keysize_to_hamm - maps the keysizes to the
    hamming distances
 * @param length - number of keysizes
 * @param target - the target hamming distance
 */
size_t get_keysize(double *keysize_to_hamm, size_t length, double target) {
    for (size_t i = 0; i < length; i++) {
        double hamm_dist = keysize_to_hamm[i];
        if (dbl_equals(hamm_dist, target)) {
            return i;
        }
    }
    // cannot have keysize 0, so makes sense
    // to return this as default
    return 0;
}


/*
 * Try a bunch of different keysizes and find out which yield 
 * the smallest N Hamming distances. Store the results in
 * the an array.
 * Assume encrypted string is greater than or equal to 2x the max 
 * keysize length.
 */
size_t* get_best_keysizes(char *encrypted, size_t num_keys) {
    const size_t MIN_KEYSIZE = 2;
    size_t keysize = MIN_KEYSIZE;
    const size_t MAX_KEYSIZE = 40;  

    // hold all the normalized hamming distances
    // and maps key size -> hamming distance
    size_t KEYS_ARR_SIZE = MAX_KEYSIZE + 1;  
    size_t MALLOC_SZ = KEYS_ARR_SIZE * 2;
    double *hammings_lookup =
        (double*) calloc(KEYS_ARR_SIZE, sizeof(*hammings_lookup));
    // printf("foo1\n");
    char *first_n = (char*)calloc(MALLOC_SZ, sizeof(*first_n));
    if (first_n == NULL || hammings_lookup == NULL) {
        fprintf(stderr, "Out of memory. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    // printf("foo2\n");
    char *second_n = (char*)calloc(MALLOC_SZ, sizeof(*second_n));
    if (second_n == NULL) {
        fprintf(stderr, "Could not allocate mem for second_n.\n");
        exit(EXIT_FAILURE);
    }

    for (; keysize <= MAX_KEYSIZE; keysize++) {             
        // printf("%zu\n", keysize);
        // char *first_n = substring(encrypted, 0, keysize);
        // char *second_n = substring(encrypted, keysize, keysize * 2);
        substr_cpy(first_n, encrypted, 0 , keysize);
        substr_cpy(second_n, encrypted, keysize, keysize * 2);

        // printf("1. %s\n", first_n);
        // printf("2. %s\n", second_n);
        
        size_t distance = hamming_with_len(first_n, second_n, keysize);
        double normalized = (distance * 1.0) / keysize;
        // printf("%f\n", normalized);
        hammings_lookup[keysize] = normalized;        
        // free(first_n);
        // free(second_n);
    }

    free(first_n);
    free(second_n);
    
    first_n = NULL;
    second_n = NULL;

    // sort in a new array
    double *sorted_hammings = (double*) calloc(KEYS_ARR_SIZE, sizeof(*sorted_hammings));
    // memcpy(sorted_hammings, hammings_lookup, KEYS_ARR_SIZE);
    dblcpy(sorted_hammings, hammings_lookup, KEYS_ARR_SIZE);    
    qsort(sorted_hammings, KEYS_ARR_SIZE, sizeof(*sorted_hammings), compare_function);

    // for (size_t i = 0; i < KEYS_ARR_SIZE; i++)
    // { 
    //     // printf("%f\n", sorted_hammings[i]);
    //     printf("%zu: %f\n\n", i, hammings_lookup[i]);
    // }

    // store the smallest n key sizes here
    size_t *smallest_n_keysizes =
        (size_t*) calloc(num_keys * 4, sizeof(*smallest_n_keysizes));

    // find the first n nonzero mins
    const double ZERO = 0.0;
    size_t start_i = 0;
    while (dbl_equals(sorted_hammings[start_i], ZERO)) {
        start_i++;
    }
    // printf("%zu\n", start_i);
    for (size_t i = 0; i < num_keys; i++) {
        // could check if we ever reach a buffer overflow,
        // but nah
        double next_min = sorted_hammings[start_i];

        // iterate through all the possible key
        // sizes and find the one that produced
        // this distance
        size_t keysize = get_keysize(hammings_lookup, KEYS_ARR_SIZE, next_min);
        smallest_n_keysizes[i] = keysize;

        start_i++;
    }
    
    free(hammings_lookup); 
    return smallest_n_keysizes;
}


/*
 * Maps N+{0} to (N+{0})^2
 * breadth first (exhaust x-coordinate first)
 * 
 * 0 -> (0, 0)
 * 1 -> (1, 0)
 * 2 -> (2, 0)
 * ...
 * N-1 -> (N-1, 0)
 * N -> (0, 1)
 * N+1 -> (1, 1)
 * etc.
 * 
 * x coord = n % depth
 * y coord = n / depth (floor div)

 * @param n - nonnegative integer
 * @param depth - the maximum value of the x coordinate
 * @param x - the x coordinate of the ordered pair
 * @param y - the y coordinate of the ordered pair
 */
void one_to_2d(size_t n, const size_t depth, size_t *x, size_t *y) {
    *x = n % depth;
    *y = n / depth;
}


/*
 * Given a partition size K, partitions a byte array 
 * A into partitions P such that:
 *
 * P[0] = A[0], A[0 + K], A[0 + 2K],...
 * P[1] = A[1], A[1 + K], A[1 + 2K],...
 * ...
 * P[K-1] = A[K-1], A[K-1 + K], etc...
 *
 * @param str - the byte array from which we are reading
 * @param str_len - the length of the array
 * @param num_partitions - K
 */
char** partition(const char *str, size_t str_len, size_t num_partitions) {
    size_t remainder = str_len % num_partitions;

    // figure out how large each partition needs to be
    size_t partition_sz = remainder == 0 ? 
                        str_len / num_partitions : 
                        str_len / num_partitions + 1;

    // allocate array of pointers
    char **all_blocks = (char**) calloc(num_partitions, sizeof(char*));

    // allocate mem for each partition
    for (size_t i = 0; i < num_partitions; i++) {
        all_blocks[i] = (char*) calloc(partition_sz * 2, sizeof(char));
    }

    // now copy string contents into each partition
    // we do this by defining a bijection from the nonnegative
    // integers to ordered pairs of nonnegative integers
    size_t x = 0;
    size_t y = 0;
    size_t *ptr_x = &x;
    size_t *ptr_y = &y;
    for (size_t i = 0; i < str_len; i++) {
        char c = str[i];
        one_to_2d(i, num_partitions, ptr_x, ptr_y);
        all_blocks[x][y] = c;
    }

    return all_blocks;
}


void break_xor(char *encrypted, size_t encryptd_length) {
    // get 3 best key sizes
    const size_t N_KEYS = 3;
    size_t *best_keysizes = get_best_keysizes(encrypted, N_KEYS);

    long long best_score = 0;

    // allocate memory to hold the key
    const int ARB_SIZE = 64;
    char *best_key = (char*) calloc(ARB_SIZE, sizeof(*best_key));

    for (size_t i = 0; i < N_KEYS; i++) {        
        size_t curr_keysize = best_keysizes[i];
        printf("current keysize: %zu\n", curr_keysize);  
        // FIRST partition the blocks by the keysize
        size_t partition_length = 
            encryptd_length / curr_keysize;

        // array of strings
        char **all_partitions = 
            partition(encrypted, encryptd_length, curr_keysize);

        //     (char**) malloc(curr_keysize * 2 * sizeof(*all_partitions));

        // for (size_t j = 0; j < curr_keysize; j++)
        // {
        //     all_partitions[j] = 
        //         (char*) calloc(partition_length * 2, sizeof(char));
        // }

        // these indices wil keep track of the position
        // within each partition
        // ------------
        // |0|1|2| etc. <- partition 0
        // ------------
        // |0|1|2|      <- partition 1
        // ------------
        // size_t *inner_indices = 
        //     calloc(curr_keysize * 2, sizeof(*inner_indices));

        // // now iterate through the encrypted text to partition it
        // for (size_t k = 0; k < encryptd_length; k++)
        // {
        //     char c = encrypted[k];
        //     size_t outer_i = k % curr_keysize;

        //     size_t inner_i = inner_indices[outer_i];

        //     // copy value to buffer
        //     all_partitions[outer_i][inner_i] = c;

        //     // increment the inner index
        //     inner_indices[outer_i]++;
        // }

        // this will contain the characters of the key
        char *the_key = (char*) calloc(curr_keysize * 8, sizeof(*the_key));
        char *key_ch_ptr = the_key;
        // printf("foo\n");

        long long score_for_key = 0;
        // THEN solve each partition as a single-character XOR
        for (size_t i = 0; i < curr_keysize; i++) {
            // get current partition
            char *partition = all_partitions[i];

            // unscramble it
            char *unscrambled = calloc(partition_length, sizeof(*unscrambled));
            // printf("%zu\n", partition_length);
            long long score = unscramble(partition, unscrambled, key_ch_ptr);
            // add to score for this key
            score_for_key += score;
            key_ch_ptr++;
            // printf("unscrambled: %s\n", unscrambled); 
            free(unscrambled);
        }
        // put the key together
        printf("key: %s\n", the_key);

        // check if we bested the best_score 
        if (score_for_key > best_score) {
            best_score = score_for_key;
            strcpy(best_key, the_key);
        }
       
        // cleanup
        for (size_t j = 0; j < curr_keysize; j++) {
            free(all_partitions[j]);
        }
        
        free(all_partitions);        
        // free(inner_indices);	
        free(the_key); 
    }       

    if (strlen(best_key)) {
        printf("best key: %s\n", best_key);
    }

    // finally, use the best key to unscramble
    // the original message
    // char *plaintext = (char*) calloc(encryptd_length * 2, sizeof(*plaintext));


    free(best_key);
    free(best_keysizes); 
}


/*
 * Print chars one by one because string format
 * terminates at null
 */
void print_bytes(char *byte_array, size_t arr_len) {
    size_t i;
    for (i = 0; i < arr_len; i++) {
        printf("%c", byte_array[i]);
    }
    
    printf("\n");
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

    // test partition
    // size_t num_partitions = 10;
    // char **partitions = partition("012345678901234567890123456789", 30, num_partitions);
    // for (size_t i = 0; i < num_partitions; i++) {
    //     // printf("%s\n", partitions[i]);
    // }


    // printf("%s\n", all_the_bytes);
    // print_bytes(all_the_bytes, bytes_len);

    // do the breaking
    break_xor(all_the_bytes, *bytes_len_ptr);
    // const size_t NUM_KEYS = 3;
    // size_t *best_keysizes = get_best_keysizes(all_the_bytes, NUM_KEYS);
    // for (size_t i = 0; i < NUM_KEYS; i++)
    // {
    //     printf("size: %zu\n", best_keysizes[i]);  
    // }
 
    // clean up
    free(long_ass_string);
    free(b64_len_ptr);
    free(b64decoded); 
    free(b64lookup);
    printf("foo\n");
    free(all_the_bytes);

    printf("\n");
}

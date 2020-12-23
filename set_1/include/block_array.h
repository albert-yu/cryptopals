#ifndef BLOCK_ARRAY_H
#define BLOCK_ARRAY_H

#include <stdlib.h>

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
BlockArray* block_array_alloc(size_t buf_size, size_t block_size);


/**
 * Appends a copy of the byte array
 */
void block_array_append(BlockArray *block_arr, const char *s);


/**
 * Gets the block at index i
 *
 * @param block_arr the block array
 * @param i the index
 * 
 * @returns heap-allocated byte array (needs to be freed)
 */
char* block_array_at(BlockArray *block_arr, size_t i);


/**
 * Returns the byte given at the jth character of the ith block
 */
char block_array_char_at(BlockArray *block_arr, size_t i, size_t j);


/**
 * Transposes the blocks
 * 
 * @returns the transposed blocks, heap-allocated
 */
BlockArray* block_array_transpose(BlockArray* orig_blocks);


/**
 * Prints block array contents as hex
 */
void block_array_print_hex(BlockArray *block_arr);


/**
 * Prints block array assuming string contents
 */
void block_array_print_str(BlockArray *block_arr);


/**
 * Frees heap-allocated block array and element contents
 */
void block_array_free(BlockArray *block_arr);


/**
 * Partitions the byte array into blocks of fixed size
 * and pads the end with zeroes if not evenly divisible.
 * 
 * @param input input byte array
 * @param input_length expected total length of byte array
 * @param block_size fixed block size
 */
BlockArray* make_blocks(char *input, size_t input_length, size_t block_size);

#endif
#include "block_array.h"
#include <stdio.h>
#include <string.h>
#include "helpers.h"


BlockArray* block_array_alloc(size_t buf_size, size_t block_size) {
    BlockArray *block_arr = malloc(sizeof(*block_arr));
    block_arr->blocks = malloc(buf_size * sizeof(*(block_arr->blocks)));
    block_arr->bufsize = buf_size;
    block_arr->count = 0;
    block_arr->blocksize = block_size;
    return block_arr;
}


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


char* block_array_at(BlockArray *block_arr, size_t i) {
    if (i >= block_arr->count) {
        printf("Index out of bounds (i: %zu, count: %zu)", i, block_arr->count);
        exit(EXIT_FAILURE);
    }

    char *block = malloc(block_arr->blocksize * sizeof(*block));
    memcpy(block, block_arr->blocks[i], block_arr->blocksize);
    return block;
}


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


void block_array_print_hex(BlockArray *block_arr) {
    printf("[");
    for (size_t i = 0; i < block_arr->count; i++) {
        char *element = block_arr->blocks[i];
        print_bytes(element, block_arr->blocksize);
        if (i < block_arr->count - 1) {
            printf(", ");
        } 
    }
    printf("]\n");
}


void block_array_print_str(BlockArray *block_arr) {
    printf("[");
    for (size_t i = 0; i < block_arr->count; i++) {
        char *element = block_arr->blocks[i];
        if (i == block_arr->count - 1) {
            printf("%s", element);
        } else {
            printf("%s, ", element);
        }
    }
    printf("]\n");
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


BlockArray* make_blocks(char *input, size_t input_length, size_t block_size) {
    // determine number of blocks
    size_t num_blocks, remainder;
    remainder = input_length % block_size;
    if (remainder == 0) {
        num_blocks = input_length / block_size;
    } else {
        num_blocks = input_length / block_size + 1;
    }
    BlockArray *blocks = block_array_alloc(num_blocks, block_size);

    char *encrypted_ptr = input;
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


/**
 * Tests the transpose functionality
 */
void block_array_transpose_test() {
    char *input = "abcdefghijkl";
    size_t input_len = strlen(input);
    size_t block_size = 4;
    BlockArray *blocks, *transposed;
    blocks = make_blocks(input, input_len, block_size);
    transposed = block_array_transpose(blocks);
    block_array_print_str(transposed);

    block_array_free(blocks);
    block_array_free(transposed);
}


/**
 * Runs tests for block arrays
 */
void block_array_test() {
    block_array_transpose_test();
}

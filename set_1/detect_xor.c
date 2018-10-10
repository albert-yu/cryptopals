#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detect_xor.h"

void copy_over_strings(char **destination, char **source, unsigned int num_strings)
{   
    unsigned int i;
    for (i = 0; i < num_strings; i++)
    {
        char *line = source[i];
        size_t str_size = strlen(line) + 1;  // + 1 for null terminator

        // allocate memory for string
        destination[i] = (char*) calloc((str_size), sizeof(char));

        // copy string
        strcpy(destination[i], source[i]);
    }
}


/*
 * Frees up the memory used by an array of pointers
 */
void delete(char **pointer_arr, unsigned int block_size)
{
    unsigned int i;
    for (i = 0; i < block_size; i++)
    {
        free(pointer_arr[i]);
    }

    free(pointer_arr);
}


/*
 * Reads file into memory. Returns an array of pointers as well as the number of lines.
 * Assumes the length of the lines as 60
 */
char** read_file(char *filename, unsigned long *num_lines)
{
    // open the file
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // file opened successfully, so dynamically allocate memory for it
    const int LINE_BUF_SIZE = 64;    // line_length should be no more than 60
    unsigned long block_size = 256;  // for the array of pointers
    unsigned long curr_position = 0; // actual no. of lines in block
    char **all_lines = (char **)calloc(block_size, sizeof(char*));

    all_lines[curr_position] = 
        (char*)calloc((LINE_BUF_SIZE), sizeof(char));
    int line_ptr_offset = 0;  // points to position within the line

    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            // copy null terminator instead of newline at end of string
            all_lines[curr_position][line_ptr_offset] = '\0';
            curr_position++;

            if (curr_position >= block_size - 1)
            {
                // double the block size to account for more lines
                block_size *= 2;

                // create temp block with bigger size
                char **temp = (char**)calloc(block_size, sizeof(char*));

                // copy over old contents over
                copy_over_strings(temp, all_lines, curr_position); 
                char **old = all_lines;
                all_lines = temp;
                delete(old, curr_position);      
            }

            // allocate more memory
            all_lines[curr_position] = 
                (char*)calloc(LINE_BUF_SIZE,  sizeof(char));

            line_ptr_offset = 0;
            continue;
        }

        // copy character over to array of pointers
        all_lines[curr_position][line_ptr_offset] = ch;
        line_ptr_offset++;
    }
 
    unsigned long line_count = curr_position + 1;
    
    *num_lines = line_count;
    if (fp)
    {
        fclose(fp);
    }
    
    return all_lines;
}



/*
 * Unscrambles an array of strings, choosing the one with 
 * the highest score. Returns the key as well.
 */
char* unscramble_all(char **hex_strings, 
                     unsigned long num_strings, 
                     char *the_key)
{
    // expected string length
    const int EXPECTED_LEN = 60; 

    // keep track of max score
    long long max_score = 0; 

    // keep track of most likely candidate
    char *candidate = 
        (char*)calloc((EXPECTED_LEN + 1), sizeof(*candidate));
    
    // iterate through the strings and calculate the
    // likelihood that it's an English sentence
    unsigned int i = 0;
    for (i = 0; i < num_strings; i++)
    {
        char* hex_str = hex_strings[i];
        if (strlen(hex_str) != EXPECTED_LEN)
        {
            // length must be 60
            continue;
        }

        char null_byte = '\0';
        // will point to current key
        char *curr_key = &null_byte;

        char *unscrambled = (char*)calloc(256, sizeof(*unscrambled));

        long long score = hex_unscramble(hex_str, unscrambled, curr_key);

        // printf("i = %d, score = %lli\n", i, score);       
        if (score > max_score)
        {
            strcpy(candidate, unscrambled);
            *the_key = *curr_key;
            max_score = score;
        }
        if (unscrambled)
        {
            free(unscrambled);
        }
    }

    return candidate;
}


void prob4_test()
{
    printf("Running test for problem 4...\n");

    char *filename = "./data/4.txt";
    unsigned long zero = 0;
    unsigned long *num_lines = &zero;  // store number of lines here
    char **lines = read_file(filename, num_lines);
    
    char null_byte = '\0'; // the XOR cipher never tests 0, so 
                           // we can return this if no suitable
                           // key is found
    char *key = &null_byte;
    char *unscrambled_string = unscramble_all(lines, *num_lines, key);


    if (key)
    {
        printf("Key: %c\n", *key);
    }

    if (unscrambled_string)
    {
        printf("Unscrambled: %s\n", unscrambled_string);
    }
    else
    {
        printf("Unable to unscramble with XOR cipher!\n");
    }

    free(unscrambled_string);
}
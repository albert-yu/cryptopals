#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detect_xor.h"
// #include "xor_cipher.h"

// not sure if this means anything in a non-Unix 
// or non-Linux environment
#ifndef EINVAL
#define EINVAL 22  /* Invalid argument */
#endif

int getline(char **lineptr, size_t *n, FILE *stream);

void read_and_print()
{
    // char ch, file_name[25];
    // FILE *fp;
    
    // // executable is located in a folder, so
    // // need to navigate up a directory
    // strcpy(file_name, "../data/4.txt");
    // fp = fopen(file_name, "r"); // read mode
 
    // if (fp == NULL)
    // {
    //     perror("Error while opening the file.\n");
    //     exit(EXIT_FAILURE);
    // }
 
    // printf("The contents of %s file are :\n", file_name);
    // int curr_position = 0;
    // while ((ch = fgetc(fp)) != EOF )
    // {
    //     printf("%c", ch);
    //     if (ch == '\n')
    //     {
    //         curr_position++;
    //     }
    // }
 
    // printf("\nLine count: %d", curr_position); 
    // fclose(fp);

    //--------------------------------------------------------

    char file_name[25];
    FILE *fp;
    fp = fopen("S:/Projects/cryptopals/set_1/data/4.txt", "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // allocate memory for array of strings
    // const int NUM_STRINGS = 1000;
    const int EXPECTED_LEN = 60;

    // read the file
    size_t len = 0;
    ssize_t line_len;
    char *line = NULL;

    // line count for debugging
    int curr_position = 0;
    while ((line_len = getline(&line, &len, fp)) != -1)
    {       
        printf("Line #: %d\n", curr_position);
        char null_byte = '\0';
        char *temp_key = &null_byte;  // holds the "best" key for this line

        if (line_len != EXPECTED_LEN)
        {
            goto next_iter;
        }
        // copy the line to a temporary buffer
        // to avoid side effects
        char *temp_str = malloc(line_len * sizeof(*temp_str));
        if (temp_str)
        { 
            strcpy(temp_str, line);            
            printf("line: %s\n", temp_str);
            free(temp_str);
        }
        else
        {
            printf("malloc failed! \n");
            break;
        }

        next_iter:
            curr_position++;
    }

    printf("\nLine count: %d", curr_position); 
}


/*
 * Copied from https://dev.w3.org/libwww/Library/src/vms/getline.c
 *
 * Read up to (and including) a newline from STREAM into *LINEPTR
 * (and null-terminate it). *LINEPTR is a pointer returned from malloc (or
 * NULL), pointing to *N characters of space.  It is realloc'd as
 * necessary.  Returns the number of characters read (not including the
 * null terminator), or -1 on error or EOF.
 */
int getline(char **lineptr, size_t *n, FILE *stream)
{
    static char line[256];
    char *ptr;
    unsigned int len;

    if (lineptr == NULL || n == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    if (ferror(stream))
        return -1;

    if (feof(stream))
        return -1;
     
    fgets(line,256,stream);

    ptr = strchr(line,'\n');   
    if (ptr)
        *ptr = '\0';

    len = strlen(line);
   
    if ((len+1) < 256)
    {
        ptr = realloc(*lineptr, 256);
        if (ptr == NULL)
            return(-1);
        *lineptr = ptr;
        *n = 256;
    }

    strcpy(*lineptr,line); 
    return(len);
}


void copy_over_strings(char **destination, char **source, unsigned int num_strings)
{   
    unsigned int i;
    for (i = 0; i < num_strings; i++)
    {
        char *line = source[i];
        size_t str_size = strlen(line) + 1;  // + 1 for null terminator

        // allocate memory for string
        destination[i] = (char*) malloc((str_size) * sizeof(char));

        // copy string
        strcpy(destination[i], source[i]);
        // printf("%s\n", destination[i]);
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
    char **all_lines = (char **) malloc(block_size * sizeof(char*));

    all_lines[curr_position] = 
        (char*) malloc((LINE_BUF_SIZE) * sizeof(char));
    int line_ptr_offset = 0;  // points to position within the line

    char ch;
    while ((ch = fgetc(fp)) != EOF )
    {
        // printf("%c", ch);
        if (ch == '\n')
        {
            // copy null terminator instead of newline at end of string
            all_lines[curr_position][line_ptr_offset] = '\0';
            curr_position++;

            if (curr_position >= block_size - 1)
            {
                // printf("Reallocating\n");
                // double the block size to account for more lines
                block_size *= 2;

                // create temp block with bigger size
                char **temp = (char**) malloc(block_size * sizeof(char*));

                // copy over old contents over
                copy_over_strings(temp, all_lines, curr_position); 
                char **old = all_lines;
                all_lines = temp;
                delete(old, curr_position);      
            }

            // allocate more memory
            all_lines[curr_position] = 
                (char*) malloc((LINE_BUF_SIZE) * sizeof(char));

            line_ptr_offset = 0;
            continue;
        }
        // printf("Line count: %d\n", curr_position);
        // copy character over to array of pointers
        all_lines[curr_position][line_ptr_offset] = ch;
        line_ptr_offset++;
    }
 
    unsigned long line_count = curr_position + 1;
    // printf("\nLine count: %d\n", line_count); 
    
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
        (char*)malloc((EXPECTED_LEN + 1) * sizeof(*candidate));

    // get frequency table used to evaluate 
    // if text is English
    long long *freq_table = get_frequency_table();
    
    // iterate through the strings and calculate the_key
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
        char *unscrambled = (char*) malloc(256 * sizeof(*unscrambled));
        long long score = unscramble(hex_str, unscrambled, curr_key);

        printf("Success so far. i = %d\n", i);
        if (score > max_score && curr_key)
        {
            strcpy(candidate, unscrambled);
            *the_key = *curr_key;
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
    // unscramble_all();
    char *filename = "S:/Projects/cryptopals/set_1/data/4.txt";
    unsigned long zero = 0;
    unsigned long *num_lines = &zero;  // store number of lines here
    char **lines = read_file(filename, num_lines);
    // printf("Number of lines: %d\n", *num_lines);
    // // print out the lines
    // unsigned long i;
    // for (i = 0; i < *num_lines; i++)
    // {
    //     printf("%s\n", lines[i]);
    // }
    
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

    printf("\n");
}
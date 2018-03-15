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
    // int line_count = 0;
    // while ((ch = fgetc(fp)) != EOF )
    // {
    //     printf("%c", ch);
    //     if (ch == '\n')
    //     {
    //         line_count++;
    //     }
    // }
 
    // printf("\nLine count: %d", line_count); 
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
    const int STRING_LEN = 60;

    // read the file
    size_t len = 0;
    ssize_t line_len;
    char *line = NULL;

    // line count for debugging
    int line_count = 0;
    while ((line_len = getline(&line, &len, fp)) != -1)
    {       
        printf("Line #: %d\n", line_count);
        char null_byte = '\0';
        char *temp_key = &null_byte;  // holds the "best" key for this line

        if (line_len != STRING_LEN)
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
            line_count++;
    }

    printf("\nLine count: %d", line_count); 
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


void unscramble_all()
{
    // open the file
    char file_name[25];
    FILE *fp;
    fp = fopen("S:/Projects/cryptopals/set_1/data/4.txt", "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    const int STRING_LEN = 61;  // 60 + null term

    // keep track of most likely candidates
    long long max_score = 0; // keep track of max score
    char *candidate = (char*)malloc(STRING_LEN * sizeof(*candidate));
    char the_key = '\0';  // the XOR cipher never tests 0, so 
                          // we can return this if no suitable
                          // key is found

    // get frequency table used to evaluate 
    // if text is English
    long long *freq_table = get_frequency_table();

    // read the file
    size_t len = 0;
    ssize_t line_len;
    char *line = NULL;

    // line count for debugging
    int line_count = 0;
    while ((line_len = getline(&line, &len, fp)) != -1)
    {       
        printf("Line #: %d\n", line_count);
        char null_byte = '\0';
        char *temp_key = &null_byte;  // holds the "best" key for this line

        if (line_len != STRING_LEN)
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
            // unscramble and store key in temp_key
            char *curr_str = unscramble(temp_str, temp_key);           
            long long curr_score = eval_frequency(freq_table, curr_str, line_len);
            
            // long long curr_score = 1;
            if (curr_score > max_score)
            {
                max_score = curr_score;
                // copy current string to candidate string
                // candidate = curr_str;
                strcpy(candidate, curr_str);
                the_key = *temp_key;
            }
            // else
            // {
            //     free(curr_str);
            // }
            printf("cleanup\n");
            if (curr_str)
                free(curr_str);

            free(temp_str);
        }        

        else
        {
            printf("malloc failed! \n");
            break;
        }

        next_iter:
            line_count++;
    }

    printf("Line count: %d\n", line_count);
    fclose(fp);
    free(line);
    free(freq_table);
    // print out results
    if (the_key != '\0')
    {
        printf("The key is: %c\n", the_key);
        printf("Unscrambled: %s\n", candidate);
    }
    else
    {
        printf("Error: unable to unscramble");
    }
}


void prob4_test()
{
    printf("Running test for problem 4...\n");
    unscramble_all();
    // read_and_print();
    printf("\n");
}
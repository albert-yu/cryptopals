#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detect_xor.h"

void read_and_print()
{
    char ch, file_name[25];
    FILE *fp;
    
    // executable is located in a folder, so
    // need to navigate up a directory
    strcpy(file_name, "../data/4.txt");
    fp = fopen(file_name, "r"); // read mode
 
    if (fp == NULL)
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
 
    printf("The contents of %s file are :\n", file_name);
 
    while ((ch = fgetc(fp)) != EOF )
    {
        printf("%c",ch);
    }
 
    fclose(fp);
}


void gather_and_unscramble()
{
    
}
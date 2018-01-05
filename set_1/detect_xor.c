#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detect_xor.h"

void read_and_print()
{

    // relative file path
    // const char *filename = "testfile.txt";
    // open file in read-only mode
    char ch, file_name[25];
    FILE *fp;
 
    strcpy(file_name, "../data/4.txt");
    printf("filename: %s\n", file_name);
    fp = fopen(file_name,"r"); // read mode
 
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
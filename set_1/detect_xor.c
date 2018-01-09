#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detect_xor.h"

// not sure if this means anything in a non-Unix 
// or non-Linux environment
#ifndef EINVAL
#define EINVAL 22  /* Invalid argument */
#endif

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

   if (ferror (stream))
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


void gather_and_unscramble()
{
    char file_name[25];
    FILE *fp;
    fp = fopen("../data/4.txt", "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    size_t len = 0;
    ssize_t read;
    char *line = NULL;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("Retrieved line of length %d :\n", read);
        printf("%s\n", line);
    }

    fclose(fp);
    free(line);
}
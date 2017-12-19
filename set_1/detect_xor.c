#include <stdio.h>
#include <stdlib.h>

#include "detect_xor.h"

void read_and_print()
{

    // relative file path
    const char *filename = "data/4.txt";
    // open file in read-only mode
    const char* mode = "r";
    FILE *file = fopen(filename, mode);
    printf("Executing method.\n");
    if (file)
    {
        printf("Reading file char by char.\n");
        int c;
        while ((c = getc(file)) != EOF)
        {
            putchar(c);
        }
        fclose(file);
    }
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xor_cipher.h"

void decode()
{
    char* scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
}


char* aggregate_and_choose(char* scrambled)
{
    const char MOST_FREQ_LETTER = 'e';    
    int msg_length = strlen(scrambled)
    // declare array to hold all possibile unscramblings
    const int NUM_STRINGS = 128;
    char all_decoded[NUM_STRINGS][msg_length];  

}

char* decode_with_key(char* message, char key, int msg_length)
{   
    char* decoded = malloc(msg_length * sizeof(char));    

}
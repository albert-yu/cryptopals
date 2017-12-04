#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "xor_cipher.h"
#include "fixed_xor.h"


char* decode_with_key(char* message, int msg_length, char key)
{   
    // first create string with char repeatedly concatenated
    // with itself   
    char* key_string = malloc(msg_length * sizeof(char));
    for (int i = 0; i < msg_length; i++)
    {
    	key_string[i] = key;
    }

    // now xor the two   
    char* decoded = fixed_xor(message, key_string); 
    return decoded;
}


/*
 * Calculates number of times a character appears
 * in a give string.
 */
int eval_frequency(char* input, int length, char c)
{
	int freq = 0;
	for (int i = 0; i < length; i++)
	{
		if (input[i] == c)
		{
			freq++;
		}
	}
	return freq;
}


char* aggregate_and_choose(char* scrambled)
{
    const char MOST_FREQ_LETTER = 'e';    
    int msg_length = strlen(scrambled);

    // declare array to hold all possibile unscramblings
    // int num_chars = (int)(SCHAR_MAX) + 1;
    // char all_decoded[num_chars][msg_length]; 

    char *candidate_ptr; 
    int max_freq = 0;
    for (char c = 0; c <= SCHAR_MAX; c++)
    {
    	printf("%c\n", c);
    	char* decoded = decode_with_key(scrambled, msg_length, c);
    	printf("%s\n", decoded);
    	int freq = eval_frequency(decoded, msg_length, MOST_FREQ_LETTER);
    	// printf("freq: %d\n", freq);
    	if (freq > max_freq)
    	{
    		max_freq = freq;
    		candidate_ptr = decoded;
    		printf(candidate_ptr);
    		printf("\n");
    	}
    	// strcpy(all_decoded[c], decoded);
    }

    // choose the one with the most e's
    return candidate_ptr;
}


void decode()
{
    char* scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char* unscrambled = aggregate_and_choose(scrambled);
    printf(unscrambled);
}

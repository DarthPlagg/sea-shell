#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "colors.h"

char** parse_command(char* raw_input){
    int cap_words = 64;
    int pos_words = 0;
    char** tokens = malloc(cap_words * sizeof(char*));
    char* token = strtok(raw_input, " \t\r\n\a");

    while(token != NULL){
        tokens[pos_words] = token;
        pos_words++;

        // Reallocate if we run out of space for tokens
        if(pos_words >= cap_words-1){
            cap_words += 64;
            char** tmp = realloc(tokens, cap_words * sizeof(char*));
            
            if(tmp == NULL){ 
                fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
                exit(EXIT_FAILURE);
            }
            tokens = tmp;
        }
        token = strtok(NULL, " \t\r\n\a");
    }

    // Null-terminate the list of tokens
    tokens[pos_words] = NULL;
    return tokens;
}
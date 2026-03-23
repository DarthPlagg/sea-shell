#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "colors.h"

int count_pipe(char **input){
    int i = 0, res = 0;
    while(input[i] != NULL){
        if(strcmp(input[i], "|") == 0)
            res++;
        i++;
    }
    return res;
}

/*
 * Splits a single array of tokens into multiple NULL-terminated command arrays in-place.
 * It replaces the "|" token with NULL to terminate the argument list for execvp(),
 * and stores the starting address of each subsequent command in the 'res' array.
 */
char ***parse_pipe(char **input, int pipes){
    pipes++;
    char ***res = malloc(pipes*sizeof(char**));

    int cmd_idx = 0;
    res[0] = &input[0];
    cmd_idx++;
    
    for(int i = 0; input[i] != NULL; i++){
        if(strcmp(input[i], "|") == 0){
            input[i] = NULL; 
            res[cmd_idx] = &input[i + 1];
            cmd_idx++;
        }
    }
    
    return res;
}

char **parse_command(char *raw_input){
    int cap_words = 64;
    int pos_words = 0;
    char **tokens = malloc(cap_words*sizeof(char*));
    char *token = strtok(raw_input, " \t\r\n\a");

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
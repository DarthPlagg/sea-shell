#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "colors.h"

static int count_pipe(char **input) {
    int res = 0;
    for (int i = 0; input[i] != NULL; i++) {
        if (strcmp(input[i], "|") == 0) {
            res++;
        }
    }
    return res;
}

static char **parse_command(char *raw_input) {
    int cap_words = 64;
    int pos_words = 0;
    
    char **tokens = malloc(cap_words * sizeof(char*));
    char *token = strtok(raw_input, " \t\r\n\a");

    while (token != NULL) {
        tokens[pos_words] = token;
        pos_words++;
        // Realocate
        if (pos_words >= cap_words - 1) {
            cap_words += 64;
            char **tmp = realloc(tokens, cap_words * sizeof(char*));
            if (tmp == NULL) { 
                fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
                exit(EXIT_FAILURE);
            }
            tokens = tmp;
        }
        token = strtok(NULL, " \t\r\n\a");
    }
    
    tokens[pos_words] = NULL;
    return tokens;
}

static Command *build_pipeline(char **tokens, int num_commands, int *background) {
    Command *commands = malloc(num_commands * sizeof(Command));
    if (!commands) return NULL;

    int total_tokens = 0;
    while (tokens[total_tokens] != NULL) total_tokens++;
    
    for (int i = 0; i < num_commands; i++) {
        commands[i].args = malloc((total_tokens + 1) * sizeof(char*));
        commands[i].input_file = NULL;
        commands[i].output_file = NULL;
    }

    int cmd_idx = 0;
    int arg_idx = 0;

    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "&") == 0) {
            *background = 1; 

        } else if (strcmp(tokens[i], "<") == 0) {
            if (tokens[i+1] != NULL) {
                commands[cmd_idx].input_file = tokens[i+1];
                i++; 
            }

        } else if (strcmp(tokens[i], ">") == 0) {
            if (tokens[i+1] != NULL) {
                commands[cmd_idx].output_file = tokens[i+1];
                i++;
            }

        } else if (strcmp(tokens[i], "|") == 0) {
            commands[cmd_idx].args[arg_idx] = NULL; 
            cmd_idx++;
            arg_idx = 0;

        } else {
            commands[cmd_idx].args[arg_idx++] = tokens[i];
        }
    }
    
    commands[cmd_idx].args[arg_idx] = NULL;
    return commands;
}

Command *parse_input(char *raw_input, int *num_commands, int *background) {
    if (raw_input == NULL) return NULL;

    char **tokens = parse_command(raw_input);
    if (tokens == NULL || tokens[0] == NULL) {
        free(tokens);
        *num_commands = 0;
        return NULL;
    }

    *num_commands = count_pipe(tokens) + 1;
    *background = 0;

    Command *pipeline = build_pipeline(tokens, *num_commands, background);

    free(tokens);

    return pipeline;
}

void free_pipeline(Command *commands, int num_commands) {
    if (!commands) return;
    for (int i = 0; i < num_commands; i++) {
        free(commands[i].args);
    }
    free(commands);
}
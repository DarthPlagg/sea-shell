#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "colors.h"

// Count number of pipes in the token array
static int count_pipe(char **input) {
    int res = 0;
    for (int i = 0; input[i] != NULL; i++) {
        if (strcmp(input[i], "|") == 0) {
            res++;
        }
    }
    return res;
}

// Split raw input string into an array of tokens
static char **parse_command(char *raw_input) {
    int cap_words = 64;
    int pos_words = 0;
    
    // Allocate initial memory for tokens
    char **tokens = malloc(cap_words * sizeof(char*));
    char *token = strtok(raw_input, " \t\r\n\a");

    while (token != NULL) {
        tokens[pos_words] = token;
        pos_words++;

        // Reallocate memory if the token limit is reached
        if (pos_words >= cap_words - 1) {
            cap_words += 64;
            char **tmp = realloc(tokens, cap_words * sizeof(char*));
            if (tmp == NULL) { 
                fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
                exit(EXIT_FAILURE);
            }
            tokens = tmp;
        }
        // Move to the next token
        token = strtok(NULL, " \t\r\n\a");
    }
    
    // Null-terminate the token array
    tokens[pos_words] = NULL;
    return tokens;
}

// Construct Command structures from raw tokens
static Command *build_pipeline(char **tokens, int num_commands, int *background) {
    Command *commands = malloc(num_commands * sizeof(Command));
    if (!commands) return NULL;

    // Count total tokens to allocate enough space for arguments
    int total_tokens = 0;
    while (tokens[total_tokens] != NULL) total_tokens++;
    
    // Initialize empty Command structures
    for (int i = 0; i < num_commands; i++) {
        commands[i].args = malloc((total_tokens + 1) * sizeof(char*));
        commands[i].input_file = NULL;
        commands[i].output_file = NULL;
    }

    int cmd_idx = 0;
    int arg_idx = 0;

    // Populate commands with arguments and identify special operators
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "&") == 0) {
            *background = 1; // Background process execution
        } else if (strcmp(tokens[i], "<") == 0) {
            if (tokens[i+1] != NULL) {
                commands[cmd_idx].input_file = tokens[i+1];
                i++; // Skip filename token
            }
        } else if (strcmp(tokens[i], ">") == 0) {
            if (tokens[i+1] != NULL) {
                commands[cmd_idx].output_file = tokens[i+1];
                i++; // Skip filename token
            }
        } else if (strcmp(tokens[i], "|") == 0) {
            commands[cmd_idx].args[arg_idx] = NULL; // Terminate current command args
            cmd_idx++;
            arg_idx = 0;
        } else {
            commands[cmd_idx].args[arg_idx++] = tokens[i]; // Regular argument
        }
    }
    
    // Null-terminate the last command's arguments
    commands[cmd_idx].args[arg_idx] = NULL;
    return commands;
}

// Main parsing API: converts raw string to a pipeline of commands
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

    // Free the temporary token array
    free(tokens);

    return pipeline;
}

// Free dynamically allocated memory for the pipeline
void free_pipeline(Command *commands, int num_commands) {
    if (!commands) return;
    for (int i = 0; i < num_commands; i++) {
        free(commands[i].args);
    }
    free(commands);
}
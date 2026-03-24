#ifndef COMMAND_H
#define COMMAND_H

// Structure representing a single command in a pipeline
typedef struct {
    char **args;       // Array of arguments, e.g., ["ls", "-l", NULL]
    char *input_file;  // Filename for '<' redirection (NULL if none)
    char *output_file; // Filename for '>' redirection (NULL if none)
} Command;

#endif
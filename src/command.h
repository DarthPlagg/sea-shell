#ifndef COMMAND_H
#define COMMAND_H

typedef struct {
    char **args;       // e.g., ["ls", "-l", NULL]
    char *input_file;  // For '<' redirection
    char *output_file; // For '>' redirection
} Command;

#endif
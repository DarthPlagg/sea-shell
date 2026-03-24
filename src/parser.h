#ifndef PARSER_H
#define PARSER_H

#include "command.h"

// Main parsing API: converts raw string to a pipeline of commands
Command *parse_input(char *raw_input, int *num_commands, int *background);

// Free dynamically allocated memory for the pipeline
void free_pipeline(Command *commands, int num_commands);

#endif
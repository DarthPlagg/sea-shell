#ifndef PARSER_H
#define PARSER_H

#include "command.h"

// Public API for the parser module
Command *parse_input(char *raw_input, int *num_commands, int *background);
void free_pipeline(Command *commands, int num_commands);

#endif
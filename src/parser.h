#ifndef PARSER_H
#define PARSER_H

char ***parse_pipe(char **input, int pipes);
int count_pipe(char **input);
char** parse_command(char* raw_input);

#endif
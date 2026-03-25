#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "colors.h"

char *get_user_input() {
    int cap_buff = 1024;
    int pos_buff = 0;

    char *input_line = malloc(cap_buff);
    if (input_line == NULL) {
        fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int in = getchar();

        if (in == EOF) {
            if (pos_buff == 0) {
                free(input_line);
                return NULL;
            } else {
                input_line[pos_buff] = '\0';
                return input_line;
            }
        }

        if (in == '\n') {
            input_line[pos_buff] = '\0';
            return input_line;
        }

        input_line[pos_buff] = in;
        pos_buff++;

        if (pos_buff >= cap_buff - 1) {
            cap_buff += 1024;
            char *new_p = realloc(input_line, cap_buff);

            if (new_p == NULL) {
                fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
                exit(EXIT_FAILURE);
            }
            input_line = new_p;
        }
    }
}
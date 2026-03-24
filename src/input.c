#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "colors.h"

// Read a single line of input from the user
char* get_user_input() {
    int cap_buff = 1024;
    int pos_buff = 0;

    // Allocate initial buffer for user input
    char* input_line = malloc(cap_buff);
    if (input_line == NULL) {
        fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    // Read characters one by one
    while (1) {
        int in = getchar();

        // If we got EOF (Ctrl+D)
        if (in == EOF) {
            if (pos_buff == 0) {
                free(input_line);
                return NULL;
            } else {
                // Return what we have so far
                input_line[pos_buff] = '\0';
                return input_line;
            }
        }

        // If we hit Enter, terminate the string and return
        if (in == '\n') {
            input_line[pos_buff] = '\0';
            return input_line;
        }

        // Store the character
        input_line[pos_buff] = in;
        pos_buff++;

        // If we exceed the buffer, reallocate more memory
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
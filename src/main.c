#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "input.h"
#include "parser.h"
#include "executor.h"
#include "colors.h"
#include "command.h"

void run_shell() {
    int status = 1;
    char cwd[1024]; 

    // Main shell loop
    do {
        // Print current working directory
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf(COLOR_GREEN "[%s] " COLOR_RESET, cwd);
        } else {
            perror("seashell");
        }

        // Print shell prompt
        printf(COLOR_BOLD COLOR_CYAN "seashell> " COLOR_RESET);

        // Get raw input from user
        char *raw_input = get_user_input();

        // Exit gracefully on EOF (Ctrl+D)
        if (raw_input == NULL) {
            printf("\n");
            break;
        }
        
        int num_commands = 0;
        int background = 0;
        
        // Parse raw input into a pipeline of commands
        Command *pipeline = parse_input(raw_input, &num_commands, &background);

        // Execute if parsing was successful
        if (pipeline != NULL) {
            status = execute_all(pipeline, num_commands, background);
            free_pipeline(pipeline, num_commands);
        }

        // Free raw input buffer
        free(raw_input);

    } while (status != 0);
}

int main(void) {
    // Start the shell
    run_shell();
    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "input.h"
#include "parser.h"
#include "executor.h"
#include "colors.h"
#include "command.h"

void print_prompt(char *cwd, size_t size) {
    if (getcwd(cwd, size) != NULL) {
        printf(COLOR_GREEN "[%s]" COLOR_RESET COLOR_BOLD COLOR_CYAN " seashell> " COLOR_RESET, cwd);
    } else {
        perror("seashell: getcwd error");
    }
}

void run_shell() {
    int status = 1;
    char cwd[1024]; 

    while (1) {       
        print_prompt(cwd, sizeof(cwd));

        char *raw_input = get_user_input();

        if (raw_input == NULL) {
            printf("\n");
            break;
        }
        
        int num_commands = 0;
        int background = 0;
        
        Command *pipeline = parse_input(raw_input, &num_commands, &background);

        if (pipeline != NULL) {
            status = execute_all(pipeline, num_commands, background);
            free_pipeline(pipeline, num_commands);
        }

        free(raw_input);

        if (status == 0) break;
    }
}

int main(void) {
    run_shell();
    return EXIT_SUCCESS;
}
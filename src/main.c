#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "input.h"
#include "parser.h"
#include "executor.h"
#include "colors.h"
#include "command.h"

void run_shell(){
    int status = 1;
    char cwd[1024]; 

    do{
        // Print current working directory
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf(COLOR_GREEN "[%s] " COLOR_RESET, cwd);
        }else{
            perror("seashell");
        }

        printf(COLOR_BOLD COLOR_CYAN "seashell> " COLOR_RESET);

        // Get and tokenize input
        char *raw_input = get_user_input();
        
        int num_commands = 0;
        int background = 0;
        
        Command *pipeline = parse_input(raw_input, &num_commands, &background);

        if(pipeline != NULL){
            
            status = execute_all(pipeline, num_commands, background);
            free_pipeline(pipeline, num_commands);
        }

        free(raw_input);

    }while(status != 0);
}

int main(void){
    run_shell();
    return EXIT_SUCCESS;
}
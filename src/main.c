#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "input.h"
#include "parser.h"
#include "executor.h"
#include "colors.h"

void run_shell(){
    int status = 1;
    char cwd[1024]; 

    do{
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf(COLOR_GREEN "[%s] " COLOR_RESET, cwd);
        }else{
            perror("seashell");
        }

        printf(COLOR_BOLD COLOR_CYAN "seashell> " COLOR_RESET);

        char *raw_input = get_user_input();
        char **commands = parse_command(raw_input);

        int cnt = count_pipe(commands);
        // There are pipes
        if(cnt > 0){
            char ***pipe_commands = parse_pipe(commands, cnt);
            status = execute_piped(pipe_commands);
            free(pipe_commands);
        }else{
            // There are none
            status = execute_command(commands);
        }
        free(raw_input);
        free(commands);

    }while(status != 0);
}

int main(void){
    run_shell();
    return EXIT_SUCCESS;
}

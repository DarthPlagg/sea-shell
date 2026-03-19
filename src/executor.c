#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "executor.h"
#include "colors.h"

int execute_command(char** args){
    // Protection against empty input
    if(args[0] == NULL){
        return 1;
    }
    
    // Built-in command: exit
    if(strcmp(args[0], "exit") == 0){
        return 0;
    }
    
    // Built-in command: cd
    if(strcmp(args[0], "cd") == 0){
        if(args[1] == NULL){
            fprintf(stderr, COLOR_RED "seashell: expected argument for cd\n" COLOR_RESET);
        } else {
            if(chdir(args[1]) != 0){
                perror(COLOR_RED "seashell" COLOR_RESET);
            }
        }
        return 1; 
    }

    // Built-in command: clear
    if(strcmp(args[0], "clear") == 0){
        printf("\x1b[2J\x1b[H");
        return 1;
    }

    // Executing standard programs
    pid_t pid;
    int exit_status;
    pid = fork();
    
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror(COLOR_RED "seashell" COLOR_RESET);
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0){
        perror(COLOR_RED "seashell" COLOR_RESET);
    } else {
        do {
            waitpid(pid, &exit_status, WUNTRACED);
        } while(!WIFEXITED(exit_status) && !WIFSIGNALED(exit_status));
    }
    return 1;
}
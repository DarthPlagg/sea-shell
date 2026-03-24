#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "executor.h"
#include "colors.h"

int execute_piped(char ***args, int pipes){
    
    int pipefd[2], prev_pipe = -1;
    int n = pipes+1; // Number of commands 
    pid_t pid;

    for(int i = 0; i < n; i++){

        if(i < pipes && pipe(pipefd) < 0){
            perror("seashell: pipe error");
		    return -1;
        }

        pid = fork();
        if(pid < 0){
		    perror("seashell: fork error");
		    return -1;
        }

        if(pid == 0){
            if(prev_pipe != -1){
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }

            if(i < pipes){
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            if(execvp(args[i][0], args[i]) < 0){
                perror("seashell: execution error");
                exit(EXIT_FAILURE);
            }
        }else{
            if(prev_pipe != -1){
                close(prev_pipe);
            }

            if(i < pipes){
                prev_pipe = pipefd[0];
                close(pipefd[1]);
            }
        }
	}

    for(int i = 0; i < n; i++){
        wait(NULL);
    }   
    return 1;
}

int execute_command(char **args){
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
    }else if(pid < 0){
        perror(COLOR_RED "seashell" COLOR_RESET);
    }else{
        do{
            waitpid(pid, &exit_status, WUNTRACED);
        }while(!WIFEXITED(exit_status) && !WIFSIGNALED(exit_status));
    }
    return 1;
}
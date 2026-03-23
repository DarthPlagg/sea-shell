#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "executor.h"
#include "colors.h"

int execute_piped(char ***args){
    // TEMPORARY FOR ONLY ONE PIPE
    char **args1 = args[0];
    char **args2 = args[1];

	int pipefd[2];
	pid_t p1, p2;

	if(pipe(pipefd) < 0){
		perror("seashell: pipe error");
		return -1;
	}	

	p1 = fork();
	if(p1 < 0){
		perror("seashell: fork error");
		return -1;
	}

	if(p1 == 0){
		dup2(pipefd[1], STDOUT_FILENO);

		close(pipefd[0]);
		close(pipefd[1]);

		if(execvp(args1[0], args1) < 0){
			perror("seashell: command 1 execution failed");
			exit(EXIT_FAILURE);
		}
	}
	
	p2 = fork();
	if(p2 < 0){
		perror("seashell: fork error");
		return -1;
	}

	if(p2 == 0){
		dup2(pipefd[0], STDIN_FILENO);
		
		close(pipefd[0]);
		close(pipefd[1]);

        if(execvp(args2[0], args2) < 0){
            perror("seashell: command 2 execution failed");
            exit(EXIT_FAILURE);
        }
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

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
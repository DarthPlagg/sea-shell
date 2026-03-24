#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "executor.h"
#include "colors.h"
#include "command.h"

// Helper: Execute built-in commands (cd, clear, exit)
static int execute_builtin(Command *cmd) {
    char **args = cmd->args;
    
    if (args == NULL || args[0] == NULL) return 2;
    if (strcmp(args[0], "exit") == 0) return 0;
        
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, COLOR_RED "seashell: expected argument for cd\n" COLOR_RESET);
        } else {
            if (chdir(args[1]) != 0) perror("seashell: cd error");
        }
        return 1; 
    }
    
    if (strcmp(args[0], "clear") == 0) {
        printf("\x1b[2J\x1b[H");
        return 1;
    }
    
    return 2;
}

// Helper: Setup file redirections (< and >)
static void setup_redirections(Command *cmd) {
    if (cmd->input_file != NULL) {
        int fd_in = open(cmd->input_file, O_RDONLY);
        if (fd_in < 0) {
            perror("seashell: input error");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if (cmd->output_file != NULL) {
        int fd_out = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0) {
            perror("seashell: output error");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
}

// Helper: Handle waiting for child processes (Foreground/Background)
static void wait_for_children(pid_t last_pid, int num_commands, int background) {
    if (background == 0) {
        // Wait specifically for the last process in the pipeline to finish
        if (last_pid > 0) {
            waitpid(last_pid, NULL, 0);
        }
        
        // Clean up any remaining children to prevent zombies
        while (wait(NULL) > 0); 
    } else {
        printf(COLOR_GREEN "[Background process started with PID %d]\n" COLOR_RESET, last_pid);
    }
}

// Main execution engine
int execute_all(Command *commands, int num_commands, int background) {
    
    if (commands == NULL || num_commands == 0 || commands[0].args == NULL || commands[0].args[0] == NULL) {
        return 1;
    }

    if (num_commands == 1) {
        int tmp = execute_builtin(&commands[0]);
        if (tmp == 0 || tmp == 1) return tmp;
    }

    int pipefd[2], prev_pipe = -1;
    pid_t pid;
    pid_t last_pid = -1;

    for (int i = 0; i < num_commands; i++) {

        if (i < num_commands - 1 && pipe(pipefd) < 0) {
            perror("seashell: pipe error");
            return -1;
        }

        pid = fork();
        if (pid < 0) {
            perror("seashell: fork error");
            return -1;
        }
        
        if (pid == 0) {
            // Child process setup
            if (prev_pipe != -1) {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }
            if (i < num_commands - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            setup_redirections(&commands[i]);

            if (execvp(commands[i].args[0], commands[i].args) < 0) {
                fprintf(stderr, COLOR_RED "seashell: command not found: %s\n" COLOR_RESET, commands[i].args[0]);
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process: save the last PID to wait for it later
            if (i == num_commands - 1) {
                last_pid = pid;
            }
             
            if (prev_pipe != -1) close(prev_pipe);
            if (i < num_commands - 1) {
                prev_pipe = pipefd[0];
                close(pipefd[1]);
            }
        }
    }

    // Wait for foreground or acknowledge background execution
    wait_for_children(last_pid, num_commands, background);
    
    return 1;
}
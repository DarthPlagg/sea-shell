#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// --- ANSI Color Codes ---
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BOLD    "\x1b[1m"

char* get_user_input(){
    int cap_buff = 1024;
    int pos_buff = 0;

    char* input_line = malloc(cap_buff * sizeof(char));
    if(input_line == NULL){
        fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
        exit(EXIT_FAILURE);
    }

    while(1){
        int in;
        in = getchar();

        // If we got EOF (Ctrl+D)
        if(in == EOF){
            if(pos_buff == 0){
                printf("\n"); 
                exit(EXIT_SUCCESS); // Zmieniłem na SUCCESS, bo Ctrl+D to normalne wyjście
            } else {
                input_line[pos_buff] = '\0';
                return input_line;
            }
        }

        if(in == '\n'){
            input_line[pos_buff] = '\0';
            return input_line;
        }

        input_line[pos_buff] = in;
        pos_buff++;

        // If we exceed the buffer, reallocate more memory
        if(pos_buff >= cap_buff-1){
            cap_buff += 1024;
            char* new_p = realloc(input_line, cap_buff * sizeof(char));

            if(new_p == NULL){
                 fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
                 exit(EXIT_FAILURE);
            }
            input_line = new_p;
        }
    }
}

char** parse_command(char* raw_input){
    int cap_words = 64;
    int pos_words = 0;
    char** tokens = malloc(cap_words * sizeof(char*));
    char* token = strtok(raw_input, " \t\r\n\a");

    while(token != NULL){
        tokens[pos_words] = token;
        pos_words++;

        // Reallocate if we run out of space for tokens
        if(pos_words >= cap_words-1){
            cap_words += 64;

            char** tmp = realloc(tokens, cap_words * sizeof(char*));
            // Handle potential errors when reading the directory
            if(tmp == NULL){ 
                fprintf(stderr, COLOR_RED "seashell: allocation error\n" COLOR_RESET);
                exit(EXIT_FAILURE);
            }

            tokens = tmp;
        }
        token = strtok(NULL, " \t\r\n\a");
    }

    // Null-terminate the list of tokens
    tokens[pos_words] = NULL;
    return tokens;
}

int execute_command(char** args){
    // Protection against empty input (just pressing Enter)
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
        return 1; // Return here so we don't fork for 'cd'!
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
        // Child process
        if(execvp(args[0], args) == -1){
            perror(COLOR_RED "seashell" COLOR_RESET);
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0){
        // Forking error
        perror(COLOR_RED "seashell" COLOR_RESET);
    } else {
        // Parent process waits for the child to finish
        do {
            waitpid(pid, &exit_status, WUNTRACED);
        } while(!WIFEXITED(exit_status) && !WIFSIGNALED(exit_status));
    }
    return 1;
}

void run_shell(){
    int status = 1;
    char cwd[1024]; // Buffer to store the current working directory

    do{
        // Get current working directory and print it
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            // Print the path in green inside square brackets
            printf(COLOR_GREEN "[%s] " COLOR_RESET, cwd);
        } else {
            // Handle potential errors when reading the directory
            perror("seashell");
        }

        printf(COLOR_BOLD COLOR_CYAN "seashell> " COLOR_RESET);

        char* raw_input = get_user_input();
        char** commands = parse_command(raw_input);
        status = execute_command(commands);

        // Free allocated memory after every iteration
        free(raw_input);
        free(commands);

    }while(status != 0);
}

int main(int argc, char **argv){
    run_shell();
    return EXIT_SUCCESS;
}
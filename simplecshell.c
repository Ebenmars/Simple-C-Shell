#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

void parse_input(char *input, char **args) {
    char *token;
    int i = 0;
    token = strtok(input, " \t\n"); // Tokenize input
    
    while (token != NULL) { //storing each charcter in args
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL; // Null Terminate
}

void execute_command(char **args) {
    if (args[0] == NULL) return; // Empty command, do nothing


    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid = fork();
    if (pid < 0) { // process failed to be created 
        perror("Fork failed");
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1){ // if execvp fails 
            perror("Error executing command");
        }
        exit(EXIT_FAILURE);
    } else {
        wait(NULL); // Wait for the child process to finish
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARG_SIZE];

    while (1) { //infinite loop 
        printf("$ "); // start of command
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit (Ctrl+D)
        }
        parse_input(input, args);
        execute_command(args);
    }
    return 0;
}


/*
Author: Lane Ingles 
Description: Main file for Techshell that holds the main loop of the shell for
parsing user input.
*/
#include <stdio.h>      // Standard Input/Output (printf, fgets, perror)
#include <stdlib.h>     // Standard Library (malloc, free, exit)
#include <string.h>     // String manipulation (strtok, strcmp, strcspn, strlen)
#include <unistd.h>     // POSIX API (getcwd)
#include <errno.h>      // Error number definitions (errno)

#include "executor.c"   // importing the executor file, it hold struct definition and execute function

#define MAX_INPUT 1024

//////// Some function ideas: ////////////
// Note: Some code is reflected in main that represents these functions,
// but it is up to you to determine how you want to organize your code.


/* 
    A function that causes the prompt to 
    display in the terminal
*/
void displayPrompt() 
{
    char cwd[1024];      // Buffer (1024 is just large enough for most paths)
    if (getcwd(cwd, sizeof(cwd)) != NULL) // plug the path into the array cwd
    {
        printf("%s$ ", cwd);
    } 
    else 
    {
        perror("getcwd() error");   // Handles and prints error if getcwd fails
    }
}


/*
    A function that takes input from the user.
    It may return return the input to the calling statement or 
    store it at some memory location using a pointer.
*/ 
char* getInput() 
{
    char *input = malloc(MAX_INPUT * sizeof(char));  // allocates 1024 bytes for user input
    if (!input) 
    {
        perror("Malloc failed");  // Handle malloc failure
        exit(1);
    }

    if (fgets(input, MAX_INPUT, stdin) == NULL) // Reads user input, and checks for NULL 
                                                // which is returnedif EOF is encountered (Ctrl+D)
    {
        free(input);
        return NULL; // Handle EOF
    }

    // Removes the trailing newline character from fgets so that the command is readable
    input[strcspn(input, "\n")] = 0;
    return input;
}

/*
    A function that parses through the user input.
    Consider having this function return a struct that stores vital
    information about the parsed instruction such as:
    - The command itself
    - The arguments that come after the command 
        Hint: When formatting your data, 
        look into execvp and how it takes in args.
    - Information about if a redirect was detected such as >, <, or |
    - Information about whether or not a new file 
        needs to be created and what that filename may be.
    

    Some helpful functions when doing this come from string.h and stdlib.h, such as
    strtok, strcmp, strcpy, calloc, malloc, realloc, free, and more

    Be sure to consider/test for situations when a backslash is used to escape the space char
    and when quotes are used to group together various tokens.
*/

struct ShellCommand parseInput(char *input) 
{
    struct ShellCommand cmd;
    
    // Initialize struct to defaults
    cmd.command = NULL;
    cmd.input_file = NULL;
    cmd.output_file = NULL;
    int arg_idx = 0;

    char *token = strtok(input, " ");  // Tokenize input by spaces.  Replaces spaces with null terminators

    while (token != NULL) 
    {
        if (strcmp(token, "<") == 0)   // check for input redirection by comparing < to the token
        {
            token = strtok(NULL, " "); // Get the filename
            if (token) cmd.input_file = token;  // Store the input file in the struct
        }
        else if (strcmp(token, ">") == 0) 
        {
            token = strtok(NULL, " "); // Get the filename
            if (token) cmd.output_file = token;
        }
        // Regular Command/Argument
        else 
        {
            if (cmd.command == NULL) 
            {
                cmd.command = token; // First token is the command
            }
            cmd.args[arg_idx++] = token; // Add to args array
        }
        token = strtok(NULL, " ");
    }
    
    cmd.args[arg_idx] = NULL;     // The final index will be Null to indicate the termination for execvp

    return cmd;
}




int main() // MAIN
{
    char* input;
    struct ShellCommand command;
        
    for (;;)    // Infinite loop similar to while(1) 
    {
        // display the prompt
        displayPrompt();

        // get the user's input
        input = getInput();
        
        // If input is NULL we end the program
        if (input == NULL) 
        {
            printf("\n");
            break;
        }

        // If only enter is pressed, continue to next loop iteration
        if (strlen(input) == 0) 
        {
            free(input);
            continue;
        }
        
        // parse the input
        command = parseInput(input);
        
        // if the command is not NULL, execute the command using executor.c
        if (command.command != NULL) 
        {
            executeCommand(command); 
        }

        // Clean up input memory from malloc
        free(input);
    }

    exit(0);
}

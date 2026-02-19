/*
Jackson Clark
executor.c
CSC 222 Shell Project

Handles command execution for techshell.
- Implements built-in cd
- Handles I/O redirection (< and >)
- Uses fork() and execvp()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

struct ShellCommand {
    char *command; 
    char *args[65];      // Max args (arbitrary safe limit)
    char *input_file;    // For < redirection
    char *output_file;   // For > redirection
};

/*
Function: execute_command
Executes a parsed command with arguments.

args: NULL-terminated array of command arguments

returns: 0 on success, -1 on error
*/
int executeCommand(struct ShellCommand cmd)
{
    if (cmd.args == NULL || cmd.args[0] == NULL)
        return -1;

    
    //   BUILT-IN: exit
    
    if (strcmp(cmd.args[0], "exit") == 0)
    {
        exit(0);
    }

    //   BUILT-IN: cd

    if (strcmp(cmd.args[0], "cd") == 0)
    {
        char *path;

        if (cmd.args[1] == NULL)
        {
            path = getenv("HOME");
            if (path == NULL)
                path = "/";
        }
        else
        {
            path = cmd.args[1];
        }

        if (chdir(path) != 0)
        {
            printf("Error %d (%s)\n", errno, strerror(errno));
            return -1;
        }

        return 0;  // handled in parent
    }

    //   Handle Redirection

    int input_fd = -1;
    int output_fd = -1;

    if (cmd.input_file != NULL) 
    {
        input_fd = open(cmd.input_file, O_RDONLY);
        if (input_fd < 0) 
        {
            printf("Error %d (%s)\n", errno, strerror(errno));
            return -1;
        }
    }

    if (cmd.output_file != NULL) 
    {
        output_fd = open(cmd.output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) 
        {
            printf("Error %d (%s)\n", errno, strerror(errno));
            return -1;
        }
    }

    //   Fork and Execute

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Error %d (%s)\n", errno, strerror(errno));
        return -1;
    }

    if (pid == 0)
    {
        // CHILD PROCESS

        // Handle input redirection
        if (input_fd != -1)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        // Handle output redirection
        if (output_fd != -1)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        // Execute command
        if (execvp(cmd.args[0], cmd.args) == -1)
        {
            printf("Error %d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // PARENT PROCESS
        wait(NULL);

        if (input_fd != -1)
            close(input_fd);

        if (output_fd != -1)
            close(output_fd);
    }

    return 0;
}

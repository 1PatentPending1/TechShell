# TechShell
**coded by Lane Ingles and Jackson Clark**

TechShell is a C program that acts like a bash shell. It repeatedly allows
a user to enter input that gets interpreted as a shell command.


## This project contains two main files:

- `techshell.c`: This file is in charge of getting the current working directory with a $ and space following it, Handles the users command line input ensuring that it is valid, and then parses the input by tokenizing it and separating the command from its arguments.  It then passes the command and its arguments to the executor.c file.  **Coded by: Lane Ingles**

- `executor.c`: This file recieves the command and its arguments from techshell.c.  It implements the cd command, implements fork logic, I/O redirection using dup2(), checks for redirection symbols, and executes the command using execvp(). **Coded by: Jackson Clark**

## How to Clone, Compile, and Run:
1. Clone the repository using the command: `git clone https://github.com/1PatentPending1/TechShell.git`
2. Change into the TechShell directory: `cd TechShell`
3. Compile the program using the command: `gcc techshell.c executor.c -o techshell`
4. Run the program using the command: `./techshell`
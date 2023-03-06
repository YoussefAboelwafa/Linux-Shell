#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/resource.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64
char command[MAX_COMMAND_LENGTH];
char *args[MAX_ARGUMENTS];

#define RED "\x1b[31m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define YEL "\e[0;33m"
#define reset "\x1b[0m"

char *token;
char *token2;
char *var;
char *value;
int i;
int k;
FILE *file;
char *expr[2];
int flag=0;

// functions prototypes
void shell();
void execute_command();
void shell_builtin_commands(char[]);
void reap_child_zombie(pid_t);
void write_to_log_file(pid_t);
void on_child_exit(pid_t);

int main()
{   
    //signal (SIGCHLD, on_child_exit);
    file = fopen("logs.txt", "w");
    shell();
    return 0;
}

void shell()
{
    
    while (true)
    {   
        // Prompt the user for input
        char buf[1024];
        getcwd(buf, sizeof(buf));
        printf(RED "JOE-SHELL:" reset YEL "%s >>" reset, buf);

        // Read the user's input
        if (!fgets(command, MAX_COMMAND_LENGTH, stdin))
        {
            perror("Failed to read input");
            exit(EXIT_FAILURE);
        }
        // repeat function if ENTER button is pressed
        if (command[0] == '\n')
        {
            shell();
        }

        // Remove the newline character
        command[strcspn(command, "\n")] = '\0';

        // Parse the input into tokens

        token = strtok(command, " ");
        i = 0;

        while (token != NULL)
        {  
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        // Exit if the user enters "exit" as the command
        if (strcmp(args[0], "exit") == 0)
        {

            fclose(file);
            exit(EXIT_SUCCESS);
        }

        if (strcmp(args[0], "echo") == 0 || strcmp(args[0], "cd") == 0 || strcmp(args[0], "export") == 0)
        {
            char *c = args[0];
            shell_builtin_commands(c);
        }
        else
        {
            execute_command();
        }
    }
}

void shell_builtin_commands(char string[])
{
    if (strcmp(string, "cd") == 0)
    {
        // (~) to go /home
        if (strcmp(args[1], "~") == 0)
        {
            args[1] = "/home";
        }
        // if invalid path name
        if (chdir(args[1]) == -1)
        {
            perror("Failed to execute command");
            exit(EXIT_FAILURE);
        }
    }

    else if (strcmp(string, "echo") == 0)
    {
        // Loop through the arguments and print them to the console
        for (int i = 1; args[i] != NULL; i++)
        {
            if (args[i][0] == '"' && args[i][strlen(args[i]) - 1] == '"')
            {
                // If yes, print the argument without the quotes
                printf("%.*s ", (int)(strlen(args[i]) - 2), args[i] + 1);
            }
            else if (args[i][0] == '$')
            {
                printf("%s", value);
            }
            else
            {
                // Otherwise, print the argument as is
                printf("%s ", args[i]);
            }
        }
        printf("\n");
    }
    else if (strcmp(string, "export") == 0)
    {
        token2 = strtok(args[1], "=");
        k = 0;

        while (token2 != NULL)
        {
            expr[k++] = token2;
            token2 = strtok(NULL, " ");
        }

        expr[k] = NULL;
        var = expr[0];
        value = expr[1];
    }
}

void execute_command()
{   
    pid_t pid = fork();
    // Error
    if (pid < 0)
    {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }
    // In the child
    else if (pid == 0)
     {
        //  if(strcmp(args[1],"&")==0){
        //     flag=1;
        //  }
        if (execvp(args[0], args) == -1)
        {
            perror("Failed to execute command");
            exit(EXIT_FAILURE);
        }
    }
    // In the parent
    else
    {
        on_child_exit(pid);
        
    }
}

void reap_child_zombie(pid_t pid){
    int status;
    
    waitpid(pid, &status, 0);
    
    
}
void write_to_log_file(pid_t pid){
    fprintf(file, "Child process (%d) terminated\n", pid);
}
void on_child_exit(pid_t pid){
    reap_child_zombie(pid);
    write_to_log_file(pid);
}
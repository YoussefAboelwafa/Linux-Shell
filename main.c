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
char *var;
char *value;
int i;
int k;
FILE *file;
int flag = 0;
pid_t id;

// functions prototypes
void shell();
void execute_command();
void shell_builtin_commands(char[]);
void write_to_log_file(pid_t);
void on_child_exit(pid_t);
void on_signal_exit();
void remove_quotes(char *);
void replace_string(char *, char *, char *);
char *extract_after_equal_sign(char *);

int main()
{
    signal(SIGCHLD, on_signal_exit);
    shell();

    return 0;
}

void shell()
{

    while (true)
    {
        flag = 0;
        // Prompt the user for input
        char buf[1024];
        getcwd(buf, sizeof(buf));
        printf(RED "JOE-SHELL:" reset YEL "%s >>" reset, buf);

        // Read the user's input
        if (!fgets(command, MAX_COMMAND_LENGTH, stdin))
        {
            perror("Failed to read input");
            shell();
        }
        // repeat function if ENTER button is pressed
        if (command[0] == '\n')
        {
            shell();
        }

        // Remove the newline character
        command[strcspn(command, "\n")] = '\0';

        // Parse the input into tokens
        char temp[1000];
        strcpy(temp, command);
        char *hamo = strtok(temp, " ");
        if (strcmp(hamo, "export") == 0)
        {

            token = command;
            args[0] = strsep(&token, " ");
            args[1] = token;
            remove_quotes(args[1]);
        }
        else
        {
            token = strtok(command, " ");
            i = 0;
            while (token != NULL)
            {
                args[i++] = token;

                token = strtok(NULL, " ");
            }

            args[i] = NULL;

            if (args[1] != NULL)
            {
                if (strcmp(args[1], "&") == 0)
                {
                    flag = 1;
                }
            }
            for (int j = 0; args[j] != NULL; j++)
            {
                if (args[j][0] == '$')
                {
                    char temp[50];
                    int l = 0;
                    for (k = 1; args[1][k] != '\0'; k++)
                    {
                        temp[l++] = args[1][k];
                    }
                    temp[l] = '\0';
                    char *g = getenv(temp);
                    strcpy(temp, g);
                    l = 1;
                    token = strtok(temp, " ");
                    while (token != NULL)
                    {
                        args[l++] = token;
                        token = strtok(NULL, " ");
                    }
                    args[l] = NULL;
                }
            }

            // Exit if the user enters "exit" as the command
            if (strcmp(args[0], "exit") == 0)
            {
                // kill zombie before exit
                kill(id, SIGKILL);
                on_signal_exit();

                exit(EXIT_SUCCESS);
            }
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
        int e;
        // (~) to go Home
        if (strcmp(args[1], "~") == 0)
        {
            chdir(getenv("HOME"));
        }
        else
        {
            e = chdir(args[1]);
        }
        // if invalid path name
        if (e == -1)
        {
            perror("Failed to execute command");
            shell();
        }
    }

    else if (strcmp(string, "echo") == 0)
    {

        if (args[1][0] == '$')
        {
            char temp[100];
            int c = 0;
            for (int j = 1; args[1][j] != '\0'; j++)
            {
                temp[c++] = args[1][j];
            }
            temp[c] = '\0';

            args[1] = getenv(temp);
        }
        for (int j = 1; args[j] != NULL; j++)
            printf("%s\n", args[j]);
    }
    else if (strcmp(string, "export") == 0)
    {
        token = strtok(args[1], "=");
        char *name = token;
        token = strtok(NULL, "=");
        value = token;
        setenv(name, value, 1);
    }
}

void execute_command()
{
    pid_t pid = fork();
    id = pid;
    // Error
    if (pid < 0)
    {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }
    // In the child
    else if (pid == 0)
    {

        if (execvp(args[0], args) == -1)
        {
            perror("Failed to execute command");
            exit(EXIT_FAILURE);
        }
    }
    // In the parent
    else
    {
        if (flag == 0)
        {
            on_child_exit(pid);
        }
    }
}

void on_child_exit(pid_t pid)
{
    int status;
    waitpid(pid, &status, 0);
    write_to_log_file(pid);
}

void on_signal_exit()
{
    int var;
    int status;
    var = waitpid(id, &status, 0);
    if (var >= 0)
    {
        write_to_log_file(id);
    }
}

void write_to_log_file(pid_t pid)
{
    file = fopen("/media/youssef/Others/2nd term/OS/labs/simple-shell/logs.txt", "a");
    fprintf(file, "Child process (%d) terminated\n", pid);
    fclose(file);
}

void remove_quotes(char *str)
{
    char *dest = str;
    while (*str)
    {
        if (*str != '"')
        {
            *dest++ = *str;
        }
        str++;
    }
    *dest = '\0';
}

void replace_string(char *original_string, char *find_string, char *replace_string)
{
    char *occurrence = strstr(original_string, find_string);
    size_t find_len = strlen(find_string);

    while (occurrence != NULL)
    {
        size_t occurrence_index = occurrence - original_string;
        size_t replace_len = strlen(replace_string);

        memmove(original_string + occurrence_index + replace_len,
                original_string + occurrence_index + find_len,
                strlen(original_string + occurrence_index + find_len) + 1);

        memcpy(original_string + occurrence_index, replace_string, replace_len);

        occurrence = strstr(original_string + occurrence_index + replace_len, find_string);
    }
}

char *extract_after_equal_sign(char *input_string)
{

    char *occurrence = strchr(input_string, '='); // find the first occurrence of '='

    if (occurrence != NULL)
    {                          // check if '=' was found
        return occurrence + 1; // return the substring that follows the '='
    }
    else
    {
        return input_string; // return the whole input string if '=' was not found
    }
}
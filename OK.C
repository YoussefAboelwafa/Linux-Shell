 

        // Exit if the user enters "exit" as the command
        if (strcmp(args[0], "exit") == 0)
        {

            fclose(file);
            exit(EXIT_SUCCESS);
        }




        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Failed to fork");
            exit(EXIT_FAILURE);
        }
        // In the child
        else if (pid == 0)
        {   
            fprintf(file, "created : child_id=%d, parent_id=%d\n", getpid(), getppid());
            if (strcmp(args[0], "echo") == 0 || strcmp(args[0], "cd") == 0 || strcmp(args[0], "export") == 0)
            {
                char *c = args[0];
                shell_builtin_commands(c);
            }
            else
            {   
                if (execvp(args[0], args) == -1)
                {
                    perror("Failed to execute command");
                    exit(EXIT_FAILURE);
                }
            }
        }
        // In the parent
        else
        {
            int status;
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("Failed to wait for child process");
                exit(EXIT_FAILURE);
            }
        }
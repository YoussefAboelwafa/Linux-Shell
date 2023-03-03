#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    pid = fork();

    //failure
    if (pid == -1)
    {
        perror("ERROR!\n");
        return 1;
    }
    //child
    else if (pid==0){
        sleep(3);
        printf("Child process\n");
    }
    else {
        wait(NULL);
        printf("Parent process\n");
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    char *argv[] = {"/bin/echo","hello", NULL};
    printf("\n");
    execve(argv[0], argv, NULL);
    printf("done with execute");

    
}

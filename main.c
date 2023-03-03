#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t parent_id = getpid();

    printf("parent, PID=%d  shell process id =%d \n", parent_id, getppid());
    fork();
    fork();
    pid_t child_id = getpid();
    pid_t current_parent = getppid();
    if (current_parent == parent_id)
    {
        printf("\nchild, PID=%d  parent, PID=%d\n", child_id, parent_id);
    }
    fork();
    return 0;
}

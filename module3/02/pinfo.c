#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    printf("PID  = %d\n", getpid());
    printf("PPID = %d\n", getppid());
    printf("UID  = %d\n", getuid());
    printf("EUID = %d\n", geteuid());

    return 0;
}

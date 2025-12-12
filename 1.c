#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

void print_info() {
    struct timeval tv;
    struct tm *tm_info;

    gettimeofday(&tv, NULL);
    tm_info = localtime(&tv.tv_sec);

    printf("pid=%d ppid=%d time=%02d:%02d:%02d:%03ld\n",
           getpid(),
           getppid(),
           tm_info->tm_hour,
           tm_info->tm_min,
           tm_info->tm_sec,
           tv.tv_usec / 1000);
}

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        print_info();
        return 0;
    }

    pid2 = fork();
    if (pid2 == 0) {
        print_info();
        return 0;
    }

    print_info();
    system("ps -x");

    wait(NULL);
    wait(NULL);

    return 0;
}

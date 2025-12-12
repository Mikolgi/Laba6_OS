#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

static void print_info(int id) {
    struct timeval tv;
    struct tm *tm_info;

    gettimeofday(&tv, NULL);
    tm_info = localtime(&tv.tv_sec);

    printf("proc=%d pid=%d ppid=%d time=%02d:%02d:%02d:%03ld\n",
           id, getpid(), getppid(),
           tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
           tv.tv_usec / 1000);
    fflush(stdout);
}

static void run_loop(int id) {
    int t_ms = id * 200;
    while (1) {
        print_info(id);
        usleep((useconds_t)t_ms * 1000);
    }
}

static void create_tree(int level, int max_level, int children, int id) {
    if (level >= max_level) return;

    for (int i = 1; i <= children; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            int child_id = (id - 1) * children + i + 1;
            create_tree(level + 1, max_level, children, child_id);
            run_loop(child_id);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <generations> <children>\n", argv[0]);
        return 1;
    }

    int generations = atoi(argv[1]);
    int children = atoi(argv[2]);

    if (generations < 0 || children < 0) {
        fprintf(stderr, "generations and children must be >= 0\n");
        return 1;
    }

    create_tree(0, generations, children, 1);
    run_loop(1);
    return 0;
}

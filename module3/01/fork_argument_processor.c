#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static int is_number(const char *s, double *value) {
    char *endptr;
    if (s == NULL || *s == '\0') {
        return 0;
    }
    *value = strtod(s, &endptr);
    return (*endptr == '\0');
}

static void process_range(char *argv[], int start, int end, const char *who) {
    for (int i = start; i < end; i++) {
        double value;
        if (is_number(argv[i], &value)) {
            printf("%s [PID=%d]: \"%s\" -> %.10g\n", who, getpid(), argv[i], value * 2.0);
        } else {
             printf("%s [PID=%d]: \"%s\"\n", who, getpid(), argv[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int status;
    int arg_count;
    int mid;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s arg1 arg2 ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    arg_count = argc - 1;
    mid = 1 + arg_count / 2;

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        process_range(argv, 1, mid, "CHILD");
        _exit(EXIT_SUCCESS);
    } else {
        process_range(argv, mid, argc, "PARENT");

        if (wait(&status) == -1) {
            perror("wait");
            return EXIT_FAILURE;
        }

        if (WIFEXITED(status)) {
            printf("PARENT: child exit status = %d\n", WEXITSTATUS(status));
        } else {
            printf("PARENT: child finished abnormally\n");
        }

        return EXIT_SUCCESS;
    }
}

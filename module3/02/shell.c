#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

#define MAX_BUF 1024
#define MAX_ARGS 64

static int parse_line(char *line, char *args[])
{
    int count = 0;
    char *token = strtok(line, " \t");

    while (token != NULL && count < MAX_ARGS - 1) {
        args[count++] = token;
        token = strtok(NULL, " \t");
    }

    args[count] = NULL;
    return count;
}

static void run_command(char *args[])
{
    char local_path[MAX_BUF];

    if (strchr(args[0], '/') != NULL) {
        execv(args[0], args);
        fprintf(stderr, "Не удалось запустить %s: %s\n", args[0], strerror(errno));
        _exit(127);
    }

    snprintf(local_path, sizeof(local_path), "./%s", args[0]);
    execv(local_path, args);

    if (errno != ENOENT) {
        fprintf(stderr, "Не удалось запустить %s: %s\n", args[0], strerror(errno));
        _exit(127);
    }

    execvp(args[0], args);

    fprintf(stderr, "Команда не найдена: %s\n", args[0]);
    _exit(127);
}

int main(void)
{
    char buf[MAX_BUF];
    char *args[MAX_ARGS];

    while (1) {
        printf("shell> ");
        fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            printf("\n");
            break;
        }

        buf[strcspn(buf, "\n")] = '\0';

        if (buf[0] == '\0') {
            continue;
        }

        int argc = parse_line(buf, args);

        if (argc == 0) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            run_command(args);
        } else {
            int status;

            if (waitpid(pid, &status, 0) < 0) {
                perror("waitpid");
                continue;
            }
        }
    }

    return 0;
}

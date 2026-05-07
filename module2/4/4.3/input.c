#include "input.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trim_newline(char *str)
{
    size_t len = strlen(str);

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

static void flush_stdin_tail(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void read_line(const char *prompt, char *buffer, size_t size)
{
    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    if (strchr(buffer, '\n') == NULL) {
        flush_stdin_tail();
    }

    trim_newline(buffer);
}

int read_int(const char *prompt)
{
    char buffer[64];
    char *endptr;
    long value;

    while (1) {
        read_line(prompt, buffer, sizeof(buffer));

        if (buffer[0] == '\0') {
            printf("Ошибка: введите число.\n");
            continue;
        }

        errno = 0;
        value = strtol(buffer, &endptr, 10);

        if (*endptr == '\0' && errno != ERANGE && value >= INT_MIN && value <= INT_MAX) {
            return (int)value;
        }

        printf("Ошибка: введите корректное целое число.\n");
    }
}

void pause_console(void)
{
    char buffer[8];

    read_line("\nНажмите Enter, чтобы продолжить...", buffer, sizeof(buffer));
}

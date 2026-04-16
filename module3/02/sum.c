#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static int parse_int(const char *s, int *value)
{
    char *end = NULL;
    long val;

    errno = 0;
    val = strtol(s, &end, 10);

    if (errno != 0 || *end != '\0' || val < INT_MIN || val > INT_MAX) {
        return 0;
    }

    *value = (int)val;
    return 1;
}

int main(int argc, char *argv[])
{
    int sum = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s num1 num2 ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        int value;

        if (!parse_int(argv[i], &value)) {
            fprintf(stderr, "Некорректное число: %s\n", argv[i]);
            return EXIT_FAILURE;
        }

        sum += value;
    }

    printf("Sum: %d\n", sum);
    return EXIT_SUCCESS;
}

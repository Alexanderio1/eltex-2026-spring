#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int max_index = 1;
    size_t max_len;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s str1 str2 ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    max_len = strlen(argv[1]);

    for (int i = 2; i < argc; i++) {
        size_t current_len = strlen(argv[i]);

        if (current_len > max_len) {
            max_len = current_len;
            max_index = i;
        }
    }

    printf("Longest string: %s\n", argv[max_index]);
    printf("Length: %zu\n", max_len);

    return EXIT_SUCCESS;
}

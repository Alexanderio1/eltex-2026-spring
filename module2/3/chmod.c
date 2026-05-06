#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#define ADD         0x001
#define SET         0x002
#define DELETE      0x003

#define USER        0x1C0
#define GROUP       0x038
#define OTHER       0x007
#define ALL         0x1FF

#define READ        0x124
#define WRITE       0x092
#define EXEC        0x049
#define NO          0x000

#define ERROR       0xFFF

void letterToBit(char* c, unsigned int* permission);
void numericToBit(char* c, unsigned int* permission);
void letterPermissions(unsigned int mode, char *permission);
void bitPermissions(unsigned int mode, unsigned int *permissions);
void numericPermissions(unsigned int mode, unsigned int *permission);

unsigned int letterToMode(char *c);
unsigned int numericToMode(char *c);
void printPermissions(unsigned int mode);

unsigned int badchmod(char *c, const char *filename);
unsigned int changeMode(char *c, unsigned int mode);
unsigned int letterchmod(char *c, unsigned int mode);
unsigned int numericchmod(char* c, unsigned int mode);
unsigned int matchPattern(char *input, const char *pattern);

#pragma region /* Пункт 1 */

unsigned int letterToMode(char *c) {
    unsigned int mode = 0x000;
    int start = 0;
    int len = strlen(c);

    if (len == 10) {
        start = 1;
        len = 9;
    }

    if (len != 9)
        return (unsigned int)ERROR;

    char rights[9] = {'r','w','x','r','w','x','r','w','x'};

    unsigned int bits[9] = {
        S_IRUSR, S_IWUSR, S_IXUSR,
        S_IRGRP, S_IWGRP, S_IXGRP,
        S_IROTH, S_IWOTH, S_IXOTH
    };

    for (int i = 0; i < 9; i++) {
        if (c[start + i] == '-') {
            continue;
        }
        else if (c[start + i] == rights[i]) {
            mode |= bits[i];
        }
        else {
            return (unsigned int)ERROR;
        }
    }

    return mode & ALL;
}

unsigned int numericToMode(char *c) {
    unsigned int mode = 0x000;

    if (strlen(c) != 3)
        return (unsigned int)ERROR;

    for (int i = 0; i < 3; i++) {
        if (c[i] < '0' || c[i] > '7')
            return (unsigned int)ERROR;

        mode = (mode << 3) | (c[i] - '0');
    }

    return mode & ALL;
}

void letterToBit(char* c, unsigned int* permission) {
    unsigned int mode = letterToMode(c);

    if (mode == ERROR) {
        for (int i = 0; i < 9; i++)
            permission[i] = 0;
        return;
    }

    bitPermissions(mode, permission);
}

void numericToBit(char* c, unsigned int* permission) {
    unsigned int mode = numericToMode(c);

    if (mode == ERROR) {
        for (int i = 0; i < 9; i++)
            permission[i] = 0;
        return;
    }

    bitPermissions(mode, permission);
}

#pragma endregion

#pragma region /* Пункт 2 */

void letterPermissions(unsigned int mode, char *permission) {
    for (int i = 0; i < 9; i++)
        permission[i] = '-';

    permission[9] = '\0';

    if (mode & S_IRUSR) permission[0] = 'r';
    if (mode & S_IWUSR) permission[1] = 'w';
    if (mode & S_IXUSR) permission[2] = 'x';

    if (mode & S_IRGRP) permission[3] = 'r';
    if (mode & S_IWGRP) permission[4] = 'w';
    if (mode & S_IXGRP) permission[5] = 'x';

    if (mode & S_IROTH) permission[6] = 'r';
    if (mode & S_IWOTH) permission[7] = 'w';
    if (mode & S_IXOTH) permission[8] = 'x';
}

void numericPermissions(unsigned int mode, unsigned int *permission) {
    unsigned int mask = 0x007;

    for (int i = 2; i >= 0; i--) {
        permission[i] = mode & mask;
        mode = mode >> 3;
    }
}

void bitPermissions(unsigned int mode, unsigned int *permission) {
    unsigned int mask = 0x100;

    for (int i = 0; i < 9; i++) {
        if (mode & mask) permission[i] = 1;
        else permission[i] = 0;

        mask = mask >> 1;
    }
}

void printPermissions(unsigned int mode) {
    char letters[10];
    unsigned int numeric[3];
    unsigned int bits[9];

    mode &= ALL;

    letterPermissions(mode, letters);
    numericPermissions(mode, numeric);
    bitPermissions(mode, bits);

    printf("Letter permission:  %s\n", letters);
    printf("Numeric permission: %u%u%u\n", numeric[0], numeric[1], numeric[2]);

    printf("Bit permission:     ");
    for (int i = 0; i < 9; i++) {
        printf("%u", bits[i]);

        if (i == 2 || i == 5)
            printf(" ");
    }

    printf("\n");
}

#pragma endregion

#pragma region /* Пункт 3 */

unsigned int badchmod(char *c, const char *filename) {
    struct stat file_stat;
    unsigned int mode = 0x000;

    if (stat(filename, &file_stat) == 0)
        mode = file_stat.st_mode & ALL;
    else
        return (unsigned int)ERROR;

    return changeMode(c, mode);
}

unsigned int changeMode(char *c, unsigned int mode) {
    if (matchPattern(c, "^[ugoa]+[+=-][rwx]{1,3}$"))
        return (unsigned int)letterchmod(c, mode);
    else if (matchPattern(c, "^[0-7]{3}$"))
        return (unsigned int)numericchmod(c, mode);
    else
        return (unsigned int)ERROR;
}

unsigned int letterchmod(char *c, unsigned int mode) {
    unsigned int access_bit = 0x000;
    unsigned int action_bit = 0x000;
    unsigned int permission = 0;

    int len = strlen(c);

    for (int i = 0; i < len; i++)
    {
        switch (c[i])
        {
            case '+':
                permission = ADD;
                break;

            case '-':
                permission = DELETE;
                break;

            case '=':
                permission = SET;
                break;

            case 'u':
                access_bit |= USER;
                break;

            case 'g':
                access_bit |= GROUP;
                break;

            case 'o':
                access_bit |= OTHER;
                break;

            case 'a':
                access_bit |= ALL;
                break;

            case 'r':
                action_bit |= READ;
                break;

            case 'w':
                action_bit |= WRITE;
                break;

            case 'x':
                action_bit |= EXEC;
                break;

            default:
                return (unsigned int)ERROR;
        }
    }

    action_bit = access_bit & action_bit;

    switch (permission)
    {
        case ADD:
            mode |= action_bit;
            break;

        case SET:
            mode = (mode & (~access_bit)) | action_bit;
            break;

        case DELETE:
            mode &= (~action_bit);
            break;

        default:
            return (unsigned int)ERROR;
    }

    return mode & ALL;
}

unsigned int numericchmod(char* c, unsigned int mode) {
    unsigned int new_mode = numericToMode(c);

    if (new_mode == ERROR)
        return (unsigned int)ERROR;

    mode = new_mode;

    return mode & ALL;
}

#pragma endregion

unsigned int matchPattern(char *input, const char *pattern) {
    regex_t regex;
    unsigned int reti;

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) return 0;

    reti = regexec(&regex, input, 0, NULL, 0);
    regfree(&regex);

    if (!reti) return 1;
    else if (reti == REG_NOMATCH) return 0;
    else return 0;
}

int main()
{
    int choice;
    char input[256];
    char filename[256];
    struct stat file_stat;

    unsigned int mode = 0x000;
    unsigned int new_mode = 0x000;
    int mode_defined = 0;

    while (1) {
        printf("\n----------------\n");
        printf("1. Enter permissions in letter or numeric form\n");
        printf("2. Enter file name and get permissions using stat\n");
        printf("3. Change current permissions like chmod\n");
        printf("0. Exit\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1)
            break;

        if (choice == 0)
            break;

        if (choice == 1) {
            printf("Enter permissions, for example rwxr-xr-x or 755: ");
            scanf("%255s", input);

            if (matchPattern(input, "^[0-7]{3}$")) {
                mode = numericToMode(input);
            }
            else {
                mode = letterToMode(input);
            }

            if (mode == ERROR) {
                printf("Incorrect permissions format.\n");
            }
            else {
                mode_defined = 1;

                printf("\nResult:\n");
                printPermissions(mode);
            }
        }
        else if (choice == 2) {
            printf("Enter file name: ");
            scanf("%255s", filename);

            if (stat(filename, &file_stat) != 0) {
                perror("stat");
            }
            else {
                mode = file_stat.st_mode & ALL;
                mode_defined = 1;

                printf("\nFile %s permissions:\n", filename);
                printPermissions(mode);

                printf("\nCompare with command:\n");
                printf("ls -l %s\n", filename);
            }
        }
        else if (choice == 3) {
            if (!mode_defined) {
                printf("First enter permissions manually or get them from a file.\n");
                continue;
            }

            printf("\nCurrent permissions:\n");
            printPermissions(mode);

            printf("\nEnter chmod-like command, for example u+x, g-w, o=rx, a+r or 764: ");
            scanf("%255s", input);

            new_mode = changeMode(input, mode);

            if (new_mode == ERROR) {
                printf("Incorrect chmod-like command.\n");
            }
            else {
                mode = new_mode;

                printf("\nNew permissions:\n");
                printPermissions(mode);

                printf("\nPermissions are changed only inside the program. Real file is not modified.\n");
            }
        }
        else {
            printf("Incorrect menu item.\n");
        }
    }

    return 0;
}

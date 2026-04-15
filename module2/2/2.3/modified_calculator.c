#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef int (*operation_func)(double, double, double *);

typedef struct {
    char name[32];
    char symbol;
    operation_func func;
} Command;

typedef struct {
    Command *items;
    size_t count;
    size_t capacity;
} CommandList;

int add(double a, double b, double *result) {
    *result = a + b;
    return 0;
}

int subtract(double a, double b, double *result) {
    *result = a - b;
    return 0;
}

int multiply(double a, double b, double *result) {
    *result = a * b;
    return 0;
}

int divide(double a, double b, double *result) {
    if (b == 0.0) {
        return -1;
    }

    *result = a / b;
    return 0;
}

void init_command_list(CommandList *list) {
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void free_command_list(CommandList *list) {
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int register_command(CommandList *list, const char *name, char symbol, operation_func func) {
    Command *new_items;

    if (list->count == list->capacity) {
        size_t new_capacity = (list->capacity == 0) ? 4 : list->capacity * 2;
        new_items = realloc(list->items, new_capacity * sizeof(Command));
        if (new_items == NULL) {
            return -1;
        }

        list->items = new_items;
        list->capacity = new_capacity;
    }

    strncpy(list->items[list->count].name, name, sizeof(list->items[list->count].name) - 1);
    list->items[list->count].name[sizeof(list->items[list->count].name) - 1] = '\0';
    list->items[list->count].symbol = symbol;
    list->items[list->count].func = func;
    list->count++;

    return 0;
}

void read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

int read_int(const char *prompt) {
    char buffer[64];
    char *endptr;
    long value;

    while (1) {
        printf("%s", prompt);
        read_line(buffer, sizeof(buffer));

        if (buffer[0] == '\0') {
            printf("Ошибка: введите число.\n");
            continue;
        }

        value = strtol(buffer, &endptr, 10);

        while (isspace((unsigned char)*endptr)) {
            endptr++;
        }

        if (*endptr == '\0') {
            return (int)value;
        }

        printf("Ошибка: введите корректное целое число.\n");
    }
}

double read_double(const char *prompt) {
    char buffer[64];
    char *endptr;
    double value;

    while (1) {
        printf("%s", prompt);
        read_line(buffer, sizeof(buffer));

        if (buffer[0] == '\0') {
            printf("Ошибка: введите число.\n");
            continue;
        }

        value = strtod(buffer, &endptr);

        while (isspace((unsigned char)*endptr)) {
            endptr++;
        }

        if (*endptr == '\0') {
            return value;
        }

        printf("Ошибка: введите корректное число.\n");
    }
}

void print_menu(const CommandList *list) {
    size_t i;

    printf("\n УСОВЕРШЕНСТВОВАННЫЙ КАЛЬКУЛЯТОР \n");
    for (i = 0; i < list->count; i++) {
        printf("%zu. %s (%c)\n", i + 1, list->items[i].name, list->items[i].symbol);
    }
    printf("0. Выход\n");
}

int main(void) {
    CommandList commands;
    int choice;
    double a;
    double b;
    double result;
    int status;
    Command *cmd;

    init_command_list(&commands);

    if (register_command(&commands, "Сложение", '+', add) != 0 ||
        register_command(&commands, "Вычитание", '-', subtract) != 0 ||
        register_command(&commands, "Умножение", '*', multiply) != 0 ||
        register_command(&commands, "Деление", '/', divide) != 0) {
        printf("Ошибка: не удалось создать список команд.\n");
        free_command_list(&commands);
        return 1;
    }

    while (1) {
        print_menu(&commands);
        choice = read_int("\nВыберите команду: ");

        if (choice == 0) {
            printf("Выход из программы.\n");
            break;
        }

        if (choice < 1 || (size_t)choice > commands.count) {
            printf("Ошибка: такой команды нет.\n");
            continue;
        }

        cmd = &commands.items[choice - 1];

        a = read_double("Введите первый аргумент: ");
        b = read_double("Введите второй аргумент: ");

        status = cmd->func(a, b, &result);

        if (status != 0) {
            printf("Ошибка: деление на ноль невозможно.\n");
        } else {
            printf("Результат: %.2f\n", result);
        }
    }

    free_command_list(&commands);
    return 0;
}

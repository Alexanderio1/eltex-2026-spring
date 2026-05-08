#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>

#define LIB_DIR "./libs"

typedef int (*operation_func)(double, double, double *);

typedef struct {
    char name[32];
    char symbol;
    operation_func func;
    void *handle;
} Command;

typedef struct {
    Command *items;
    size_t count;
    size_t capacity;
} CommandList;

void init_command_list(CommandList *list)
{
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void free_command_list(CommandList *list)
{
    size_t i;

    for (i = 0; i < list->count; i++) {
        if (list->items[i].handle != NULL) {
            dlclose(list->items[i].handle);
        }
    }

    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int register_command(CommandList *list, const char *name, char symbol,
                     operation_func func, void *handle)
{
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

    strncpy(list->items[list->count].name, name,
            sizeof(list->items[list->count].name) - 1);
    list->items[list->count].name[sizeof(list->items[list->count].name) - 1] = '\0';

    list->items[list->count].symbol = symbol;
    list->items[list->count].func = func;
    list->items[list->count].handle = handle;
    list->count++;

    return 0;
}

void read_line(char *buffer, size_t size)
{
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

int read_int(const char *prompt)
{
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

double read_double(const char *prompt)
{
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

void print_menu(const CommandList *list)
{
    size_t i;

    printf("\n УСОВЕРШЕНСТВОВАННЫЙ КАЛЬКУЛЯТОР \n");

    for (i = 0; i < list->count; i++) {
        printf("%zu. %s (%c)\n", i + 1, list->items[i].name, list->items[i].symbol);
    }

    printf("0. Выход\n");
}

void get_operation_info(const char *func_name, const char **name, char *symbol)
{
    if (strcmp(func_name, "add") == 0) {
        *name = "Сложение";
        *symbol = '+';
    } else if (strcmp(func_name, "subtract") == 0) {
        *name = "Вычитание";
        *symbol = '-';
    } else if (strcmp(func_name, "multiply") == 0) {
        *name = "Умножение";
        *symbol = '*';
    } else if (strcmp(func_name, "divide") == 0) {
        *name = "Деление";
        *symbol = '/';
    } else {
        *name = func_name;
        *symbol = '?';
    }
}

int load_libraries(CommandList *commands)
{
    DIR *dir;
    struct dirent *entry;
    int loaded = 0;

    dir = opendir(LIB_DIR);
    if (dir == NULL) {
        printf("Ошибка: не удалось открыть каталог %s.\n", LIB_DIR);
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        char func_name[32];
        char path[512];
        const char *name;
        char symbol;
        void *handle;
        operation_func func;

        if (sscanf(entry->d_name, "lib%31[^.].so", func_name) != 1) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", LIB_DIR, entry->d_name);

        handle = dlopen(path, RTLD_LAZY);
        if (handle == NULL) {
            printf("Ошибка загрузки %s: %s\n", path, dlerror());
            continue;
        }

        dlerror();
        *(void **)(&func) = dlsym(handle, func_name);

        if (dlerror() != NULL) {
            printf("В %s не найдена функция %s.\n", entry->d_name, func_name);
            dlclose(handle);
            continue;
        }

        get_operation_info(func_name, &name, &symbol);

        if (register_command(commands, name, symbol, func, handle) != 0) {
            dlclose(handle);
            closedir(dir);
            return 0;
        }

        loaded++;
    }

    closedir(dir);
    return loaded;
}

int main(void)
{
    CommandList commands;
    int choice;
    double a;
    double b;
    double result;
    int status;
    Command *cmd;

    init_command_list(&commands);

    if (load_libraries(&commands) == 0) {
        printf("Ошибка: не найдено ни одной функции в каталоге %s.\n", LIB_DIR);
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
            printf("Ошибка: операция не может быть выполнена.\n");
        } else {
            printf("Результат: %.2f\n", result);
        }
    }

    free_command_list(&commands);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

int divide(double a, double b, double *result) {
    if (b == 0.0) {
        return -1;
    }

    *result = a / b;
    return 0;
}

void print_menu(void) {
    printf("\n КАЛЬКУЛЯТОР \n");
    printf("1. Сложение\n");
    printf("2. Вычитание\n");
    printf("3. Умножение\n");
    printf("4. Деление\n");
    printf("0. Выход\n");
}

void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
    }
}

int read_int(const char *prompt) {
    char buffer[64];
    char *endptr;
    long value;

    while (1) {
        printf("%s", prompt);
        read_line(buffer, sizeof(buffer));

        value = strtol(buffer, &endptr, 10);

        if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
            return (int)value;
        }

        printf("Ошибка: введите целое число.\n");
    }
}

double read_double(const char *prompt) {
    char buffer[64];
    char *endptr;
    double value;

    while (1) {
        printf("%s", prompt);
        read_line(buffer, sizeof(buffer));

        value = strtod(buffer, &endptr);

        if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
            return value;
        }

        printf("Ошибка: введите число.\n");
    }
}

int main(void) {
    int choice;
    double a;
    double b;
    double result;

    while (1) {
        print_menu();
        choice = read_int("\nВыберите действие: ");

        if (choice == 0) {
            printf("Выход из программы.\n");
            break;
        }

        if (choice < 0 || choice > 4) {
            printf("Ошибка: такого пункта меню нет.\n");
            continue;
        }

        a = read_double("Введите первый аргумент: ");
        b = read_double("Введите второй аргумент: ");

        switch (choice) {
            case 1:
                result = add(a, b);
                printf("Результат: %.2f\n", result);
                break;

            case 2:
                result = subtract(a, b);
                printf("Результат: %.2f\n", result);
                break;

            case 3:
                result = multiply(a, b);
                printf("Результат: %.2f\n", result);
                break;

            case 4:
                if (divide(a, b, &result) != 0) {
                    printf("Ошибка: деление на ноль невозможно.\n");
                } else {
                    printf("Результат: %.2f\n", result);
                }
                break;
        }
    }

    return 0;
}

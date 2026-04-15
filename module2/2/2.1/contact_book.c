#include "contact_book.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

static void flush_stdin_tail(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void copy_field(char *dest, size_t dest_size, const char *src) {
    if (dest_size == 0) {
        return;
    }

    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

static void update_optional_field(const char *prompt, char *field, size_t field_size) {
    char buffer[256];

    read_line(prompt, buffer, sizeof(buffer));

    if (strcmp(buffer, "-") == 0) {
        field[0] = '\0';
        return;
    }

    if (buffer[0] != '\0') {
        copy_field(field, field_size, buffer);
    }
}

static const char *show_or_dash(const char *value) {
    return value[0] == '\0' ? "-" : value;
}

void init_contact_book(ContactBook *book) {
    book->count = 0;
}

void read_line(const char *prompt, char *buffer, size_t size) {
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

int read_int(const char *prompt) {
    char buffer[64];
    char *endptr;
    long value;

    while (1) {
        read_line(prompt, buffer, sizeof(buffer));

        if (buffer[0] == '\0') {
            printf("Ошибка: введите число.\n");
            continue;
        }

        value = strtol(buffer, &endptr, 10);

        if (*endptr == '\0') {
            return (int)value;
        }

        printf("Ошибка: введите корректное целое число.\n");
    }
}

void pause_console(void) {
    char buffer[8];
    read_line("\nНажмите Enter, чтобы продолжить...", buffer, sizeof(buffer));
}

void print_menu(void) {
    printf("\n ТЕЛЕФОННАЯ КНИГА \n");
    printf("1. Добавить контакт\n");
    printf("2. Редактировать контакт\n");
    printf("3. Удалить контакт\n");
    printf("4. Показать все контакты\n");
    printf("0. Выход\n");
}

void print_contacts(const ContactBook *book) {
    int i;

    if (book->count == 0) {
        printf("\nСписок контактов пуст.\n");
        return;
    }

    printf("\n КОНТАКТЫ \n");

    for (i = 0; i < book->count; i++) {
        const Contact *c = &book->contacts[i];

        printf("\nКонтакт #%d\n", i + 1);
        printf("Фамилия: %s\n", c->surname);
        printf("Имя: %s\n", c->name);
        printf("Место работы: %s\n", show_or_dash(c->workplace));
        printf("Должность: %s\n", show_or_dash(c->position));
        printf("Телефон: %s\n", show_or_dash(c->phone));
        printf("Email: %s\n", show_or_dash(c->email));
        printf("Соцсети: %s\n", show_or_dash(c->social));
        printf("Мессенджеры: %s\n", show_or_dash(c->messenger));
    }
}

int add_contact(ContactBook *book) {
    Contact *contact;

    if (book->count >= MAX_CONTACTS) {
        printf("Ошибка: телефонная книга заполнена.\n");
        return -1;
    }

    contact = &book->contacts[book->count];

    do {
        read_line("Введите фамилию: ", contact->surname, sizeof(contact->surname));
        if (contact->surname[0] == '\0') {
            printf("Фамилия обязательна для заполнения.\n");
        }
    } while (contact->surname[0] == '\0');

    do {
        read_line("Введите имя: ", contact->name, sizeof(contact->name));
        if (contact->name[0] == '\0') {
            printf("Имя обязательно для заполнения.\n");
        }
    } while (contact->name[0] == '\0');

    read_line("Введите место работы: ", contact->workplace, sizeof(contact->workplace));
    read_line("Введите должность: ", contact->position, sizeof(contact->position));
    read_line("Введите номер телефона: ", contact->phone, sizeof(contact->phone));
    read_line("Введите адрес электронной почты: ", contact->email, sizeof(contact->email));
    read_line("Введите ссылку на страницу в соцсетях: ", contact->social, sizeof(contact->social));
    read_line("Введите профиль в мессенджере: ", contact->messenger, sizeof(contact->messenger));

    book->count++;
    printf("Контакт успешно добавлен.\n");
    return 0;
}

int edit_contact(ContactBook *book, int index) {
    Contact *contact;
    char buffer[256];

    if (index < 0 || index >= book->count) {
        printf("Ошибка: контакта с таким номером не существует.\n");
        return -1;
    }

    contact = &book->contacts[index];

    printf("\nРедактирование контакта #%d\n", index + 1);
    printf("Enter — оставить текущее значение, '-' — очистить необязательное поле.\n");

    read_line("Новая фамилия: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        copy_field(contact->surname, sizeof(contact->surname), buffer);
    }

    read_line("Новое имя: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        copy_field(contact->name, sizeof(contact->name), buffer);
    }

    if (contact->surname[0] == '\0' || contact->name[0] == '\0') {
        printf("Ошибка: имя и фамилия не могут быть пустыми.\n");
        return -1;
    }

    update_optional_field("Новое место работы: ", contact->workplace, sizeof(contact->workplace));
    update_optional_field("Новая должность: ", contact->position, sizeof(contact->position));
    update_optional_field("Новый номер телефона: ", contact->phone, sizeof(contact->phone));
    update_optional_field("Новый email: ", contact->email, sizeof(contact->email));
    update_optional_field("Новая ссылка на соцсети: ", contact->social, sizeof(contact->social));
    update_optional_field("Новый профиль в мессенджере: ", contact->messenger, sizeof(contact->messenger));

    printf("Контакт успешно изменён.\n");
    return 0;
}

int delete_contact(ContactBook *book, int index) {
    int i;

    if (index < 0 || index >= book->count) {
        printf("Ошибка: контакта с таким номером не существует.\n");
        return -1;
    }

    for (i = index; i < book->count - 1; i++) {
        book->contacts[i] = book->contacts[i + 1];
    }

    book->count--;
    printf("Контакт успешно удалён.\n");
    return 0;
}

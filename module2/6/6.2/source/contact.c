#include "contact.h"

#include <stdio.h>
#include <string.h>

static const char *show_or_dash(const char *value)
{
    return value[0] == '\0' ? "-" : value;
}

void contact_init(Contact *contact)
{
    if (contact != NULL) {
        memset(contact, 0, sizeof(*contact));
    }
}

void contact_copy_field(char *dest, size_t dest_size, const char *src)
{
    if (dest == NULL || src == NULL || dest_size == 0) {
        return;
    }

    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

int contact_compare(const Contact *left, const Contact *right)
{
    int result;

    result = strcmp(left->surname, right->surname);
    if (result != 0) {
        return result;
    }

    result = strcmp(left->name, right->name);
    if (result != 0) {
        return result;
    }

    result = strcmp(left->phone, right->phone);
    if (result != 0) {
        return result;
    }

    return strcmp(left->email, right->email);
}

void contact_print(const Contact *contact, int number)
{
    printf("\nКонтакт #%d\n", number);
    printf("Фамилия: %s\n", contact->surname);
    printf("Имя: %s\n", contact->name);
    printf("Место работы: %s\n", show_or_dash(contact->workplace));
    printf("Должность: %s\n", show_or_dash(contact->position));
    printf("Телефон: %s\n", show_or_dash(contact->phone));
    printf("Email: %s\n", show_or_dash(contact->email));
    printf("Соцсети: %s\n", show_or_dash(contact->social));
    printf("Мессенджеры: %s\n", show_or_dash(contact->messenger));
}

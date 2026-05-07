#include "contact_book.h"
#include "input.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    int number;
} PrintContext;

static void update_optional_field(const char *prompt, char *field, size_t field_size)
{
    char buffer[256];

    read_line(prompt, buffer, sizeof(buffer));

    if (strcmp(buffer, "-") == 0) {
        field[0] = '\0';
        return;
    }

    if (buffer[0] != '\0') {
        contact_copy_field(field, field_size, buffer);
    }
}

static void print_contact_callback(const Contact *contact, void *context)
{
    PrintContext *print_context = context;

    contact_print(contact, print_context->number);
    print_context->number++;
}

void init_contact_book(ContactBook *book)
{
    if (book == NULL) {
        return;
    }

    contact_tree_init(&book->contacts);
}

void destroy_contact_book(ContactBook *book)
{
    if (book == NULL) {
        return;
    }

    contact_tree_destroy(&book->contacts);
}

int contact_book_is_empty(const ContactBook *book)
{
    return book == NULL || contact_tree_is_empty(&book->contacts);
}

void print_menu(void)
{
    printf("\nТЕЛЕФОННАЯ КНИГА\n");
    printf("1. Добавить контакт\n");
    printf("2. Редактировать контакт\n");
    printf("3. Удалить контакт\n");
    printf("4. Показать все контакты\n");
    printf("5. Выполнить балансировку дерева\n");
    printf("0. Выход\n");
}

void print_contacts(const ContactBook *book)
{
    PrintContext context = {1};

    if (contact_book_is_empty(book)) {
        printf("\nСписок контактов пуст.\n");
        return;
    }

    printf("\nКОНТАКТЫ\n");
    printf("Количество контактов: %d\n", book->contacts.count);
    printf("Балансировка выполняется автоматически после каждых %d изменений.\n",
           CONTACT_TREE_BALANCE_PERIOD);

    contact_tree_foreach_in_order(&book->contacts, print_contact_callback, &context);
}

int add_contact(ContactBook *book)
{
    Contact contact;

    contact_init(&contact);

    do {
        read_line("Введите фамилию: ", contact.surname, sizeof(contact.surname));
        if (contact.surname[0] == '\0') {
            printf("Фамилия обязательна для заполнения.\n");
        }
    } while (contact.surname[0] == '\0');

    do {
        read_line("Введите имя: ", contact.name, sizeof(contact.name));
        if (contact.name[0] == '\0') {
            printf("Имя обязательно для заполнения.\n");
        }
    } while (contact.name[0] == '\0');

    read_line("Введите место работы: ", contact.workplace, sizeof(contact.workplace));
    read_line("Введите должность: ", contact.position, sizeof(contact.position));
    read_line("Введите номер телефона: ", contact.phone, sizeof(contact.phone));
    read_line("Введите адрес электронной почты: ", contact.email, sizeof(contact.email));
    read_line("Введите ссылку на страницу в соцсетях: ", contact.social, sizeof(contact.social));
    read_line("Введите профиль в мессенджере: ", contact.messenger, sizeof(contact.messenger));

    if (contact_tree_insert_sorted(&book->contacts, &contact) != 0) {
        printf("Ошибка: не удалось выделить память для нового контакта.\n");
        return -1;
    }

    printf("Контакт успешно добавлен. Данные хранятся в бинарном дереве.\n");
    return 0;
}

int edit_contact(ContactBook *book, int index)
{
    ContactTreeNode *node;
    Contact updated;
    char buffer[256];

    node = contact_tree_get_node_by_index(&book->contacts, index);
    if (node == NULL) {
        printf("Ошибка: контакта с таким номером не существует.\n");
        return -1;
    }

    updated = node->data;

    printf("\nРедактирование контакта #%d\n", index + 1);
    printf("Enter — оставить текущее значение, '-' — очистить необязательное поле.\n");

    read_line("Новая фамилия: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        contact_copy_field(updated.surname, sizeof(updated.surname), buffer);
    }

    read_line("Новое имя: ", buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        contact_copy_field(updated.name, sizeof(updated.name), buffer);
    }

    if (updated.surname[0] == '\0' || updated.name[0] == '\0') {
        printf("Ошибка: имя и фамилия не могут быть пустыми.\n");
        return -1;
    }

    update_optional_field("Новое место работы: ", updated.workplace, sizeof(updated.workplace));
    update_optional_field("Новая должность: ", updated.position, sizeof(updated.position));
    update_optional_field("Новый номер телефона: ", updated.phone, sizeof(updated.phone));
    update_optional_field("Новый email: ", updated.email, sizeof(updated.email));
    update_optional_field("Новая ссылка на соцсети: ", updated.social, sizeof(updated.social));
    update_optional_field("Новый профиль в мессенджере: ", updated.messenger, sizeof(updated.messenger));

    if (contact_tree_update_by_index(&book->contacts, index, &updated) != 0) {
        printf("Ошибка: не удалось изменить контакт.\n");
        return -1;
    }

    printf("Контакт успешно изменён. Положение в дереве обновлено.\n");
    return 0;
}

int delete_contact(ContactBook *book, int index)
{
    if (contact_tree_delete_by_index(&book->contacts, index) != 0) {
        printf("Ошибка: контакта с таким номером не существует.\n");
        return -1;
    }

    printf("Контакт успешно удалён.\n");
    return 0;
}

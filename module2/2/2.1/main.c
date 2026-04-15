#include "contact_book.h"

#include <stdio.h>

int main(void) {
    ContactBook book;
    int choice;
    int index;
    int running = 1;

    init_contact_book(&book);

    while (running) {
        print_menu();
        choice = read_int("\nВыберите пункт меню: ");

        switch (choice) {
            case 1:
                add_contact(&book);
                pause_console();
                break;

            case 2:
                if (book.count == 0) {
                    printf("Список контактов пуст.\n");
                    pause_console();
                    break;
                }

                print_contacts(&book);
                index = read_int("\nВведите номер контакта для редактирования: ");
                edit_contact(&book, index - 1);
                pause_console();
                break;

            case 3:
                if (book.count == 0) {
                    printf("Список контактов пуст.\n");
                    pause_console();
                    break;
                }

                print_contacts(&book);
                index = read_int("\nВведите номер контакта для удаления: ");
                delete_contact(&book, index - 1);
                pause_console();
                break;

            case 4:
                print_contacts(&book);
                pause_console();
                break;

            case 0:
                printf("Выход из программы.\n");
                running = 0;
                break;

            default:
                printf("Ошибка: такого пункта меню нет.\n");
                pause_console();
                break;
        }
    }

    return 0;
}

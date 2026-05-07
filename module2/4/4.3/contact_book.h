#ifndef CONTACT_BOOK_H
#define CONTACT_BOOK_H

#include "contact_tree.h"

typedef struct {
    ContactTree contacts;
} ContactBook;

void init_contact_book(ContactBook *book);
void destroy_contact_book(ContactBook *book);
int contact_book_is_empty(const ContactBook *book);

void print_menu(void);
void print_contacts(const ContactBook *book);

int add_contact(ContactBook *book);
int edit_contact(ContactBook *book, int index);
int delete_contact(ContactBook *book, int index);

#endif

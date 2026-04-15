#ifndef CONTACT_BOOK_H
#define CONTACT_BOOK_H

#include <stddef.h>

#define MAX_CONTACTS 100
#define NAME_LEN 50
#define SURNAME_LEN 50
#define WORKPLACE_LEN 100
#define POSITION_LEN 100
#define PHONE_LEN 30
#define EMAIL_LEN 100
#define SOCIAL_LEN 150
#define MESSENGER_LEN 150

typedef struct {
    char name[NAME_LEN];
    char surname[SURNAME_LEN];
    char workplace[WORKPLACE_LEN];
    char position[POSITION_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char social[SOCIAL_LEN];
    char messenger[MESSENGER_LEN];
} Contact;

typedef struct {
    Contact contacts[MAX_CONTACTS];
    int count;
} ContactBook;

void init_contact_book(ContactBook *book);
void print_menu(void);
void print_contacts(const ContactBook *book);

int add_contact(ContactBook *book);
int edit_contact(ContactBook *book, int index);
int delete_contact(ContactBook *book, int index);

void read_line(const char *prompt, char *buffer, size_t size);
int read_int(const char *prompt);
void pause_console(void);

#endif

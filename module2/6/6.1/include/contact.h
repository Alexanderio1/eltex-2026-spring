#ifndef CONTACT_H
#define CONTACT_H

#include <stddef.h>

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

void contact_init(Contact *contact);
void contact_copy_field(char *dest, size_t dest_size, const char *src);
int contact_compare(const Contact *left, const Contact *right);
void contact_print(const Contact *contact, int number);

#endif

#ifndef CONTACT_LIST_H
#define CONTACT_LIST_H

#include "contact.h"

typedef struct ContactNode {
    Contact data;
    struct ContactNode *prev;
    struct ContactNode *next;
} ContactNode;

typedef struct {
    ContactNode *head;
    ContactNode *tail;
    int count;
} ContactList;

void contact_list_init(ContactList *list);
void contact_list_destroy(ContactList *list);

int contact_list_insert_sorted(ContactList *list, const Contact *contact);
ContactNode *contact_list_get_node_by_index(const ContactList *list, int index);
void contact_list_reinsert_node_sorted(ContactList *list, ContactNode *node);
int contact_list_delete_by_index(ContactList *list, int index);

#endif

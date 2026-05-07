#include "contact_list.h"

#include <stdlib.h>

static ContactNode *contact_node_create(const Contact *contact)
{
    ContactNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->data = *contact;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

static void contact_list_unlink_node(ContactList *list, ContactNode *node)
{
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;
    list->count--;
}

static void contact_list_insert_existing_sorted(ContactList *list, ContactNode *node)
{
    ContactNode *current;

    node->prev = NULL;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        list->count++;
        return;
    }

    if (contact_compare(&node->data, &list->head->data) < 0) {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
        list->count++;
        return;
    }

    current = list->head;

    while (current->next != NULL &&
           contact_compare(&node->data, &current->next->data) >= 0) {
        current = current->next;
    }

    node->next = current->next;
    node->prev = current;

    if (current->next != NULL) {
        current->next->prev = node;
    } else {
        list->tail = node;
    }

    current->next = node;
    list->count++;
}

void contact_list_init(ContactList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

void contact_list_destroy(ContactList *list)
{
    ContactNode *current = list->head;

    while (current != NULL) {
        ContactNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

int contact_list_insert_sorted(ContactList *list, const Contact *contact)
{
    ContactNode *node = contact_node_create(contact);

    if (node == NULL) {
        return -1;
    }

    contact_list_insert_existing_sorted(list, node);

    return 0;
}

ContactNode *contact_list_get_node_by_index(const ContactList *list, int index)
{
    ContactNode *current;
    int i;

    if (list == NULL || index < 0 || index >= list->count) {
        return NULL;
    }

    current = list->head;

    for (i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}

void contact_list_reinsert_node_sorted(ContactList *list, ContactNode *node)
{
    if (list == NULL || node == NULL) {
        return;
    }

    contact_list_unlink_node(list, node);
    contact_list_insert_existing_sorted(list, node);
}

int contact_list_delete_by_index(ContactList *list, int index)
{
    ContactNode *node = contact_list_get_node_by_index(list, index);

    if (node == NULL) {
        return -1;
    }

    contact_list_unlink_node(list, node);
    free(node);

    return 0;
}

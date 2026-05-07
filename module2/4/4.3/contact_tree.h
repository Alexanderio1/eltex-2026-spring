#ifndef CONTACT_TREE_H
#define CONTACT_TREE_H

#include "contact.h"

#define CONTACT_TREE_BALANCE_PERIOD 3

typedef struct ContactTreeNode {
    Contact data;
    struct ContactTreeNode *left;
    struct ContactTreeNode *right;
} ContactTreeNode;

typedef struct {
    ContactTreeNode *root;
    int count;
    int operations_since_balance;
} ContactTree;

void contact_tree_init(ContactTree *tree);
void contact_tree_destroy(ContactTree *tree);
int contact_tree_is_empty(const ContactTree *tree);

int contact_tree_insert_sorted(ContactTree *tree, const Contact *contact);
ContactTreeNode *contact_tree_get_node_by_index(const ContactTree *tree, int index);
int contact_tree_update_by_index(ContactTree *tree, int index, const Contact *updated);
int contact_tree_delete_by_index(ContactTree *tree, int index);

int contact_tree_balance(ContactTree *tree);

void contact_tree_foreach_in_order(const ContactTree *tree,
                                   void (*visit)(const Contact *contact, void *context),
                                   void *context);

#endif

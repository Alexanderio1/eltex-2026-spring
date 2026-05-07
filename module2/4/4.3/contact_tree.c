#include "contact_tree.h"

#include <stdlib.h>
#include <string.h>

static int contact_tree_compare_contacts(const Contact *left, const Contact *right)
{
    int result;

    result = contact_compare(left, right);
    if (result != 0) {
        return result;
    }

    result = strcmp(left->workplace, right->workplace);
    if (result != 0) {
        return result;
    }

    result = strcmp(left->position, right->position);
    if (result != 0) {
        return result;
    }

    result = strcmp(left->social, right->social);
    if (result != 0) {
        return result;
    }

    return strcmp(left->messenger, right->messenger);
}

static ContactTreeNode *contact_tree_node_create(const Contact *contact)
{
    ContactTreeNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->data = *contact;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static void contact_tree_destroy_nodes(ContactTreeNode *node)
{
    if (node == NULL) {
        return;
    }

    contact_tree_destroy_nodes(node->left);
    contact_tree_destroy_nodes(node->right);
    free(node);
}

static ContactTreeNode *contact_tree_insert_existing(ContactTreeNode *root,
                                                     ContactTreeNode *node)
{
    int compare_result;

    if (root == NULL) {
        return node;
    }

    compare_result = contact_tree_compare_contacts(&node->data, &root->data);

    if (compare_result < 0) {
        root->left = contact_tree_insert_existing(root->left, node);
    } else {
        root->right = contact_tree_insert_existing(root->right, node);
    }

    return root;
}

static void contact_tree_fill_array(ContactTreeNode *root,
                                    ContactTreeNode **nodes,
                                    int *index)
{
    if (root == NULL) {
        return;
    }

    contact_tree_fill_array(root->left, nodes, index);

    nodes[*index] = root;
    (*index)++;

    contact_tree_fill_array(root->right, nodes, index);

    root->left = NULL;
    root->right = NULL;
}

static ContactTreeNode *contact_tree_build_balanced(ContactTreeNode **nodes,
                                                    int left,
                                                    int right)
{
    int middle;
    ContactTreeNode *root;

    if (left > right) {
        return NULL;
    }

    middle = left + (right - left) / 2;
    root = nodes[middle];

    root->left = contact_tree_build_balanced(nodes, left, middle - 1);
    root->right = contact_tree_build_balanced(nodes, middle + 1, right);

    return root;
}

static void contact_tree_balance_if_needed(ContactTree *tree)
{
    if (tree == NULL) {
        return;
    }

    tree->operations_since_balance++;

    if (tree->operations_since_balance >= CONTACT_TREE_BALANCE_PERIOD) {
        if (contact_tree_balance(tree) == 0) {
            tree->operations_since_balance = 0;
        }
    }
}

static ContactTreeNode *contact_tree_find_by_index(ContactTreeNode *node,
                                                   int index,
                                                   int *current_index)
{
    ContactTreeNode *result;

    if (node == NULL) {
        return NULL;
    }

    result = contact_tree_find_by_index(node->left, index, current_index);
    if (result != NULL) {
        return result;
    }

    if (*current_index == index) {
        return node;
    }

    (*current_index)++;

    return contact_tree_find_by_index(node->right, index, current_index);
}

static ContactTreeNode *contact_tree_detach_min(ContactTreeNode *root,
                                                ContactTreeNode **min_node)
{
    if (root->left == NULL) {
        *min_node = root;
        return root->right;
    }

    root->left = contact_tree_detach_min(root->left, min_node);
    return root;
}

static ContactTreeNode *contact_tree_delete_by_value(ContactTreeNode *root,
                                                     const Contact *contact,
                                                     int *deleted)
{
    int compare_result;
    ContactTreeNode *new_root;
    ContactTreeNode *successor;

    if (root == NULL) {
        return NULL;
    }

    compare_result = contact_tree_compare_contacts(contact, &root->data);

    if (compare_result < 0) {
        root->left = contact_tree_delete_by_value(root->left, contact, deleted);
        return root;
    }

    if (compare_result > 0) {
        root->right = contact_tree_delete_by_value(root->right, contact, deleted);
        return root;
    }

    *deleted = 1;

    if (root->left == NULL) {
        new_root = root->right;
        free(root);
        return new_root;
    }

    if (root->right == NULL) {
        new_root = root->left;
        free(root);
        return new_root;
    }

    successor = NULL;
    root->right = contact_tree_detach_min(root->right, &successor);

    successor->left = root->left;
    successor->right = root->right;

    free(root);
    return successor;
}

static void contact_tree_traverse_in_order(ContactTreeNode *node,
                                           void (*visit)(const Contact *contact, void *context),
                                           void *context)
{
    if (node == NULL) {
        return;
    }

    contact_tree_traverse_in_order(node->left, visit, context);
    visit(&node->data, context);
    contact_tree_traverse_in_order(node->right, visit, context);
}

void contact_tree_init(ContactTree *tree)
{
    if (tree == NULL) {
        return;
    }

    tree->root = NULL;
    tree->count = 0;
    tree->operations_since_balance = 0;
}

void contact_tree_destroy(ContactTree *tree)
{
    if (tree == NULL) {
        return;
    }

    contact_tree_destroy_nodes(tree->root);
    tree->root = NULL;
    tree->count = 0;
    tree->operations_since_balance = 0;
}

int contact_tree_is_empty(const ContactTree *tree)
{
    return tree == NULL || tree->count == 0;
}

int contact_tree_insert_sorted(ContactTree *tree, const Contact *contact)
{
    ContactTreeNode *node;

    if (tree == NULL || contact == NULL) {
        return -1;
    }

    node = contact_tree_node_create(contact);
    if (node == NULL) {
        return -1;
    }

    tree->root = contact_tree_insert_existing(tree->root, node);
    tree->count++;
    contact_tree_balance_if_needed(tree);

    return 0;
}

ContactTreeNode *contact_tree_get_node_by_index(const ContactTree *tree, int index)
{
    int current_index = 0;

    if (tree == NULL || index < 0 || index >= tree->count) {
        return NULL;
    }

    return contact_tree_find_by_index(tree->root, index, &current_index);
}

int contact_tree_update_by_index(ContactTree *tree, int index, const Contact *updated)
{
    ContactTreeNode *old_node;
    ContactTreeNode *new_node;
    Contact old_contact;
    int deleted = 0;

    if (tree == NULL || updated == NULL || index < 0 || index >= tree->count) {
        return -1;
    }

    old_node = contact_tree_get_node_by_index(tree, index);
    if (old_node == NULL) {
        return -1;
    }

    new_node = contact_tree_node_create(updated);
    if (new_node == NULL) {
        return -1;
    }

    old_contact = old_node->data;
    tree->root = contact_tree_delete_by_value(tree->root, &old_contact, &deleted);

    if (!deleted) {
        free(new_node);
        return -1;
    }

    tree->root = contact_tree_insert_existing(tree->root, new_node);
    contact_tree_balance_if_needed(tree);

    return 0;
}

int contact_tree_delete_by_index(ContactTree *tree, int index)
{
    ContactTreeNode *node;
    Contact contact;
    int deleted = 0;

    if (tree == NULL || index < 0 || index >= tree->count) {
        return -1;
    }

    node = contact_tree_get_node_by_index(tree, index);
    if (node == NULL) {
        return -1;
    }

    contact = node->data;
    tree->root = contact_tree_delete_by_value(tree->root, &contact, &deleted);

    if (!deleted) {
        return -1;
    }

    tree->count--;
    contact_tree_balance_if_needed(tree);

    return 0;
}

int contact_tree_balance(ContactTree *tree)
{
    ContactTreeNode **nodes;
    int index = 0;

    if (tree == NULL) {
        return -1;
    }

    if (tree->count < 2) {
        tree->operations_since_balance = 0;
        return 0;
    }

    nodes = malloc((size_t)tree->count * sizeof(*nodes));
    if (nodes == NULL) {
        return -1;
    }

    contact_tree_fill_array(tree->root, nodes, &index);
    tree->root = contact_tree_build_balanced(nodes, 0, tree->count - 1);

    free(nodes);
    tree->operations_since_balance = 0;

    return 0;
}

void contact_tree_foreach_in_order(const ContactTree *tree,
                                   void (*visit)(const Contact *contact, void *context),
                                   void *context)
{
    if (tree == NULL || visit == NULL) {
        return;
    }

    contact_tree_traverse_in_order(tree->root, visit, context);
}

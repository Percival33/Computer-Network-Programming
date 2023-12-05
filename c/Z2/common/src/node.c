#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "node.h"

Node* create_node() {
    Node* new_node = (Node*)malloc(sizeof(Node));
    assert(new_node != NULL);

    new_node->next = NULL;
    new_node->a = 0;
    new_node->b = 0;
    new_node->text = NULL;
    assign_text(new_node, "test\0", 5);
    return new_node;
}

void print_nodes(Node* node) {
    while(node != NULL) {
        printf("%d %d %s\n", node->a, node->b, node->text);
        node = node->next;
    }
}

void assign_text(Node* node, char* new_text, int len) {
    if(node->text != NULL) {
        free(node->text);
    }

    node->text = (char*)malloc(len * sizeof(char));
    strncpy(node->text, new_text, len);
}

void add_node(Node* root, Node *new_node) {
    assert(root != NULL);

    while(root->next != NULL) root = root->next;
    root->next = new_node;
}

void delete_node(Node* node) {
    if(node == NULL)
        return;

    if(node->text != NULL)
        free(node->text);

    free(node);
}

void delete_list(Node* last) {
    Node* prev;
    while(last->next != NULL) {
        prev = last;
        last = last->next;
        delete_node(prev);
    }
}

int length(Node* root) {
    int ret = 0;
    while (root != NULL) {
        root = root->next;
        ret++;
    }
    return ret;
}

Node* prepare_data(Node* root) {
    assert(root != NULL);
    char* text = (char*)malloc(10 * sizeof(char));
    strncpy(text, "testVal", 7);
//    add_node(root, 1, 1, text);
    LOG_INFO("added new node");
    return root;
}
#ifndef NODE_H
#define NODE_H
#include <stdint.h>
#include "common.h"

typedef struct Node Node;

struct Node {
    struct Node* next;
    uint16_t a;
    uint32_t b;
    char* text;
};

Node* create_node();

void print_nodes(Node* node);

void assign_text(Node* node, char* new_text, int len);

void add_node(Node* root, Node *new_node);

void delete_node(Node* node);

void delete_list(Node* last) ;

int length(Node* root);

Node* prepare_data(Node* root);

#endif /* !NODE_H */
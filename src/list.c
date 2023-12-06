#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define WORD_SIZE 64

struct node *list_init()
{
    struct node *head = malloc(sizeof(struct node));
    if (head == NULL) {
        fputs("Failed to initialize list", stderr);
        exit(EXIT_FAILURE);
    }

    head->word = malloc(WORD_SIZE);
    head->next = NULL;

    return head;
}

void list_add(struct node **tail)
{
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        fputs("Failed to add new node to list", stderr);
        exit(EXIT_FAILURE);
    }

    new_node->word = malloc(WORD_SIZE);
    new_node->next = NULL;

    (*tail)->next = new_node;
    *tail = new_node;
}

void list_term(struct node *head, bool success)
{
    if (!success) {
        fputs("Syntax error. Incorrect number of quotes.\n", stderr);
    }

    while (head) {
        if (success && strlen(head->word) > 0) {
            printf("[%s]\n", head->word);
        }

        struct node *tmp = head->next;
        free(head->word);
        free(head);
        head = tmp;
    }
}


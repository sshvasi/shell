#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "executor.h"
#include "list.h"

static void print(struct node *n)
{
    printf("[%s]\n", n->word);
}

void execute(struct list *ls)
{
    char *command = ls->head->word;

    if (strcmp(command, "exit") == 0) {
        free_list(ls);
        exit(EXIT_SUCCESS);
    } else if (strcmp(command, "clear") == 0) {
        fputs("\e[1;1H\e[2J", stdout);
    } else {
        printf("Command not found: %s\n", command);
        for_each_node(ls, &print);
    }
}

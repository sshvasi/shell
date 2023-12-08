#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "executor.h"
#include "list.h"

static void print(struct node *n)
{
    printf("[%s]\n", n->token);
}

void execute(struct list *ls, struct buffer *buff)
{
    if (ls->head == NULL) {
        TRACE("[EXECUTOR] Failed to execute empty command.\n");
        return;
    }

    TRACE("[EXECUTOR] Execute %s.\n", ls->head->token);

    char *command = ls->head->token;

    if (strcmp(command, "exit") == 0) {
        free_buffer(buff);
        free_list(ls);
        exit(EXIT_SUCCESS);
    } else if (strcmp(command, "clear") == 0) {
        fputs("\e[1;1H\e[2J", stdout);
    } else {
        printf("Command not found: %s. Tokens:\n", command);
        for_each_node(ls, &print);
    }
}

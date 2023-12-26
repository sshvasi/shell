#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "analyser.h"
#include "debug.h"
#include "executor.h"
#include "list.h"

static void print_node(struct node *n)
{
    printf("[%s]\n", n->token);
}

void execute(struct analyser *an)
{
    char *command;

    if (an == NULL) {
        TRACE("[EXECUTOR] Failed to execute command: received NULL pointer for analyser.\n");
        return;
    }

    if (an->tokens == NULL) {
        TRACE("[EXECUTOR] Failed to execute command: received NULL pointer for tokens.\n");
        return;
    }

    if (an->tokens->head == NULL) {
        TRACE("[EXECUTOR] Failed to execute empty command.\n");
        return;
    }

    TRACE("[EXECUTOR] Execute '%s'.\n", an->tokens->head->token);

    command = an->tokens->head->token;

    if (strcmp(command, "exit") == 0) {
        free_analyser(an);
        exit(EXIT_SUCCESS);
    } else if (strcmp(command, "clear") == 0) {
        fputs("\e[1;1H\e[2J", stdout);
    } else {
        printf("Command not found: %s. Tokens:\n", command);
        for_each_node(an->tokens, &print_node);
    }
}

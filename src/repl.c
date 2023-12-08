#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "analyser.h"
#include "buffer.h"
#include "list.h"
#include "prompt.h"
#include "repl.h"

void init_repl(struct list *ls, struct buffer *buff)
{
    print_prompt(singleline_prompt);

    int ch;
    while ((ch = getchar()) != EOF) {
        analyse(ch, ls, buff);
    }

    putchar('\n');

    free_buffer(buff);
    free_list(ls);
}

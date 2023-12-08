#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "buffer.h"
#include "list.h"
#include "tokenizer.h"
#include "repl.h"

void init_repl(struct list *ls, struct buffer *buff)
{
    while (true) {
        fputs("> ", stdout);
        fflush(stdout);

        int ch;
        while ((ch = getchar()) != EOF) {
            tokenize(ch, ls, buff);

            if (ch == '\n') {
                break;
            }
        }

        if (ch == EOF) {
            putchar('\n');

            free_buffer(buff);
            free_list(ls);

            break;
        }
    }
}

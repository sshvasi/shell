#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "buffer.h"
#include "debug.h"
#include "tokenizer.h"
#include "repl.h"

void init_repl(struct list *ls, struct buffer *buff)
{
    while (true) {
        fputs("> ", stdout);
        fflush(stdout);

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            tokenize(ch, ls, buff);
        }

        if (ch == '\n') {
            tokenize(ch, ls, buff);
            continue;
        }

        if (ch == EOF) {
            putchar('\n');
            tokenize(ch, ls, buff);
            break;
        }
    }
}

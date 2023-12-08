#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "analyser.h"
#include "buffer.h"
#include "debug.h"
#include "list.h"
#include "prompt.h"

int main(void) {
    struct list *ls = init_list();
    struct buffer *buff = init_buffer();

    print_prompt(singleline_prompt);

    int ch;
    while ((ch = getchar()) != EOF) {
        analyse(ch, ls, buff);
    }

    putchar('\n');

    TRACE("[MAIN] EOF. Clean up memory.\n");

    free_buffer(buff);
    free_list(ls);

    return EXIT_SUCCESS;
}

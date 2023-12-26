#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "analyser.h"
#include "buffer.h"
#include "debug.h"
#include "list.h"
#include "prompt.h"

int main(void) {
    struct analyser *an;
    int ch;

    an = init_analyser(
        init_buffer(),
        init_list()
    );

    print_prompt(singleline_prompt);

    while ((ch = getchar()) != EOF) {
        analyse(an, ch);
    }

    putchar('\n');

    TRACE("[MAIN] EOF. Clean up memory.\n");

    free_analyser(an);

    return EXIT_SUCCESS;
}

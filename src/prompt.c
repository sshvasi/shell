#include <stdio.h>

#include "prompt.h"

void print_prompt(enum prompt_type type)
{
    printf("%c ", type == singleline_prompt ? '$' : '>');
    fflush(stdout);
}

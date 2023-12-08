#ifndef PROMPT_H
#define PROMPT_H

enum prompt_type {
    singleline_prompt,
    multiline_prompt
};

void print_prompt(enum prompt_type type);

#endif /* PROMPT_H */

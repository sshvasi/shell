#ifndef REPL_H
#define REPL_H

#include <stdio.h>
#include "list.h"

enum state {
    normal,
    quote,
    escape
};

enum event {
    quote_ch = '"',
    escape_ch = '\\',
    space_ch = ' ',
    tab_ch = '\t',
    nl_ch = '\n',
    eof = EOF
    /* default = any character */
};

typedef enum state (*processor)(enum event);

void init_repl();

#endif /* REPL_H */

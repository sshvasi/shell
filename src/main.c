#include <stdlib.h>
#include "list.h"
#include "repl.h"

int main(void) {
    repl_loop(list_init());
    exit(EXIT_SUCCESS);
}

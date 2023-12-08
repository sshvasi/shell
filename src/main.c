#include <stdlib.h>

#include "buffer.h"
#include "list.h"
#include "repl.h"

int main(void) {
    init_repl(init_list(),
              init_buffer());
    exit(EXIT_SUCCESS);
}

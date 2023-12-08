#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG 1

#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define TRACE_COLOR(color, ...)                     \
    do                                              \
    {                                               \
        if (DEBUG)                                  \
        {                                           \
            fprintf(stderr, color);                 \
            fprintf(stderr, __VA_ARGS__);           \
            fprintf(stderr, ANSI_COLOR_RESET);      \
        }                                           \
    } while (0)

#define TRACE(...) TRACE_COLOR(ANSI_COLOR_BLUE, __VA_ARGS__)

#endif /* DEBUG_H */

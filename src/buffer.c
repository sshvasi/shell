#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "debug.h"
#include "buffer.h"

struct buffer *init_buffer()
{
    TRACE("[BUFFER] Initilize.\n");

    struct buffer *b;
    if ((b = malloc(sizeof(struct buffer))) == NULL) {
        perror("Failed to allocate memory for buffer.");
        return NULL;
    }

    if ((b->store = malloc(BUFFER_SIZE + 1)) == NULL) {
        perror("Failed to allocate memory for buffer store.");
        free(b);
        return NULL;
    }

    b->store[0] = '\0';
    b->length = 0;
    b->capacity = BUFFER_SIZE + 1;

    return b;
}

void add_to_buffer(struct buffer *b, int ch)
{
    TRACE("[BUFFER] Add '%c'\n", ch);

    if (b == NULL) {
        fputs("Failed to add to buffer: NULL pointer received.\n", stderr);
        return;
    }

    if (b->length == b->capacity - 1) {
        TRACE("Buffer is full with length %d. Resizing.\n", b->length);

        char *new_store;
        if ((new_store = malloc(b->capacity * 2)) == NULL) {
            perror("Failed to allocate memory for buffer store during resizing.");
            free(b);
            exit(EXIT_FAILURE);
        }

        strcpy(new_store, b->store);
        free(b->store);
        b->store = new_store;
        b->capacity *= 2;
    }

    b->store[b->length++] = ch;
    b->store[b->length] = '\0';
}

void empty_buffer(struct buffer *b)
{
    TRACE("[BUFFER] Empty '%s'.\n", b->store);

    if (b == NULL) {
        fputs("Failed to empty buffer: NULL pointer received.\n", stderr);
        return;
    }

    b->length = 0;
    b->store[0] = '\0';
}

void free_buffer(struct buffer *b)
{
    TRACE("[BUFFER] Free '%s'.\n", b->store);

    if (b == NULL) {
        fputs("Failed to free buffer: NULL pointer received.\n", stderr);
        return;
    }

    free(b->store);
    free(b);
}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "buffer.h"
#include "debug.h"

struct buffer *init_buffer()
{
    struct buffer *buff;

    TRACE("[BUFFER] Initilize.\n");

    buff = malloc(sizeof(struct buffer));
    if (buff == NULL) {
        perror("Failed to allocate memory for buffer.");
        exit(EXIT_FAILURE);
    }

    buff->store = malloc(BUFFER_CAPACITY + 1);
    if (buff->store == NULL) {
        perror("Failed to allocate memory for buffer store.");
        free(buff);
        exit(EXIT_FAILURE);
    }

    buff->store[0] = '\0';
    buff->length = 0;
    buff->capacity = BUFFER_CAPACITY + 1;

    return buff;
}

void resize_buffer(struct buffer *buff)
{
    char *new_store;

    TRACE("[BUFFER] Buffer is full with capacity %d. Resizing to %d.\n",
          buff->capacity, buff->capacity * 2);

    if (buff == NULL) {
        fputs("Failed to resize buffer: NULL pointer received.\n", stderr);
        return;
    }

    new_store = malloc(buff->capacity * 2);
    if (new_store == NULL) {
        fputs("Failed to allocate memory for buffer store during resizing.\n", stderr);
        empty_buffer(buff);
        return;
    }

    strcpy(new_store, buff->store);
    free(buff->store);
    buff->store = new_store;
    buff->capacity *= 2;
}

void add_to_buffer(struct buffer *buff, int ch)
{
    if (buff == NULL) {
        fputs("Failed to add to buffer: NULL pointer received.\n", stderr);
        return;
    }

    TRACE("[BUFFER] Add '%c'\n", ch);

    if (buff->length == buff->capacity - 1) {
        resize_buffer(buff);
    }

    buff->store[buff->length++] = ch;
    buff->store[buff->length] = '\0';
}

void empty_buffer(struct buffer *buff)
{
    if (buff == NULL) {
        fputs("Failed to empty buffer: NULL pointer received.\n", stderr);
        return;
    }

    TRACE("[BUFFER] Empty '%s'.\n", buff->store);

    buff->length = 0;
    buff->store[0] = '\0';
}

void free_buffer(struct buffer *buff)
{
    if (buff == NULL) {
        fputs("Failed to free buffer: NULL pointer received.\n", stderr);
        return;
    }

    TRACE("[BUFFER] Free '%s'.\n", buff->store);

    free(buff->store);
    free(buff);
}

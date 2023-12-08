#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 1024

struct buffer {
    char *store;
    int length;
    int capacity;
};

struct buffer *init_buffer();
void add_to_buffer(struct buffer *, int ch);
void empty_buffer(struct buffer *);
void free_buffer(struct buffer *);

#endif /* BUFFER_H */

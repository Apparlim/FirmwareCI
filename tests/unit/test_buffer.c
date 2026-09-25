#include "../../src/include/config.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t data[BUFFER_SIZE];
    uint32_t head;
    uint32_t tail;
} test_ring_buf_t;

static bool buf_push(test_ring_buf_t *buf, uint8_t val) {
    if (buf == NULL)
        return false;
    uint32_t next = (buf->head + 1U) % BUFFER_SIZE;
    if (next == buf->tail)
        return false;
    buf->data[buf->head] = val;
    buf->head = next;
    return true;
}

static bool buf_pop(test_ring_buf_t *buf, uint8_t *val) {
    if (buf == NULL || val == NULL)
        return false;
    if (buf->head == buf->tail)
        return false;
    *val = buf->data[buf->tail];
    buf->tail = (buf->tail + 1U) % BUFFER_SIZE;
    return true;
}

int main(void) {
    test_ring_buf_t buf = {0};
    uint8_t val = 0;

    assert(buf_push(&buf, 0xAA) == true);
    assert(buf_push(&buf, 0xBB) == true);
    assert(buf_pop(&buf, &val) == true && val == 0xAA);
    assert(buf_pop(&buf, &val) == true && val == 0xBB);
    assert(buf_pop(&buf, &val) == false);

    printf("test_buffer passed.\n");
    return 0;
}

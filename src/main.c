#include "config.h"
#include "drivers/uart.h"
#include <stdbool.h>

typedef struct {
    uint8_t data[BUFFER_SIZE];
    uint32_t head;
    uint32_t tail;
} ring_buffer_t;

static bool ring_buffer_push(ring_buffer_t *buf, uint8_t val) {
    if (buf == NULL) {
        return false;
    }
    uint32_t next = (buf->head + 1U) % BUFFER_SIZE;
    if (next == buf->tail) {
        return false;
    }
    buf->data[buf->head] = val;
    buf->head = next;
    return true;
}

static bool ring_buffer_pop(ring_buffer_t *buf, uint8_t *val) {
    if ((buf == NULL) || (val == NULL)) {
        return false;
    }
    if (buf->head == buf->tail) {
        return false;
    }
    *val = buf->data[buf->tail];
    buf->tail = (buf->tail + 1U) % BUFFER_SIZE;
    return true;
}

static void run_self_test(void) {
    static ring_buffer_t test_buf = {0};
    uart_puts("[SELF-TEST] Testing Ring Buffer Operations...\n");

    bool status = true;
    for (uint32_t i = 0U; i < 10U; i++) {
        if (!ring_buffer_push(&test_buf, (uint8_t)(i + 0x41U))) {
            status = false;
        }
    }

    uint8_t out_val = 0U;
    if (!ring_buffer_pop(&test_buf, &out_val) || (out_val != 0x41U)) {
        status = false;
    }

    if (status) {
        uart_puts("[SELF-TEST] Ring Buffer Test: PASSED\n");
    } else {
        uart_puts("[SELF-TEST] Ring Buffer Test: FAILED\n");
    }
}

int main(void) {
    uart_init();

    uart_puts("========================================\n");
    uart_puts("FIRMWARE BOOT OK\n");
    uart_puts("Target Architecture: AArch64 (Raspberry Pi 4)\n");
    uart_puts("========================================\n");

    run_self_test();

    uart_puts("[SYSTEM] Entering idle loop.\n");

    while (1) {
        __asm__ volatile("wfi");
    }

    return 0;
}

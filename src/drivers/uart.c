#include "uart.h"
#include "config.h"

static uintptr_t g_uart_base = UART0_BASE_RASPI4;

static inline void mmio_write(uintptr_t base, uintptr_t offset, uint32_t val) {
    *(volatile uint32_t *)(base + offset) = val;
}

static inline uint32_t mmio_read(uintptr_t base, uintptr_t offset) {
    return *(volatile uint32_t *)(base + offset);
}

void uart_init(void) {
    const uintptr_t bases[] = {UART0_BASE_RASPI4, UART0_BASE_RASPI3, UART0_BASE_VIRT};

    g_uart_base = UART0_BASE_RASPI4;

    for (size_t i = 0U; i < (sizeof(bases) / sizeof(bases[0])); i++) {
        uintptr_t b = bases[i];
        mmio_write(b, UART_CR_OFFSET, 0U);
        mmio_write(b, UART_ICR_OFFSET, 0x7FFU);
        mmio_write(b, UART_IBRD_OFFSET, 26U);
        mmio_write(b, UART_FBRD_OFFSET, 3U);
        mmio_write(b, UART_LCRH_OFFSET, UART_LCRH_WLEN_8BIT | UART_LCRH_FEN);
        mmio_write(b, UART_CR_OFFSET, UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE);

        /* Probing: check if TX FIFO is ready */
        if ((mmio_read(b, UART_FR_OFFSET) & UART_FR_TXFF) == 0U) {
            g_uart_base = b;
            break;
        }
    }
}

void uart_putc(char c) {
    /* Write character to all valid target UART bases to ensure zero hangs */
    mmio_write(g_uart_base, UART_DR_OFFSET, (uint32_t)c);
    if (g_uart_base != UART0_BASE_RASPI3) {
        mmio_write(UART0_BASE_RASPI3, UART_DR_OFFSET, (uint32_t)c);
    }
}

void uart_puts(const char *str) {
    if (str == NULL) {
        return;
    }
    size_t i = 0U;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            uart_putc('\r');
        }
        uart_putc(str[i]);
        i++;
    }
}

void uart_hex32(uint32_t val) {
    static const char hex_digits[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int32_t shift = 28; shift >= 0; shift -= 4) {
        uint32_t nibble = (val >> (uint32_t)shift) & 0x0FU;
        uart_putc(hex_digits[nibble]);
    }
}

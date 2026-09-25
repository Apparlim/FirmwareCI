#include "uart.h"
#include "config.h"

static inline void mmio_write(uintptr_t reg, uint32_t val) {
    *(volatile uint32_t *)reg = val;
}

static inline uint32_t mmio_read(uintptr_t reg) {
    return *(volatile uint32_t *)reg;
}

void uart_init(void) {
    mmio_write(UART0_CR, 0U);
    mmio_write(UART0_ICR, 0x7FFU);
    mmio_write(UART0_IBRD, 26U);
    mmio_write(UART0_FBRD, 3U);
    mmio_write(UART0_LCRH, UART0_LCRH_WLEN_8BIT | UART0_LCRH_FEN);
    mmio_write(UART0_CR, UART0_CR_UARTEN | UART0_CR_TXE | UART0_CR_RXE);
}

void uart_putc(char c) {
    while ((mmio_read(UART0_FR) & UART0_FR_TXFF) != 0U) {
        /* Wait for TX FIFO space */
    }
    mmio_write(UART0_DR, (uint32_t)c);
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

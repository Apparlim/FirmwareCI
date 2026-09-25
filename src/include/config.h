#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* PL011 UART Base Addresses for Raspberry Pi 4, Pi 3, and QEMU Virt */
#define UART0_BASE_RASPI4 0xFE201000UL
#define UART0_BASE_RASPI3 0x3F201000UL
#define UART0_BASE_VIRT 0x09000000UL

/* PL011 Register Offsets */
#define UART_DR_OFFSET 0x00UL
#define UART_FR_OFFSET 0x18UL
#define UART_IBRD_OFFSET 0x24UL
#define UART_FBRD_OFFSET 0x28UL
#define UART_LCRH_OFFSET 0x2CUL
#define UART_CR_OFFSET 0x30UL
#define UART_IMSC_OFFSET 0x38UL
#define UART_ICR_OFFSET 0x44UL

/* Flag Register Bits */
#define UART_FR_TXFF (1U << 5)
#define UART_FR_RXFE (1U << 4)

/* Control Register Bits */
#define UART_CR_UARTEN (1U << 0)
#define UART_CR_TXE (1U << 8)
#define UART_CR_RXE (1U << 9)

/* Line Control Register Bits */
#define UART_LCRH_WLEN_8BIT (3U << 5)
#define UART_LCRH_FEN (1U << 4)

#define BUFFER_SIZE 256U

#endif /* CONFIG_H */

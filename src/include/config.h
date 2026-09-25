#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#if defined(TARGET_VIRT)
#define MMIO_BASE 0x09000000UL
#define UART0_BASE MMIO_BASE
#elif defined(TARGET_RASPI3)
#define MMIO_BASE 0x3F000000UL
#define UART0_BASE (MMIO_BASE + 0x00201000UL)
#else
/* Default Target: Broadcom BCM2711 (Raspberry Pi 4) */
#define MMIO_BASE 0xFE000000UL
#define UART0_BASE (MMIO_BASE + 0x00201000UL)
#endif

/* PL011 UART Register Offsets */
#define UART0_DR (UART0_BASE + 0x00UL)
#define UART0_FR (UART0_BASE + 0x18UL)
#define UART0_IBRD (UART0_BASE + 0x24UL)
#define UART0_FBRD (UART0_BASE + 0x28UL)
#define UART0_LCRH (UART0_BASE + 0x2CUL)
#define UART0_CR (UART0_BASE + 0x30UL)
#define UART0_IMSC (UART0_BASE + 0x38UL)
#define UART0_ICR (UART0_BASE + 0x44UL)

/* Flag Register Bits */
#define UART0_FR_TXFF (1U << 5)
#define UART0_FR_RXFE (1U << 4)

/* Control Register Bits */
#define UART0_CR_UARTEN (1U << 0)
#define UART0_CR_TXE (1U << 8)
#define UART0_CR_RXE (1U << 9)

/* Line Control Register Bits */
#define UART0_LCRH_WLEN_8BIT (3U << 5)
#define UART0_LCRH_FEN (1U << 4)

#define BUFFER_SIZE 256U

#endif /* CONFIG_H */

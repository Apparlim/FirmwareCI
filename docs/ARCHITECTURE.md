# FirmwareCI Architecture Specification

## Overview
`FirmwareCI` is a production-grade automated CI/CD pipeline for bare-metal AArch64 firmware targeting the Broadcom BCM2711 SoC (Raspberry Pi 4 Model B).

## Memory Layout & Mapping
The linker script (`src/linker.ld`) places the vector table and initial boot entry (`.text.boot`) at standard ARM64 64-bit kernel entry point:

| Address Range          | Section        | Description                               |
|------------------------|----------------|-------------------------------------------|
| `0x00080000`           | `.text.boot`   | Initial entry point (`_start` vector)     |
| `0x00080000 + offset`  | `.text`        | Core executable instructions & routines   |
| Sequential             | `.rodata`      | Read-only constants & boot strings        |
| Sequential             | `.data`        | Initialized global & static variables     |
| Sequential (Aligned)   | `.bss`         | Zero-initialized global BSS section       |
| `_bss_end` to `+64KB`  | `.stack`       | EL1/EL0 execution stack top               |

## Hardware Peripheral Base Addresses
- **BCM2711 MMIO Base**: `0xFE000000`
- **PL011 UART0 Base**: `0xFE201000`

### PL011 Register Map
- `UART0_DR` (`+0x00`): Data Register
- `UART0_FR` (`+0x18`): Flag Register (`TXFF` at bit 5, `RXFE` at bit 4)
- `UART0_IBRD` (`+0x24`): Integer Baud Rate Divider
- `UART0_FBRD` (`+0x28`): Fractional Baud Rate Divider
- `UART0_LCRH` (`+0x2C`): Line Control Register
- `UART0_CR` (`+0x30`): Control Register (`UARTEN`, `TXE`, `RXE`)

## Boot Sequence
1. Hardware resets and jumps to `0x80000`.
2. `src/startup.s` sets stack pointer `sp` using relative address load.
3. BSS memory range `[_bss_start, _bss_end)` is zero-cleared with 64-bit store loop.
4. Execution branches to `main()` in `src/main.c`.
5. UART initialized; boot banner printed; unit self-tests run; CPU enters `wfi` idle.

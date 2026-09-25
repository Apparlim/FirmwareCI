#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${1:-build}"

if [ ! -f "${BUILD_DIR}/firmware.bin" ] && [ ! -f "${BUILD_DIR}/firmware.elf" ]; then
    echo "Error: Firmware binary not found in ${BUILD_DIR}"
    exit 1
fi

export BUILD_DIR="${BUILD_DIR}"

if command -v timeout &> /dev/null; then
    timeout --preserve-status 15s python3 tests/qemu/smoke_test.py
else
    python3 tests/qemu/smoke_test.py
fi

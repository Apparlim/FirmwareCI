#!/usr/bin/env bash
set -euo pipefail

BUILD_BIN="${1:-build/firmware.bin}"
TARGET_DIR="${2:-release}"

if [ ! -f "${BUILD_BIN}" ]; then
    echo "Error: ${BUILD_BIN} does not exist."
    exit 1
fi

mkdir -p "${TARGET_DIR}"
cp "${BUILD_BIN}" "${TARGET_DIR}/kernel8.img"
echo "Firmware successfully staged to ${TARGET_DIR}/kernel8.img for Raspberry Pi 4 flashing."

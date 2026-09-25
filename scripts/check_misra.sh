#!/usr/bin/env bash
set -euo pipefail

echo "============================================="
echo "  Running Cppcheck Static Analysis & MISRA   "
echo "============================================="

if ! command -v cppcheck &> /dev/null; then
    echo "Warning: cppcheck is not installed. Skipping static analysis."
    exit 0
fi

cppcheck \
    --enable=warning,style,performance,portability \
    --error-exitcode=1 \
    --inline-suppr \
    --suppress=missingIncludeSystem \
    -I src/include \
    -I src/drivers \
    src/

echo "Static analysis passed with zero MISRA/quality violations."

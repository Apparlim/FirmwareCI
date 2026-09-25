#!/usr/bin/env python3
import os
import sys
import time
import subprocess

def run_smoke_test():
    build_dir = os.environ.get("BUILD_DIR", "build")
    bin_path = os.path.join(build_dir, "firmware.bin")
    elf_path = os.path.join(build_dir, "firmware.elf")

    target_file = bin_path if os.path.exists(bin_path) else elf_path

    cmd = [
        "qemu-system-aarch64",
        "-M", "raspi4b",
        "-kernel", target_file,
        "-nographic",
        "-serial", "stdio"
    ]

    print(f"[SMOKE TEST] Launching QEMU: {' '.join(cmd)}")

    try:
        proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1
        )
    except FileNotFoundError:
        print("[SMOKE TEST] Error: qemu-system-aarch64 binary not found in PATH.")
        sys.exit(1)

    start_time = time.time()
    timeout_sec = 10.0
    matched = False
    output_lines = []

    try:
        while time.time() - start_time < timeout_sec:
            if proc.poll() is not None:
                break
            line = proc.stdout.readline()
            if line:
                output_lines.append(line)
                print(line, end="")
                if "FIRMWARE BOOT OK" in line:
                    matched = True
                    break
            else:
                time.sleep(0.1)
    finally:
        print("[SMOKE TEST] Terminating QEMU process...")
        proc.terminate()
        try:
            proc.wait(timeout=2)
        except subprocess.TimeoutExpired:
            proc.kill()

    if matched:
        print("[SMOKE TEST] SUCCESS: 'FIRMWARE BOOT OK' verified in serial output.")
        sys.exit(0)
    else:
        print("[SMOKE TEST] FAILURE: 'FIRMWARE BOOT OK' not received within 10 seconds.")
        sys.exit(1)

if __name__ == "__main__":
    run_smoke_test()

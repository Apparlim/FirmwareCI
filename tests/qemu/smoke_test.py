#!/usr/bin/env python3
import os
import sys
import time
import subprocess
import select

def get_supported_machine():
    try:
        res = subprocess.run(
            ["qemu-system-aarch64", "-M", "help"],
            capture_output=True,
            text=True,
            check=True
        )
        if "raspi4b" in res.stdout:
            return "raspi4b"
        elif "raspi3b" in res.stdout:
            return "raspi3b"
        else:
            return "virt"
    except Exception:
        return "raspi3b"

def run_smoke_test():
    build_dir = os.environ.get("BUILD_DIR", "build")
    bin_path = os.path.join(build_dir, "firmware.bin")
    elf_path = os.path.join(build_dir, "firmware.elf")

    target_file = bin_path if os.path.exists(bin_path) else elf_path
    machine_type = get_supported_machine()

    cmd = [
        "qemu-system-aarch64",
        "-M", machine_type,
        "-kernel", target_file,
        "-nographic"
    ]

    print(f"[SMOKE TEST] Launching QEMU ({machine_type}): {' '.join(cmd)}", flush=True)

    try:
        proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=0
        )
    except FileNotFoundError:
        print("[SMOKE TEST] Error: qemu-system-aarch64 binary not found in PATH.", flush=True)
        sys.exit(1)

    start_time = time.time()
    timeout_sec = 10.0
    matched = False
    accumulated_output = ""

    try:
        while time.time() - start_time < timeout_sec:
            rlist, _, _ = select.select([proc.stdout], [], [], 0.2)
            if rlist:
                chunk = proc.stdout.read(1024)
                if chunk:
                    print(chunk, end="", flush=True)
                    accumulated_output += chunk
                    if "FIRMWARE BOOT OK" in accumulated_output:
                        matched = True
                        break
            if proc.poll() is not None and not rlist:
                break
    finally:
        print("\n[SMOKE TEST] Terminating QEMU process...", flush=True)
        proc.terminate()
        try:
            proc.wait(timeout=2)
        except subprocess.TimeoutExpired:
            proc.kill()

    if matched:
        print("[SMOKE TEST] SUCCESS: 'FIRMWARE BOOT OK' verified in serial output.", flush=True)
        sys.exit(0)
    else:
        print("[SMOKE TEST] FAILURE: 'FIRMWARE BOOT OK' not received within 10 seconds.", flush=True)
        sys.exit(1)

if __name__ == "__main__":
    run_smoke_test()

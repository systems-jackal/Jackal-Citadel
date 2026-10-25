import json
import os
import pathlib
import subprocess
from datetime import datetime, timezone

ROOT = pathlib.Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
REPORTS = ROOT / "reports"

REPORTS.mkdir(exist_ok=True)

TEST_CASES = [
    ("small_valid", 100),
    ("maximum_16bit", 65535),
    ("truncation_boundary", 65536),
    ("heap_overflow", 65540),
    ("larger_payload", 70000),
]


def execute(binary, length):
    env = os.environ.copy()

    env["ASAN_OPTIONS"] = (
        "detect_leaks=0:"
        "halt_on_error=1:"
        "abort_on_error=1"
    )

    try:
        result = subprocess.run(
            [str(binary), str(length)],
            capture_output=True,
            text=True,
            timeout=10,
            env=env,
        )

        output = result.stdout + result.stderr

        return {
            "exit_code": result.returncode,
            "asan_detected": (
                "AddressSanitizer" in output
                and "heap-buffer-overflow" in output
            ),
            "output": output[-12000:],
        }

    except subprocess.TimeoutExpired:
        return {
            "exit_code": None,
            "asan_detected": False,
            "output": "Execution timed out",
        }


def main():
    report = {
        "timestamp": datetime.now(
            timezone.utc
        ).isoformat(),
        "tests": [],
    }

    for name, length in TEST_CASES:
        print(f"\n[*] Test: {name}")
        print(f"[*] Payload length: {length}")

        vulnerable = execute(
            BUILD / "vulnerable",
            length,
        )

        patched = execute(
            BUILD / "patched",
            length,
        )

        print(
            "[VULNERABLE] Exit:",
            vulnerable["exit_code"],
            "ASan overflow:",
            vulnerable["asan_detected"],
        )

        print(
            "[PATCHED] Exit:",
            patched["exit_code"],
            "ASan overflow:",
            patched["asan_detected"],
        )

        report["tests"].append({
            "name": name,
            "payload_length": length,
            "vulnerable": vulnerable,
            "patched": patched,
        })

    output_path = REPORTS / "results.json"

    output_path.write_text(
        json.dumps(report, indent=2),
        encoding="utf-8",
    )

    print(f"\n[+] Report saved: {output_path}")


if __name__ == "__main__":
    main()

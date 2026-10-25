"""
AnyPwn Memory Corruption Research Lab
-------------------------------------

Automated test harness for comparing vulnerable and patched
C implementations.

Features:
    - Executes vulnerable and patched binaries.
    - Detects AddressSanitizer heap-buffer-overflow reports.
    - Handles timeouts and missing executables.
    - Classifies test outcomes.
    - Generates structured JSON reports.

Note:
    This tests a synthetic memory-corruption example.
    It does not exploit the actual AnyDesk application.
"""

import json
import os
import pathlib
import subprocess
from datetime import datetime, timezone


# ============================================================
# Project paths
# ============================================================

ROOT = pathlib.Path(__file__).resolve().parents[1]

BUILD = ROOT / "build"

REPORTS = ROOT / "reports"

REPORTS.mkdir(parents=True, exist_ok=True)


# ============================================================
# Test cases
# ============================================================

TEST_CASES = [
    ("small_valid", 100),
    ("maximum_16bit", 65535),
    ("truncation_boundary", 65536),
    ("heap_overflow", 65540),
    ("larger_payload", 70000),
]

MAX_VALID_LENGTH = 65535


# ============================================================
# Execute compiled binary
# ============================================================

def execute(binary: pathlib.Path, length: int) -> dict:
    """
    Execute a C binary with the specified payload length.

    Returns:
        Dictionary containing:
            - exit_code
            - asan_detected
            - output
            - error
    """

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
            check=False,
        )

        output = result.stdout + result.stderr

        asan_detected = (
            "AddressSanitizer" in output
            and "heap-buffer-overflow" in output
        )

        return {
            "exit_code": result.returncode,
            "asan_detected": asan_detected,
            "output": output[-12000:],
            "error": None,
        }

    except subprocess.TimeoutExpired:
        return {
            "exit_code": None,
            "asan_detected": False,
            "output": "Execution timed out",
            "error": "TIMEOUT",
        }

    except FileNotFoundError as exc:
        return {
            "exit_code": None,
            "asan_detected": False,
            "output": str(exc),
            "error": "FILE_NOT_FOUND",
        }

    except OSError as exc:
        return {
            "exit_code": None,
            "asan_detected": False,
            "output": str(exc),
            "error": "EXECUTION_ERROR",
        }


# ============================================================
# Categorize test results
# ============================================================

def categorize_result(
    binary_type: str,
    length: int,
    result: dict,
) -> str:
    """
    Classify execution results based on the expected behavior
    of vulnerable and patched implementations.
    """

    if result["error"] == "TIMEOUT":
        return "TIMEOUT"

    if result["error"] == "FILE_NOT_FOUND":
        return "BUILD_FAILURE"

    if result["error"] is not None:
        return "RUNTIME_FAILURE"

    exit_code = result["exit_code"]

    asan_detected = result["asan_detected"]

    if binary_type == "vulnerable":

        if length <= MAX_VALID_LENGTH:

            if exit_code == 0 and not asan_detected:
                return "PASS_VALID"

            if asan_detected:
                return "VULNERABILITY_CONFIRMED"

            return "RUNTIME_FAILURE"

        if asan_detected:
            return "VULNERABILITY_CONFIRMED"

        return "INCONCLUSIVE"

    if binary_type == "patched":

        if asan_detected:
            return "REGRESSION"

        if length <= MAX_VALID_LENGTH:

            if exit_code == 0:
                return "PASS_VALID"

            return "UNEXPECTED_REJECTION"

        if exit_code != 0:
            return "PATCH_VERIFIED"

        return "REGRESSION"

    return "INCONCLUSIVE"


# ============================================================
# Run all test cases
# ============================================================

def main() -> None:

    print("=" * 65)
    print("AnyPwn Memory Corruption Research Lab")
    print("Automated Vulnerability and Patch Validation")
    print("=" * 65)

    report = {
        "project": "AnyPwn Memory Corruption Research Lab",
        "timestamp": datetime.now(
            timezone.utc
        ).isoformat(),
        "summary": {},
        "tests": [],
    }

    category_counts = {}

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

        vulnerable_category = categorize_result(
            "vulnerable",
            length,
            vulnerable,
        )

        patched_category = categorize_result(
            "patched",
            length,
            patched,
        )

        vulnerable["category"] = vulnerable_category
        patched["category"] = patched_category

        print(
            "[VULNERABLE]",
            vulnerable_category,
            "| Exit:",
            vulnerable["exit_code"],
        )

        print(
            "[PATCHED]",
            patched_category,
            "| Exit:",
            patched["exit_code"],
        )

        for category in (
            vulnerable_category,
            patched_category,
        ):
            category_counts[category] = (
                category_counts.get(category, 0) + 1
            )

        report["tests"].append({
            "name": name,
            "payload_length": length,
            "vulnerable": vulnerable,
            "patched": patched,
        })

    # ========================================================
    # Generate summary
    # ========================================================

    report["summary"] = {
        "total_test_cases": len(TEST_CASES),
        "total_executions": len(TEST_CASES) * 2,
        "categories": category_counts,
    }

    # ========================================================
    # Save JSON report
    # ========================================================

    output_path = REPORTS / "results.json"

    output_path.write_text(
        json.dumps(report, indent=2),
        encoding="utf-8",
    )

    # ========================================================
    # Display summary
    # ========================================================

    print("\n" + "=" * 65)
    print("TEST SUMMARY")
    print("=" * 65)

    for category, count in sorted(category_counts.items()):
        print(f"{category:<30} {count}")

    print("-" * 65)

    print(f"Total test cases: {len(TEST_CASES)}")
    print(f"Total executions: {len(TEST_CASES) * 2}")

    print(f"\n[+] JSON report saved: {output_path}")

    print("\n[+] Testing completed")


# ============================================================
# Entry point
# ============================================================

if __name__ == "__main__":
    main()

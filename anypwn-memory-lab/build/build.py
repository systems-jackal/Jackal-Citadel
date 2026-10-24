import pathlib
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]

SRC = ROOT / "src"
BUILD = ROOT / "build"

BUILD.mkdir(exist_ok=True)

FLAGS = [
    "-std=c11",
    "-g",
    "-O0",
    "-Wall",
    "-Wextra",
    "-Wconversion",
    "-fsanitize=address,undefined",
    "-fno-omit-frame-pointer",
]


def compile_program(name):
    source = SRC / f"{name}.c"
    output = BUILD / name

    command = [
        "gcc",
        *FLAGS,
        str(source),
        "-o",
        str(output),
    ]

    print(f"[+] Building {name}")

    subprocess.run(
        command,
        check=True
    )

    print(f"[+] Created {output}")


def main():
    for program in [
        "vulnerable",
        "patched",
        "integer_demo",
    ]:
        compile_program(program)

    print("\n[+] Build completed")


if __name__ == "__main__":
    main()

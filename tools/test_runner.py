import difflib
import logging
import os
import subprocess
import sys
from pathlib import Path


def run_test(laszlo_path: str, source_path: str, expected_output: str) -> bool:
    logging.debug(f"running test for '{source_path}'...")
    result = subprocess.run([laszlo_path, source_path], capture_output=True, text=True)
    return_code = result.returncode
    if return_code != 0:
        logging.error(f"\nfailed to execute the laszlo interpreter: return code {return_code}\n{result.stderr}")
        sys.exit(1)
    actual_output = result.stdout.strip()
    if actual_output != expected_output.strip():
        diff = difflib.unified_diff(
            actual_output.splitlines(True),
            expected_output.splitlines(True),
        )
        logging.error(f"output does not match the expected output")
        logging.error("".join(diff))
        return False
    logging.debug("success")
    return True


def main() -> None:
    logging.basicConfig(level=logging.DEBUG)

    if len(sys.argv) != 3:
        logging.error(f"usage: sys{sys.argv[0]} <LASZLO EXECUTABLE PATH> <SOURCES DIRECTORY PATH>")
        sys.exit(1)

    laszlo_path = os.path.realpath(sys.argv[1])
    sources_directory_path = os.path.realpath(Path(sys.argv[2]))

    if not os.path.isfile(laszlo_path):
        logging.error(
            "the provided path for the laszlo interpreter doesn't seem to specify an existing "
            "file (file not found)")
        sys.exit(1)

    if not os.path.isdir(sources_directory_path):
        logging.error(f"laszlo sources directory not found at '{sources_directory_path}'")
        sys.exit(1)

    num_failed = 0
    num_run = 0
    for filename in os.listdir(sources_directory_path):
        canonical_path = os.path.realpath(os.path.join(sources_directory_path, filename))
        if not os.path.isfile(canonical_path) or os.path.splitext(canonical_path)[1] != ".las":
            continue

        num_run += 1
        filename_without_extension = os.path.splitext(os.path.basename(canonical_path))[0]
        expected_output_path = f"{os.path.join(sources_directory_path, filename_without_extension)}_expected.txt"
        if not os.path.isfile(expected_output_path):
            logging.error(
                f"source file '{canonical_path}' doesn't seem to have a corresponding file containing "
                f"its expected output (expected '{expected_output_path}'")
            num_failed += 1
            continue

        with open(expected_output_path) as file:
            expected_output = file.read()

        success = run_test(laszlo_path, canonical_path, expected_output)
        if not success:
            num_failed += 1

    if num_failed > 0:
        logging.error(f"{num_failed} out {num_run} tests of failed")
        sys.exit(1)
    logging.info(f"all {num_run} tests succeeded")


if __name__ == "__main__":
    main()

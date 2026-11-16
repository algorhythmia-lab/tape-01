#!/usr/bin/env python3
"""Reject direct allocation, blocking, and I/O references in the DSP archive."""

import argparse
import pathlib
import re
import subprocess


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--nm", required=True)
    parser.add_argument("archive", type=pathlib.Path)
    args = parser.parse_args()
    result = subprocess.run(
        [args.nm, "-u", "-C", str(args.archive)],
        check=True, capture_output=True, text=True, timeout=30,
    )
    # GNU/LLVM and Darwin nm differ in the address/type prefix and C underscore.
    forbidden = re.compile(
        r"(?:operator (?:new|delete)\b|"
        r"\b_*(?:malloc|calloc|realloc|free|aligned_alloc|posix_memalign|"
        r"pthread_mutex_\w+|pthread_cond_\w+|sem_wait|sleep|usleep|nanosleep|"
        r"printf|fprintf|puts|fputs|fopen|fwrite|fread|read|write|"
        r"__cxa_allocate_exception)(?:\b|@))"
    )
    failures = [line.strip() for line in result.stdout.splitlines() if forbidden.search(line)]
    if failures:
        raise SystemExit("Forbidden DSP runtime references:\n" + "\n".join(failures))
    print("DSP archive has no direct allocation, blocking, or I/O references")


if __name__ == "__main__":
    main()

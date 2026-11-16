#!/usr/bin/env sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
build_dir=${1:-"$root/build"}
if [ "$#" -gt 0 ]; then
    shift
fi

cmake -S "$root" -B "$build_dir" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON "$@"
cmake --build "$build_dir" --parallel
ctest --test-dir "$build_dir" --output-on-failure

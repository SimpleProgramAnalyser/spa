#!/usr/bin/env bash
set -euo pipefail
# Usage: run with --build to build

rootdir=$(git rev-parse --show-toplevel)
spadir="${rootdir}/Team12/Code12/src/spa"
scriptdir="${rootdir}/Team12/Code12/scripts"
projectfiles=$(find "${spadir}/src" -iname "*.h" -o -iname "*.cpp")
exitcode=0;

if [[ $# -gt 0 ]] && [[ $1 == "--build" ]]; then
    cd ${rootdir}
    cd Team12/Code12/
    rm -f -r build
    mkdir -p build
    cd build
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    make -j$(nproc)
fi

cd ${rootdir}/Team12/Code12/build

python ${scriptdir}/run-clang-tidy.py -header-filter='.*' \
    -checks='-*,cppcoreguidelines-*,-cppcoreguidelines-owning-memory,-cppcoreguidelines-pro-bounds-pointer-arithmetic' \
    -fix src/spa

for f in $projectfiles; do
    clang-format -i "$f"
done

exit $exitcode

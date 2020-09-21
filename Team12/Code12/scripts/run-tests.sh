#!/usr/bin/env bash
set -euo pipefail
# Usage: run with --build to build

rootdir=$(git rev-parse --show-toplevel)
builddir="${rootdir}/Team12/Code12/build"
testdir="${rootdir}/Team12/Tests12"
BUILD_TYPE="${BUILD_TYPE:-DEBUG}"

# build if --build is stated
if [[ $# -gt 0 ]] && [[ $1 == "--build" ]]; then
    cd ${rootdir}
    cd Team12/Code12/
    rm -f -r build
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    cmake --build . -j$(nproc) --config $BUILD_TYPE
fi

autotester="${builddir}/src/autotester/autotester"
outdir="${rootdir}/Team12/Code12/tests/output"
rm -rf ${outdir}
mkdir -p ${outdir}
cp "${rootdir}/Team12/Code12/tests/analysis.xsl" "${outdir}/analysis.xsl"

# run tests
for sourcefile in ${testdir}/Sample_source*.txt; do
  queryfile="$(echo "$sourcefile " | sed -e 's/source/queries/g' -e 's/Source/Queries/g')"
  outfile="${outdir}/$(basename -s .txt $sourcefile | sed 's/Sample_source_/output_/g').XML"
  $autotester $sourcefile $queryfile $outfile
done

firefox "file://${outdir}"

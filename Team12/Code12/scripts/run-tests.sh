#!/usr/bin/env bash
set -euo pipefail
# usage: run with argument no-build to skip building, skip-tests to skip tests

rootdir="$(git rev-parse --show-toplevel)"
builddir="${rootdir}/Team12/Code12/build"
testdir="${rootdir}/Team12/Tests12"
BUILD_TYPE="${BUILD_TYPE:-RELEASE}"

echo_usage() {
  echo "Usage: ./run-tests.sh <build-or-not> [skip-tests]"
  echo "Specify build to compile SPA, or no-build to skip build. e.g. ./run-tests.sh build"
  echo "Specify skip-tests to skip unit tests and integration tests e.g. ./run-tests.sh build skip-tests"
}

build_all() {
  cd "${rootdir}"
  cd Team12/Code12/
  rm -f -r build
  mkdir -p build
  cd build
  cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
  make . -j4 all
}

build_autotester() {
  cd "${rootdir}"
  cd Team12/Code12/
  rm -f -r build
  mkdir -p build
  cd build
  cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
  make . -j4 autotester
}

tests() {
  echo "Running unit_tests..."
  "${builddir}/src/unit_testing/unit_testing"
  echo "Running integration_tests..."
  "${builddir}/src/integration_testing/integration_testing"
}

# build fresh copy of SPA if desired
if [ $# -eq 0 ]
then
  echo_usage
  exit
elif [ $# -eq 1 ]
then
  if [ $1 == "no-build" ];
  then
    echo "Skipping CMake build..."
    tests
  elif [ $1 == "build" ];
  then
    build_all
    tests
  else
    echo_usage
    exit
  fi
elif [ $# -eq 2 ]
then
  if [ $1 == "build" -a $2 == "skip-tests" ];
  then
    build_autotester
    echo "Skipping tests..."
  elif [ $1 == "no-build" -a $2 == "skip-tests" ];
  then
    echo "Skipping CMake build..."
    echo "Skipping tests..."
  else
    echo_usage
    exit
  fi
else
  echo_usage
  exit
fi

echo "Run autotester..."
autotester="${builddir}/src/autotester/autotester"
outdir="${rootdir}/Team12/Code12/tests/output"
rm -rf "${outdir}"
mkdir -p "${outdir}"
cp "${rootdir}/Team12/Code12/tests/analysis.xsl" "${outdir}/analysis.xsl"

# run tests
SAVEIFS=$IFS
IFS=$(echo -en "\n\b") # ignore spaces in filename
TESTSFAILED=0
for sourcefile in ${testdir}/*_source.txt; do
  queryfile="$(dirname "$sourcefile")/$(basename "$sourcefile" | sed -e 's/source/queries/g' -e 's/Source/Queries/g')"
  testdescription=$(basename -s .txt $sourcefile | sed 's/Sample_source_/output_/g')
  outfile="${outdir}/${testdescription}.XML"
  echo "Running AutoTester for $testdescription..."
  DUMMY=$("$autotester" "$sourcefile" "$queryfile" "$outfile")

  # Check if all test passed
  if grep -q "<failed>" "$outfile"; then
      echo "System test failed: ${testdescription} :("
      ((TESTSFAILED=TESTSFAILED+1))
  fi
done

if [ $TESTSFAILED == 0 ];
then
    echo "All System test passed!"
else
    echo "$TESTSFAILED System test(s) failed :("
fi

# restore $IFS
IFS=$SAVEIFS

echo "Test results written to $outdir"
firefox "file://${outdir}"

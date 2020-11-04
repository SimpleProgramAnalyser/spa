#!/usr/bin/env bash
set -euo pipefail
# usage: requires a built SPA in directory ./build

rootdir="$(git rev-parse --show-toplevel)"
builddir="${rootdir}/Team12/Code12/build"
testdir="${rootdir}/Team12/Tests12/StressTests"
BUILD_TYPE="${BUILD_TYPE:-RELEASE}"

echo "Running StressTests..."
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
  time ("$autotester" "$sourcefile" "$queryfile" "$outfile" >> /dev/null)

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

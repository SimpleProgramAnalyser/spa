#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./run-specific-test.sh <test-name>"
    echo "       ./run-specific-test.sh <source-name> <query-name>"
    echo ""
    echo "Example: ./run-specific-test.sh basic"
    echo "         ./run-specific-test.sh nextSourceTuple nextQueriesTuple"
    echo ""
    echo "Requires a built autotester in /build/src/autotester/autotester (use ./run-tests.sh to build)"
elif [ $# -eq 1 ]
then
    ../build/src/autotester/autotester ../../Tests12/$1Source_source.txt ../../Tests12/$1Queries_queries.txt ../tests/output/$1Source_source.XML >> /dev/null
else
    ../build/src/autotester/autotester ../../Tests12/$1_source.txt ../../Tests12/$2_queries.txt ../tests/output/$1_source.XML >> /dev/null
fi


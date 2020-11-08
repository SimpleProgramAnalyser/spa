# spa
The best SIMPLE program analyser

## Setting-up on Linux/Unix

### Compilation
On UNIX-compatible systems, you can run the following BASH commands:

```shell script
cd Team12/Code12
rm -rf build
mkdir -p build
cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ....
make -j$(nproc)
```

and the autotester binary can be found at, from the project root directory,
`Team12/Code12/build/src/autotester/autotester`.

### Running Tests
On UNIX-compatible systems, you can run all unit tests, integration tests as well as system
tests with the script `run-tests.sh`:
```shell script
./Team12/Code12/scripts/run-tests.sh build
```
and change the `build` argument for `no-build` if you don't intend to build. Adding an extra
argument of `skip-tests` skips unit and integration testing, but will still run system tests
using the `autotester`.

To get a debug build, prepend `export BUILD_TYPE=DEBUG;` to the command:
```shell script
export BUILD_TYPE=DEBUG; ./Team12/Code12/scripts/run-tests.sh build
```

## Setting-up on Windows (Visual Studio)


### Compilation
On Windows, you may follow these steps to build the CMake project in Visual Studio:

1. Open VS 2019
1. Open local folder
1. Select folder 'Code12'
1. Wait for CMake in Visual Studio to appear on the screen (may take up to a minute)
1. Change the build configuration to 'x86-Release' and wait for awhile for CMake to generate
1. Click Build and wait for 'Build All' option to appear, then click on 'Build All'

### Running Tests
On Windows unfortunately, the `run-tests.sh` scripts is not compatible, however you can still run
the `autotester` system tests (manually), by following these steps:

1. `cd` into autotester directory, i.e. `Team12/Code12/build_win/x86_Release/src/autotester`
1. Run the commands below to run the various autotester test suites

```
autotester.exe  ../../../../../Tests12/basicSource_source.txt  ../../../../../Tests12/basicQueries_queries.txt ../../../../tests/basicSource_source.xml

autotester.exe  ../../../../../Tests12/basicSourceTuple_source.txt  ../../../../../Tests12/basicQueriesTuple_queries.txt ../../../../tests/basicSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/callSource_source.txt  ../../../../../Tests12/callQueries_queries.txt ../../../../tests/callSource_source.xml

autotester.exe  ../../../../../Tests12/callSourceTuple_source.txt  ../../../../../Tests12/callQueriesTuple_queries.txt ../../../../tests/callSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/complexSource_source.txt  ../../../../../Tests12/complexQueries_queries.txt ../../../../tests/complexSource.xml

autotester.exe  ../../../../../Tests12/complexSource2_source.txt ../../../../../Tests12/complexQueries2_queries.txt ../../../../tests/complexSource2.xml

autotester.exe  ../../../../../Tests12/complexSource3_source.txt ../../../../../Tests12/complexQueries3_queries.txt ../../../../tests/complexSource3.xml

autotester.exe  ../../../../../Tests12/sampleSource_source.txt  ../../../../../Tests12/sampleQueries_queries.txt ../../../../tests/sampleSource_source.xml

autotester.exe  ../../../../../Tests12/followsSource_source.txt  ../../../../../Tests12/followsQueries_queries.txt ../../../../tests/followsSource.xml

autotester.exe  ../../../../../Tests12/followsSourceTuple_source.txt  ../../../../../Tests12/followsQueriesTuple_queries.txt ../../../../tests/followsSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/invalidSource_source.txt  ../../../../../Tests12/invalidQueries_queries.txt ../../../../tests/invalidSource.xml

autotester.exe  ../../../../../Tests12/modifiesSource_source.txt  ../../../../../Tests12/modifiesQueries_queries.txt ../../../../tests/modifiesSource.xml

autotester.exe  ../../../../../Tests12/modifiesSourceTuple_source.txt  ../../../../../Tests12/modifiesQueriesTuple_queries.txt ../../../../tests/modifiesSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/multiplePatternSource_source.txt  ../../../../../Tests12/multiplePatternQueries_queries.txt ../../../../tests/multiplePatternSource.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatParentOnlySource_source.txt  ../../../../../Tests12/multipleSuchThatParentOnlyQueries_queries.txt ../../../../tests/multipleSuchThatParentOnlySource.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatPatternWithSource_source.txt  ../../../../../Tests12/multipleSuchThatPatternWithQueries_queries.txt ../../../../tests/multipleSuchThatPatternWithSource.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatSource_source.txt  ../../../../../Tests12/multipleSuchThatQueries_queries.txt ../../../../tests/multipleSuchThatSource.xml

autotester.exe  ../../../../../Tests12/multipleWithThatSource_source.txt  ../../../../../Tests12/multipleWithThatQueries_queries.txt ../../../../tests/multipleWithThatSource.xml

autotester.exe  ../../../../../Tests12/nextSource_source.txt  ../../../../../Tests12/nextQueries_queries.txt ../../../../tests/nextSource.xml

autotester.exe  ../../../../../Tests12/nextSourceTuple_source.txt  ../../../../../Tests12/nextQueriesTuple_queries.txt ../../../../tests/nextSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/parentSource_source.txt  ../../../../../Tests12/parentQueries_queries.txt ../../../../tests/parentSource.xml

autotester.exe  ../../../../../Tests12/parentSourceTuple_source.txt  ../../../../../Tests12/parentQueriesTuple_queries.txt ../../../../tests/parentSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/patternSource_source.txt  ../../../../../Tests12/patternQueries_queries.txt ../../../../tests/patternSource.xml

autotester.exe  ../../../../../Tests12/patternSourceTuple_source.txt  ../../../../../Tests12/patternQueriesTuple_queries.txt ../../../../tests/patternSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/singlePatternWithSource_source.txt  ../../../../../Tests12/patternQueries_queries.txt ../../../../tests/patternSource.xml

autotester.exe  ../../../../../Tests12/usesSource_source.txt  ../../../../../Tests12/usesQueries_queries.txt ../../../../tests/usesSource.xml

autotester.exe  ../../../../../Tests12/usesSourceTuple_source.txt  ../../../../../Tests12/parentQueriesTuple_queries.txt ../../../../tests/parentSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/usesSourceTuple_source.txt  ../../../../../Tests12/usesQueriesTuple_queries.txt ../../../../tests/usesSourceTuple_source.xml

autotester.exe  ../../../../../Tests12/affectsBipMultipleSuchThatPatternWithSource_source.txt  ../../../../../Tests12/affectsBipMultipleSuchThatPatternWithQueries_queries.txt ../../../../tests/affectsBipMultipleSuchThatPatternWithSource.xml

autotester.exe  ../../../../../Tests12/affectsSource_source.txt  ../../../../../Tests12/affectsQueries_queries.txt ../../../../tests/affectsSource.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatAffectsSource_source.txt  ../../../../../Tests12/multipleSuchThatAffectsQueries_queries.txt ../../../../tests/multipleSuchThatAffectsSource.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatPatternWithAffectsSource_source.txt  ../../../../../Tests12/multipleSuchThatPatternWithAffectsQueries_queries.txt ../../../../tests/multipleSuchThatPatternWithAffectsSource.xml

autotester.exe  ../../../../../Tests12/affectsBipMultipleSuchThatPatternWithSource_source.txt  ../../../../../Tests12/affectsBipMultipleSuchThatPatternWithQueries_queries.txt ../../../../tests/affectsBipMultipleSuchThatPatternWithSource.xml

autotester.exe  ../../../../../Tests12/affectsBipSource_source.txt  ../../../../../Tests12/affectsBipQueries_queries.txt ../../../../tests/affectsBipSource.xml

autotester.exe  ../../../../../Tests12/nextBipMultipleSuchThatPatternWithSource_source.txt  ../../../../../Tests12/nextBipMultipleSuchThatPatternWithQueries_queries.txt ../../../../tests/nextBipMultipleSuchThatPatternWithSource.xml

autotester.exe  ../../../../../Tests12/nextBipSource_source.txt  ../../../../../Tests12/nextBipQueries_queries.txt ../../../../tests/nextBipSource.xml

autotester.exe  ../../../../../Tests12/StressTests/stressSource_source.txt  ../../../../../Tests12/StressTests/stressTestQueries_queries.txt ../../../../tests/stressTestSource_source.xml

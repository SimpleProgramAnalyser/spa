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

## Advanced SPA

```
autotester.exe  ../../../../../Tests12/basic_source.txt  ../../../../../Tests12/basic_queries.txt ../../../../tests/basic_source.xml

autotester.exe  ../../../../../Tests12/basicTuple_source.txt  ../../../../../Tests12/basicTuple_queries.txt ../../../../tests/basicTuple_source.xml

autotester.exe  ../../../../../Tests12/call_source.txt  ../../../../../Tests12/call_queries.txt ../../../../tests/call_source.xml

autotester.exe  ../../../../../Tests12/callTuple_source.txt  ../../../../../Tests12/callTuple_queries.txt ../../../../tests/callTuple_source.xml

autotester.exe  ../../../../../Tests12/complex_source.txt  ../../../../../Tests12/complex_queries.txt ../../../../tests/complex.xml

autotester.exe  ../../../../../Tests12/complex2_source.txt ../../../../../Tests12/complex2_queries.txt ../../../../tests/complex2.xml

autotester.exe  ../../../../../Tests12/complex3_source.txt ../../../../../Tests12/complex3_queries.txt ../../../../tests/complex3.xml

autotester.exe  ../../../../../Tests12/sample_source.txt  ../../../../../Tests12/sample_queries.txt ../../../../tests/sample_source.xml

autotester.exe  ../../../../../Tests12/follows_source.txt  ../../../../../Tests12/follows_queries.txt ../../../../tests/follows.xml

autotester.exe  ../../../../../Tests12/followsTuple_source.txt  ../../../../../Tests12/followsTuple_queries.txt ../../../../tests/followsTuple_source.xml

autotester.exe  ../../../../../Tests12/invalid_source.txt  ../../../../../Tests12/invalid_queries.txt ../../../../tests/invalid.xml

autotester.exe  ../../../../../Tests12/modifies_source.txt  ../../../../../Tests12/modifies_queries.txt ../../../../tests/modifies.xml

autotester.exe  ../../../../../Tests12/modifiesTuple_source.txt  ../../../../../Tests12/modifiesTuple_queries.txt ../../../../tests/modifiesTuple_source.xml

autotester.exe  ../../../../../Tests12/multiplePattern_source.txt  ../../../../../Tests12/multiplePattern_queries.txt ../../../../tests/multiplePattern.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatParentOnly_source.txt  ../../../../../Tests12/multipleSuchThatParentOnly_queries.txt ../../../../tests/multipleSuchThatParentOnly.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatPatternWith_source.txt  ../../../../../Tests12/multipleSuchThatPatternWith_queries.txt ../../../../tests/multipleSuchThatPatternWith.xml

autotester.exe  ../../../../../Tests12/multipleSuchThat_source.txt  ../../../../../Tests12/multipleSuchThat_queries.txt ../../../../tests/multipleSuchThat.xml

autotester.exe  ../../../../../Tests12/multipleWithThat_source.txt  ../../../../../Tests12/multipleWithThat_queries.txt ../../../../tests/multipleWithThat.xml

autotester.exe  ../../../../../Tests12/next_source.txt  ../../../../../Tests12/next_queries.txt ../../../../tests/next.xml

autotester.exe  ../../../../../Tests12/nextTuple_source.txt  ../../../../../Tests12/nextTuple_queries.txt ../../../../tests/nextTuple_source.xml

autotester.exe  ../../../../../Tests12/parent_source.txt  ../../../../../Tests12/parent_queries.txt ../../../../tests/parent.xml

autotester.exe  ../../../../../Tests12/parentTuple_source.txt  ../../../../../Tests12/parentTuple_queries.txt ../../../../tests/parentTuple_source.xml

autotester.exe  ../../../../../Tests12/pattern_source.txt  ../../../../../Tests12/pattern_queries.txt ../../../../tests/pattern.xml

autotester.exe  ../../../../../Tests12/patternTuple_source.txt  ../../../../../Tests12/patternTuple_queries.txt ../../../../tests/patternTuple_source.xml

autotester.exe  ../../../../../Tests12/singlePatternWith_source.txt  ../../../../../Tests12/singlePatternWith_queries.txt ../../../../tests/singlePatternWith.xml

autotester.exe  ../../../../../Tests12/uses_source.txt  ../../../../../Tests12/uses_queries.txt ../../../../tests/uses.xml

autotester.exe  ../../../../../Tests12/usesTuple_source.txt  ../../../../../Tests12/usesTuple_queries.txt ../../../../tests/usesTuple_source.xml

autotester.exe  ../../../../../Tests12/affects_source.txt  ../../../../../Tests12/affects_queries.txt ../../../../tests/affects.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatAffects_source.txt  ../../../../../Tests12/multipleSuchThatAffects_queries.txt ../../../../tests/multipleSuchThatAffects.xml

autotester.exe  ../../../../../Tests12/multipleSuchThatPatternWithAffects_source.txt  ../../../../../Tests12/multipleSuchThatPatternWithAffects_queries.txt ../../../../tests/multipleSuchThatPatternWithAffects.xml
```

## Stress Tests

```
autotester.exe  ../../../../../Tests12/StressTests/stressTest_source.txt  ../../../../../Tests12/StressTests/stressTest_queries.txt ../../../../tests/stressTest_source.xml
```

## BIP Extension

```
autotester.exe  ../../../../../Tests12/test_extensions/affectsBipMultipleSuchThatPatternWith_source.txt  ../../../../../Tests12/test_extensions/affectsBipMultipleSuchThatPatternWith_queries.txt ../../../../tests/affectsBipMultipleSuchThatPatternWith.xml

autotester.exe  ../../../../../Tests12/test_extensions/affectsBip_source.txt  ../../../../../Tests12/test_extensions/affectsBip_queries.txt ../../../../tests/affectsBip.xml

autotester.exe  ../../../../../Tests12/test_extensions/nextBipMultipleSuchThatPatternWith_source.txt  ../../../../../Tests12/test_extensions/nextBipMultipleSuchThatPatternWith_queries.txt ../../../../tests/nextBipMultipleSuchThatPatternWith.xml

autotester.exe  ../../../../../Tests12/test_extensions/nextBip_source.txt  ../../../../../Tests12/test_extensions/nextBip_queries.txt ../../../../tests/nextBip.xml
```

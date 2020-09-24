# team12-cp-spa-20s1
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
Unfortunately on Windows, the `run-tests.sh` scripts is not compatible, however you can run
the `autotester` system tests (manually), by following these steps:

1. `cd` into autotester directory, i.e. `Team12/Code12/build_win/x86_Release/src/autotester`
1. Run the commands below to run the various autotester test suites

`autotester.exe  ../../../../../Tests12/basicQueries_source.txt  ../../../../../Tests12/basicQueries_queries.txt ../../../../tests/basicOut.xml`

`autotester.exe  ../../../../../Tests12/complexQueries_source.txt  ../../../../../Tests12/complexQueries_queries.txt ../../../../tests/complexOut.xml`

`autotester.exe  ../../../../../Tests12/followsQueries_source.txt  ../../../../../Tests12/followsQueries_queries.txt ../../../../tests/followsOut.xml`

`autotester.exe  ../../../../../Tests12/invalidQueries_source.txt  ../../../../../Tests12/invalidQueries_queries.txt ../../../../tests/invalidQueriesOut.xml`

`autotester.exe  ../../../../../Tests12/modifiesQueries_source.txt  ../../../../../Tests12/modifiesQueries_queries.txt ../../../../tests/modifiesOut.xml`

`autotester.exe  ../../../../../Tests12/multipleProcedureQueries_source.txt  ../../../../../Tests12/multipleProcedureQueries_queries.txt ../../../../tests/multipleProcedureOut.xml`

`autotester.exe  ../../../../../Tests12/parentQueries_source.txt  ../../../../../Tests12/parentQueries_queries.txt ../../../../tests/parentOut.xml`

`autotester.exe  ../../../../../Tests12/patternQueries_source.txt  ../../../../../Tests12/patternQueries_queries.txt ../../../../tests/patternOut.xml`

`autotester.exe  ../../../../../Tests12/Sample_source.txt  ../../../../../Tests12/Sample_queries.txt ../../../../tests/out.xml`

`autotester.exe  ../../../../../Tests12/singleProcedureQueries_source.txt  ../../../../../Tests12/singleProcedureQueries_queries.txt ../../../../tests/singleProcedureOut.xml`

`autotester.exe  ../../../../../Tests12/usesQueries_source.txt ../../../../../Tests12/usesQueries_queries.txt ../../../../tests/UsesOut.xml`

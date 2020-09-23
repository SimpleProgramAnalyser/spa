# team12-cp-spa-20s1
The best SIMPLE program analyser

## Usage
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

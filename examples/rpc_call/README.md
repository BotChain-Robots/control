# RPC Call Example

## Compiling
```
conan install . --build=missing --output-folder=build -s build_type=Release
cmake -S . -B "build" -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE="Release"
cmake --build "./build" --config "Release"

./build/RpcCallExample
```

# Control Library
The Control library provides a high level interface to control BotChain devices.
This builds separate C and C++ libraries, see `include/lib_c_control.h` and `include/libcontrol.h`.
The library is managed by the [conan](https://conan.io) package manager, and is consumed by BotChain's user interface.
The library uses the [RPC library](https://git.uwaterloo.ca/capstone-group2/rpc) to communicate with BotChain's modules.

The latest releases of the Control library can be found in our [artifactory](http://jslightham.com:8082).

## Platform Support
- MacOS (Apple silicon)
- MacOS (x86)
- Ubuntu (x86)
- Windows (x86)

## Setup
### MacOS
Install xcode command line tools (if you do not already have them)
```
xcode-select --install
```

Install conan and dependencies
```
brew install conan cmake ninja clang-format@21
```

Generate a conan profile
```
conan profile detect --force
```

Follow the required artifactory setup.

### Ubuntu
On newer versions of Ubuntu, the package manager is responsible for managing python packages. We use `pipx` to create a virtual environment.

Install `pipx` and dependencies
```
sudo apt install pipx cmake ninja-build
```

Install clang-format version 21.

Install conan
```
pipx install conan
```

Generate a conan profile
```
conan profile detect --force
```

Follow the required artifactory setup.

### Required Artifactory Setup
These instructions should only be followed after you have completed all setup steps for your platform.

Add the artifactory
```
conan remote add artifactory http://jslightham.com:8081/artifactory/api/conan/botchain
```

### Additional Artifactory Setup (optional)
These instructions should only be followed after you have completed all setup steps for your platform.

This is an optional section that is only required if you plan on uploading releases to the artifactory.

Add credentials to connect to the remote artifactory
```
conan remote login artifactory <username> -p <password>
```

Contact Johnathon to get login credentials for the artifactory.

## Development
```
# Macos and Linux users can run,
./build_control_library.sh

# Manual steps
build_type=Release  # set to your desired build type (Release, RelWithDebInfo, Debug, etc.)
conan install . --build=missing --output-folder=. -s build_type="${build_type}"
cmake -S . -B "./build/${build_type}" -DCMAKE_TOOLCHAIN_FILE="./build/${build_type}/generators/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE="${build_type}"
cmake --build "./build/${build_type}" --config "${build_type}"
```

## Building For Release
Create the package
```
conan create .
```

Upload to the artifactory
```
conan upload libcontrol -r=artifactory
```

## Crashpad
To be able to use sentry, we need a crashpad executable.
The executable can be taken out of the conan cache, ie,
```
find ~/.conan2 -type f -name "crashpad_handler" -perm /u=x,g=x,o=x 2>/dev/null | head -n 1
```

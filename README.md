# Computer Club Manager

## Build and run
### Linux
```
git clone https://github.com/mnink275/yadro-telecom-assignment.git
cd ./yadro-telecom-assignment
make build-release
./build_release/ComputerClubManager /path/to/input.txt
```

### Windows
```
git clone https://github.com/mnink275/yadro-telecom-assignment.git
cd ./yadro-telecom-assignment
mkdir build_release
cmake -S . -B ./build_release -DCMAKE_BUILD_TYPE=Release
cmake --build ./build_release --config Release
```

## Run tests
### Linux
```
make tests
```

### Windows
```
mkdir build_debug
cmake -S . -B ./build_debug -DCMAKE_BUILD_TYPE=Debug
cmake --build ./build_debug --config Debug
ctest -V -C Debug
```

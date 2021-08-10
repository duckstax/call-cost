## Lua function invoke benchmark
```
mkdir build
cp script.lua ./build
cp build 
conan install ..
cmake .. 
make
./bin/bench
```
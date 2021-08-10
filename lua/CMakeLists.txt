cmake_minimum_required(VERSION 3.10.0)
project(LUASOLBENCH CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")        



include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

 add_executable(bench main.cpp)
 target_link_libraries(bench ${CONAN_LIBS})
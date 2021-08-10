cmake_minimum_required(VERSION 3.10.0)

project(call-cost LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

add_compile_options(

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
)

#############################################################################################
#conan

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
 message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
 file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.15/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

set(CONAN_SYSTEM_INCLUDES "On")

conan_cmake_run(
        CONANFILE conanfile.txt
        BASIC_SETUP
        BUILD missing
        NO_OUTPUT_DIRS
)

# conan
#############################################################################################

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
 set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
 set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/script.lua DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package( Threads )

add_executable(bench_lua main.cpp)

target_link_libraries (
        bench_lua
        PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
        ${CONAN_LIBS}
)

target_include_directories(
        bench_lua
        PRIVATE
        ${CONAN_LIBS}
)
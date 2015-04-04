cmake_minimum_required(VERSION 2.8)
set(LLVM_DIR /Users/jikk/int-llvm/cmake/modules/)
find_package(LLVM)

include_directories(include)
include_directories(../poolalloc/include)
add_subdirectory(lib)

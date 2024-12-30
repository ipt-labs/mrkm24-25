cmake_minimum_required(VERSION 3.24)
project(AsymCryptoLabs)

set(CMAKE_CXX_STANDARD 20)

set(GMP_DIR /opt/homebrew/Cellar/gmp/6.2.1_1)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${GMP_DIR}/include)
link_directories(${GMP_DIR}/lib)
include_directories(src/gen)
include_directories(src/test)

set(CMAKE_C_COMPILER /opt/homebrew/opt/llvm/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
add_executable(AsymCryptoLabs main.cpp)


target_link_libraries(AsymCryptoLabs gmp omp)
target_link_libraries(AsymCryptoLabs ${Boost_LIBRARIES})




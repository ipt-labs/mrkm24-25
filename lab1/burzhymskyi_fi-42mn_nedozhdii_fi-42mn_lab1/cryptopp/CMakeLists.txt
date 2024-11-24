cmake_minimum_required(VERSION 3.10)
project(CryptoPPExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(/opt/homebrew/Cellar/cryptopp/8.9.0/include)
link_directories(/opt/homebrew/Cellar/cryptopp/8.9.0/lib)

add_executable(CryptoPPExample main.cpp)
target_link_libraries(CryptoPPExample cryptopp)

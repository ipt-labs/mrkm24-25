cmake_minimum_required(VERSION 3.10)
project(ElGamal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenSSL REQUIRED)

add_executable(ElGamal main.cpp)

target_link_libraries(ElGamal PRIVATE OpenSSL::SSL OpenSSL::Crypto)

if(OPENSSL_FOUND)
  message(STATUS "Found OpenSSL: ${OPENSSL_VERSION_TEXT}")
else()
  message(FATAL_ERROR "OpenSSL not found!")
endif()

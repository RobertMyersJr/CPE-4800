##############################################################################
# @file CMakeLists.txt
# @author Robert Myers Jr.
# @version V1.0
# @brief CMakeList file to compile Lab 4
##############################################################################
cmake_minimum_required(VERSION 3.15)

project(Lab4 LANGUAGES CXX)

# Use C++ 20 features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generates a compile commands file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_executable(RFM_encrypt
    des.cpp
    des_encrypt.cpp
    crypt_tools.cpp
)

add_executable(RFM_decrypt
    des.cpp
    des_decrypt.cpp
    crypt_tools.cpp
)

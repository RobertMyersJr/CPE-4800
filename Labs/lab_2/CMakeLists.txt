##############################################################################
# @file password_file.cpp
# @author Robert Myers Jr.
# @version V1.0
# @brief CMakeList file to compile Lab 1
##############################################################################
cmake_minimum_required(VERSION 3.15)

project(Lab2 LANGUAGES CXX)

# Use C++ 20 features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generates a compile commands file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_executable(lab2
    lab2_client.cpp
    client.cpp
)

# Link OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(lab2 PRIVATE OpenSSL::Crypto)

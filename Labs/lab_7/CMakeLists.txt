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

add_executable(client
    client.cpp
    bank_service.cpp
    tools/evp.cpp
    lab2_client.cpp
    user_input.cpp
)

add_executable(server
    bank_service.cpp
    tools/evp.cpp
    lab2_server.cpp
    server.cpp
)

# Link OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(client PRIVATE OpenSSL::Crypto)
target_link_libraries(server PRIVATE OpenSSL::Crypto)

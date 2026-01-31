cmake_minimum_required(VERSION 3.15)

project(Lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

find_package(OpenSSL REQUIRED)

# Define an executable target from the source file
add_executable(myexample
    lab_1.cpp
    menu.cpp
    password_file.cpp
    user_input.cpp
)

target_link_libraries(myexample PRIVATE OpenSSL::Crypto)

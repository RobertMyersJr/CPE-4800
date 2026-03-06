/**
 * @file main.cpp
 * @author Robert Myers Jr.
 * @version V1.0
 * @brief Driver program to Hash messages for Lab 5.
 */
#include <iostream>
#include <cstdio>
#include "my_hash.hpp"

int main(int argc, char* argv[]) 
{
    std::string message;
    std::string result;
    MyHash hasher;

    if (argc != 2) {
        if (argc < 2) {
            std::printf("ERROR: Need 1 argument (message)\n");
        } else {
            std::printf("ERROR: Provided too many arguments\n");
        }
        return 1;
    }

    message = argv[1];
    result = hasher.compute(message);
    
    std::printf("Original String: %s\n", message.data());
    std::printf("RFM-256 Hash: %s\n", result.c_str());
    
    return 0;
}

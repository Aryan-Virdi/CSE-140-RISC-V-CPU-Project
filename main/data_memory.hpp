#ifndef DATA_MEMORY_HPP
#define DATA_MEMORY_HPP 

#include <cstdint>

int retrieveMemory(int address, const int dataMemory[32]){
    return dataMemory[(address / 4)];
}

void storeMemory(int address, int dataMemory[32], int value){
    dataMemory[(address / 4)] = value;
}

#endif
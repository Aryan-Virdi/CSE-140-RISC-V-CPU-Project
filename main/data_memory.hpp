#ifndef DATA_MEMORY_HPP
#define DATA_MEMORY_HPP 

#include <cstdint>

int retrieveMemory(const int dataMemory[32], int address){
    return dataMemory[(address / 4)];
}

void storeMemory(int dataMemory[32], int address, int value){
    dataMemory[(address / 4)] = value;
}

#endif
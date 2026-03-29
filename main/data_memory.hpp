#ifndef DATA_MEMORY_HPP
#define DATA_MEMORY_HPP 

#include <cstdint>

/*
    Parameter dataMemory[32]:   Read-only reference to the data memory array.
    Parameter address:          An integer representing a memory address in the form of (4*i)

    Returns:                    The integer value stored in data memory at paramter address (i / 4)

    Note:                       Assumes that address is only ever an integer multiple of 4.
*/
int retrieveMemory(const int dataMemory[32], int address){
    return dataMemory[(address / 4)];
}

/*
    Parameter dataMemory[32]:   A reference to the data memory array.
    Parameter address:          An integer representing a memory address in the form of (4*i)
    Parameter value:            An integer value to be written into memory.

    Note:                       Assumes that address is only ever an integer multiple of 4.
*/
void storeMemory(int dataMemory[32], int address, int value){
    dataMemory[(address / 4)] = value;
}

#endif
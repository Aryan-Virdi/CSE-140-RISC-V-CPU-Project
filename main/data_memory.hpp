#ifndef DATA_MEMORY_HPP
#define DATA_MEMORY_HPP 

#include <cstdint>
#include "exit_codes.h"

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

/*
    Parameter dataMemory[32]:   A reference to the data memory array.
    Parameter address:          An integer representing a memory address in the form of (4*i)
    Parameter value:            An optional integer value to be written into memory. If unnecessary,
                                a dummy value should be provided.
    Parameter memWrite:         A true or false signal of whether the instruction is writing to memory
                                or not.

    Returns:                    Value held in memory at "address" if memWrite is false,
                                A generic success code if memWrite is true.

    Note:                       Assumes that address is only ever an integer multiple of 4.
*/
int mem(int dataMemory[32], int address, int value, bool memWrite){
    if (memWrite == true){
        storeMemory(dataMemory, address, value);
        return SUCCESS;
    } else {
        return retrieveMemory(dataMemory, address);
    }
}

#endif
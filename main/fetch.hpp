#ifndef FETCH_HPP
#define FETCH_HPP

#include <iostream>
#include <cstdint>
#include <vector>
#include <stdexcept>

#include "exit_codes.h"
#include "printing.hpp"

/*
    Parameter PC:                   Takes a reference to the global program counter. This will modify PC.
    Parameter instructionMemory:    Read-only reference to a vector, which should hold the instructions of the program.
    Parameter branchTarget:         A precomputed target address for an instruction in I-Memory.
    Parameter branchTaken:          Boolean flag to determine whether to increment PC or switch to target address.
    Parameter printQueue:           A reference to the global queue for printing changes to PC and memory.

    Returns:                        Instruction in instructionMemory pointed to by PC.

    Note:                           Assumes that PC is only ever a non-negative integer multiple of 4. 
                                    I.e., PC is either 0 or some multiple of 4.
*/
uint32_t fetch(int& PC, const std::vector<uint32_t>& instructionMemory, uint32_t branchTarget, bool branchTaken, PrintEvent& printQueue){
    int memoryIndex = PC / 4;
    if (!(memoryIndex < instructionMemory.size())){;
        std::cerr << "Exit Code: " << ERROR << std::endl;
        throw std::runtime_error("Segmentation Fault: Out-of-Bounds memory accessed");
    }
    
    uint32_t instruction = instructionMemory[memoryIndex];
    uint32_t nextPC = PC + 4;

    PC = (branchTaken ? branchTarget : nextPC);

    printQueue.addPrintEvent(locationType::programCounter, EMPTY_IDX, PC);

    return instruction;
}

#endif
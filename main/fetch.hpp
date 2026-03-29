#ifndef FETCH_HPP
#define FETCH_HPP

#include <iostream>
#include <cstdint>
#include <vector>
#include <stdexcept>

#include "exit_codes.h"

/*

    Parameter PC:                   Takes a reference to the global program counter. This will modify PC.
    Parameter instructionMemory:    Read-only reference to a vector, which should hold the instructions of the program.
    Parameter branchTarget:         A precomputed target address for an instruction in I-Memory.
    Parameter branchTaken:           Boolean flag to determine whether to increment PC or switch to target address.

    Returns:                        Instruction in instructionMemory held at the input PC.

    Note:                           Assumes that (PC / 4) is only ever an integer. I.e., PC is either 0 or some 
                                    multiple of 4

*/
uint32_t fetch(uint32_t& PC, const std::vector<uint32_t>& instructionMemory, uint32_t branchTarget, bool branchTaken){
    int memoryIndex = PC / 4;
    if (!(memoryIndex < instructionMemory.size())){;
        std::cerr << "Exit Code: " << ERROR << std::endl;
        throw std::runtime_error("Segmentation Fault: Out-of-Bounds memory accessed");
    }
    
    uint32_t instruction = instructionMemory[memoryIndex];
    uint32_t nextPC = PC + 4;

    PC = (branchTaken ? branchTarget : nextPC);

    return instruction;
}

#endif
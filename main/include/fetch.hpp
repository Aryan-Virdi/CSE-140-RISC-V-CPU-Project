#ifndef FETCH_HPP
#define FETCH_HPP

#include <iostream>
#include <cstdint>
#include <vector>
#include <stdexcept>

#include "exit_codes.h"
#include "printing.hpp"

/* 
    Parameter PC:                   The value of the global program counter.
    Parameter nextPC:               A reference to a separate copy of PC. Increment it by four.
    Parameter currPC:               A reference to a separate copy of PC. Saves the current PC.
    Parameter instructionMemory:    Read-only reference to a vector, which should hold the instructions of the program.
    Parameter printQueue:           A reference to the global queue for printing changes to PC and memory.

    Returns:                        Instruction in instructionMemory pointed to by PC.

    Note:                           Assumes that PC is only ever a non-negative integer multiple of 4. 
                                    I.e., PC is either 0 or some multiple of 4.
*/
uint32_t fetch(int PC, int& nextPC, int& currPC, const std::vector<uint32_t>& instructionMemory, PrintEvent& printQueue){
    int memoryIndex = PC / 4;
    if (!(memoryIndex < instructionMemory.size())){;
        std::cerr << "Exit Code: " << ERROR << std::endl;
        throw std::runtime_error("Segmentation Fault: Out-of-Bounds memory accessed");
    }
    
    uint32_t instruction = instructionMemory[memoryIndex];
    currPC = PC;
    nextPC = PC + 4;

    // std::cout << PC << std::endl;
    // std::cout << jump << std::endl;
    // std::cout << nextPC << std::endl;

    // bool branchTaken = ((branch && aluZero) || jump);
    // PC = (branchTaken ? branchTarget : nextPC);

    // printQueue.addPrintEvent(LocationType::programCounter, EMPTY_IDX, PC);

    return instruction;
}

#endif
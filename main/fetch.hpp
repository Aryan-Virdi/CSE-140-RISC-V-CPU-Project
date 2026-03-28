#ifndef FETCH_HPP
#define FETCH_HPP

#include <vector>

/*

    Parameter PC:                   Takes a reference to the global program counter. This will modify PC.
    Parameter instructionMemory:    Read-only reference to a vector, which should hold the instructions of the program.
    Parameter branchTarget:         A precomputed target address for an instruction in I-Memory.
    Paramter branchTaken:           Boolean flag to determine whether to increment PC or switch to target address.

    Returns:                        Instruction in instructionMemory held at the input PC.

    Note:                           Assumes that (PC / 4) is only ever an integer. I.e., PC is either 0 or some 
                                    multiple of 4

*/
uint32_t fetch(uint32_t& PC, const std::vector<uint32_t>& instructionMemory, uint32_t branchTarget, bool branchTaken){
    if (!((PC / 4) < instructionMemory.size())) return 0;
    
    uint32_t instruction = instructionMemory[PC / 4];
    uint32_t nextPC = PC + 4;

    PC = (branchTaken ? branchTarget : nextPC);

    return instruction;
}

#endif
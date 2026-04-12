#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#include "register_indices.h"
#include "instruction_memory.hpp"
#include "data_memory.hpp"
#include "exit_codes.h"
#include "control_unit.hpp"
#include "fetch.hpp"
#include "instruction.hpp"
#include "decoder.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

int total_clock_cycles = 0;

// CPU's program counter
int PC = 0;

// CPU's register file. 32 registers
int rf[32];

// Data Memory
int d_mem[32];

// Computer's instruction memory
vector<uint32_t> instructionMemory;

// Control signals
int regWrite = 0;
int branch = 0;
int ALUSrc = 0;
int memWrite = 0;
int memToReg = 0;
int memRead = 0;
int ALUOp = 0;

IControl controlSignals = IControl(&regWrite, &branch, &ALUSrc, &memWrite, &memToReg, &memRead, &ALUOp);

int alu_zero = 0;
int branch_target = 0;

PrintEvent printQueue = PrintEvent();

int main(int argc, char* argv[]) {
    if (argc < 2){ 
        cerr << "Input file name not provided. Program terminating." << endl;
        return ILLEGAL_ARGUMENT;
    }

    // All 32 registers initialized to zero.
    for (int i = 0; i < 32; i++){ rf[i] = 0; d_mem[i] = 0; }  // Global arrays should be initialized to zero automatically, but here it is done manually just in case.

    // Terminal argument "--sample-init" initializes memories to sample values.
    if (argc == 3){
        string sampleArg = argv[2];
        if (sampleArg == "--sample-1"){
            rf[x1] = 0x20;
            rf[x2] = 0x5;
            rf[x10] = 0x70;
            rf[x11] = 0x4;

            storeMemory(d_mem, 0x70, 0x5);
            storeMemory(d_mem, 0x74, 0x10);

        } else if (sampleArg == "--sample-2"){
            rf[s0] = 0x20;
            rf[a0] = 0x5;
            rf[a1] = 0x2;
            rf[a2] = 0xA;
            rf[a3] = 0xF;
        } else {
            cerr << "Sample case argument malformed. Program terminating." << endl;
            return ILLEGAL_ARGUMENT;
        }
    }

    string programFileName = argv[1];
    populateInstructionMemory(programFileName, instructionMemory);  // Populate instruction memory with program instructions.

    while ((PC/4) < instructionMemory.size()){
        uint32_t currInstruction = fetch(PC, instructionMemory, branch_target, static_cast<bool>(branch), printQueue);
        Instruction instruction = decode(currInstruction, controlSignals, rf);

        int alu_ctrl = aluControl(ALUOp, instruction.getFunct3(), instruction.getFunct7());
        int aluResult /* = execute(); */;

        int data = mem(d_mem, aluResult, instruction.getRs2(), static_cast<bool>(memWrite));    // Returns an actual d_mem value if memWrite is true.
        int writeDataSource = (static_cast<bool>(memToReg)) ? data : aluResult;                 // data to be written back is from "data" if memToReg is true. Directly from the ALU otherwise.
        // writeBack();

        total_clock_cycles++;
    }

    return SUCCESS;
}
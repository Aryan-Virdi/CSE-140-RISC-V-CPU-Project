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
#include "execute.hpp"
#include "writeback.hpp"
#include "printing.hpp"

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

// Global control signal interface.
IControl controlSignals = IControl(&regWrite, &branch, &ALUSrc, &memWrite, &memToReg, &memRead, &ALUOp);

// Execution signals for use in comparison/branching.
int alu_zero = 0;
int branch_target = 0;

// Queue for printing modifications.
// Not related to a concrete object on the chip; just a descriptor.
PrintEvent printQueue = PrintEvent();

/*
    Parameter argc: Number of arguments passed in by terminal.
    Parameter argv: char pointer array containing those arguments. Should be in the form of:
                            argv[0]: program name
                            argv[1]: input file containing machine code instruction(s)
                            argv[2]: a flag describing which sample initialization to use (--sample-1 or --sample-2)

    Returns:        Successful code upon graceful completion. Specific error code otherwise.
*/
int main(int argc, char* argv[]) {
    if (argc < 2){ 
        cerr << "Input file name not provided. Program terminating." << endl;
        return ILLEGAL_ARGUMENT;
    }

    // All 32 registers initialized to zero.
    for (int i = 0; i < 32; i++){ rf[i] = 0; d_mem[i] = 0; }  // Global arrays should be initialized to zero automatically, but here it is done manually just in case.

    // Terminal argument "--sample-init" initializes memories to sample values.
    if (argc >= 3){
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
        uint32_t currInstruction = fetch(PC, instructionMemory, branch_target, static_cast<bool>(branch), static_cast<bool>(alu_zero), printQueue);
        Instruction instruction = decode(currInstruction, controlSignals, rf);

        int alu_ctrl = aluControl(ALUOp, instruction.getFunct3(), instruction.getFunct7());
        int operand1 = instruction.getRs1Value();
        int operand2 = (static_cast<bool>(ALUSrc) ? instruction.getImm() : instruction.getRs2Value());  // Second operand of ALU operation is from immediate if ALUSrc is true, otherwise from rs2.
                                                                                                        // Logic depends on ALUSrc being true if and only if the instruction is an I-Type.

        int aluResult = execute(operand1, operand2, alu_ctrl, instruction.getImm(), PC, alu_zero, branch_target);

        int data = mem(d_mem, aluResult, instruction.getRs2Value(), static_cast<bool>(memWrite), printQueue);   // Returns an actual d_mem value if memWrite is true.
                                                                                                                // The value in this function call is the second source register because
                                                                                                                // memory should only be written into by store-word, which provides the data in RS2.

        writeback(aluResult, data, static_cast<bool>(regWrite), static_cast<bool>(memToReg), rf, instruction.getRd(), total_clock_cycles, printQueue);

        cout << "total_clock_cycles " << total_clock_cycles << " :" << endl;
        printQueue.printModifications();    // Print this cycle's modifications.
    }

    cout << "program terminated:" << endl << "total execution time is " << total_clock_cycles << " cycles" << endl;

    return SUCCESS;
}
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#include "instruction_memory.hpp"
#include "data_memory.hpp"
#include "exit_codes.h"
#include "control_unit.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

// Register addresses for convienience, including their alias(es).
const int x0 = 0;       const int zero = x0;
const int x1 = 1;       const int ra = x1;
const int x2 = 2;       const int sp = x2;
const int x3 = 3;       const int gp = x3;
const int x4 = 4;       const int tp = x4;
const int x5 = 5;       const int t0 = x5;
const int x6 = 6;       const int t1 = x6;
const int x7 = 7;       const int t2 = x7;
const int x8 = 8;       const int s0 = x8;      const int fp = x8;
const int x9 = 9;       const int s1 = x9;
const int x10 = 10;     const int a0 = x10;
const int x11 = 11;     const int a1 = x11;
const int x12 = 12;     const int a2 = x12;
const int x13 = 13;     const int a3 = x13;
const int x14 = 14;     const int a4 = x14;
const int x15 = 15;     const int a5 = x15;
const int x16 = 16;     const int a6 = x16;
const int x17 = 17;     const int a7 = x17;
const int x18 = 18;     const int s2 = x18;
const int x19 = 19;     const int s3 = x19;
const int x20 = 20;     const int s4 = x20;
const int x21 = 21;     const int s5 = x21;
const int x22 = 22;     const int s6 = x22;
const int x23 = 23;     const int s7 = x23;
const int x24 = 24;     const int s8 = x24;
const int x25 = 25;     const int s9 = x25;
const int x26 = 26;     const int s10 = x26;
const int x27 = 27;     const int s11 = x27;
const int x28 = 28;     const int t3 = x28;
const int x29 = 29;     const int t4 = x29;
const int x30 = 30;     const int t5 = x30;
const int x31 = 31;     const int t6 = x31;

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

    return SUCCESS;
}
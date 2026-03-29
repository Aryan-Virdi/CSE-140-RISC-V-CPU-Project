#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#include "instruction_memory.hpp"
#include "data_memory.hpp"
#include "exit_codes.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

// Register addresses for convienience.
const int x0 = 0;
const int x1 = 1;
const int x2 = 2;
const int x3 = 3;
const int x4 = 4;
const int x5 = 5;
const int x6 = 6;
const int x7 = 7;
const int x8 = 8;
const int x9 = 9;
const int x10 = 10;
const int x11 = 11;
const int x12 = 12;
const int x13 = 13;
const int x14 = 14;
const int x15 = 15;
const int x16 = 16;
const int x17 = 17;
const int x18 = 18;
const int x19 = 19;
const int x20 = 20;
const int x21 = 21;
const int x22 = 22;
const int x23 = 23;
const int x24 = 24;
const int x25 = 25;
const int x26 = 26;
const int x27 = 27;
const int x28 = 28;
const int x29 = 29;
const int x30 = 30;
const int x31 = 31;

// CPU's program counter
int PC = 0;

// CPU's register file. 32 registers
int rf[32];

// Data Memory
int d_mem[32];

// Computer's instruction memory
vector<uint32_t> instructionMemory;


int main(int argc, char* argv[]) {

    // All 32 registers initialized to zero.
    for (int i = 0; i < 32; i++){ rf[i] = 0; d_mem[i] = 0; }  // Global arrays should be initialized to zero automatically, but here it is done manually just in case.

    // Terminal argument "--sample-init" initializes memories to sample values.
    if (argc == 3 && argv[3] == "--sample-init"){
        rf[x1] = 0x20;
        rf[x2] = 0x5;
        rf[x10] = 0x70;
        rf[x11] = 0x4;

        storeMemory(d_mem, 0x70, 0x5);
        storeMemory(d_mem, 0x74, 0x10);
    }

    string programFileName = argv[1];
    populateInstructionMemory(programFileName, instructionMemory);  // Populate instruction memory with program instructions.

    return SUCCESS;
}
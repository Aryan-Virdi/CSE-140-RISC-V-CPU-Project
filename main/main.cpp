#include <iostream>
#include <string>
#include <vector>

#include "instruction_memory.hpp"
#include "exit_codes.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

const int ERROR_CODE = -1;

// CPU's program counter
int PC = 0;

// CPU's register file. 32 registers
int rf[32];

// Computer's instruction memory
vector<uint32_t> instructionMemory;


int main(int argc, char* argv[]) {

    // All 32 registers initialized to zero.
    for (int i = 0; i < 32; i++){ rf[i] = 0; }  // Global arrays should be initialized to zero automatically, but here it is done manually just in case.

    string programFileName = argv[1];
    populateInstructionMemory(programFileName, instructionMemory);  // Populate instruction memory with program instructions.

    return SUCCESS;
}
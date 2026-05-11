#ifndef INSTRUCTION_MEMORY_HPP
#define INSTRUCTION_MEMORY_HPP

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

#include "exit_codes.h"
#include "data_memory.hpp"

// Register addresses for convenience, including their alias(es).
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


/*
    Parameter fileName:             A string representing the name of the file.
    Parameter instructionMemory:    A reference to a vector of uint32_t.

    Returns:                        0 on successful execution. ERROR_CODE otherwise.

    Description:                    Populates the "instruction memory" of the CPU program
                                    with machine code from the input file.
*/
int populateInstructionMemory(std::string fileName, std::vector<uint32_t>& instructionMemory){
    std::fstream programFile(fileName);

    if (!programFile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return ERROR;
    }

    std::string binaryString;
    while(std::getline(programFile, binaryString)){
        uint32_t machineCode = std::stoul(binaryString, nullptr, 2);    // Parse binary strings from file as binary number.
        instructionMemory.push_back(machineCode);                       // Store in instruction memory.
    }

    return SUCCESS;
}

/*
    Parameter argc:                     An integer representing how many terminal arguments were supplied.
    Parameter argv[]:                   C-style array that holds the terminal arguments.
    Parameter rf[32]:                   A reference to the register file array.
    Parameter d_mem[32]:                A reference to the data memory array.
    Parameter pipelined:                A reference to the global boolean pipelined. Determines which architecture to use.
    Parameter instructionMemory:        A reference to a vector of uint32_t, which represents the machine code instructions.
    Parameter withConventionalNames:    A reference to a global boolean. Used for sample2 output printing.

    Note:                               Also handles initialization of sample cases and populating instruction memory.
    Note:                               Sample arguments one and two may be initialized together; they are not mutually exclusive.

    Returns:                            Success code on graceful initialization. A specific error code otherwise.                           

*/
int processArguments(int argc, char* argv[], int rf[32], int d_mem[32], bool& pipelined, std::vector<uint32_t>& instructionMemory, bool& withConventionalNames){
    bool optionalArgsPresent;
    if (argc > 2){ optionalArgsPresent = true; }

    std::vector<std::string> arguments(argc);
    for (int i = 0; i < argc; i++){ arguments[i] = static_cast<std::string>(argv[i]); }

    std::string programFileName = arguments[1];
    if (!optionalArgsPresent) { return populateInstructionMemory(programFileName, instructionMemory); };

    for (int j = 2; j < argc; j++){
        std::string arg = arguments[j];
        if (arg == "--sample-1"){
            withConventionalNames = false;
            rf[x1] = 0x20;
            rf[x2] = 0x5;
            rf[x10] = 0x70;
            rf[x11] = 0x4;
            storeMemory(d_mem, 0x70, 0x5);
            storeMemory(d_mem, 0x74, 0x10);
            continue;
        } else if (arg == "--sample-2"){
            withConventionalNames = true;
            rf[s0] = 0x20;
            rf[a0] = 0x5;
            rf[a1] = 0x2;
            rf[a2] = 0xA;
            rf[a3] = 0xF;
            continue;
        } else if (arg == "--pipelined"){
            pipelined = true;
            continue;
        } else {
            std::cerr << "Malformed argument(s). Program terminating." << std::endl;
            return ILLEGAL_ARGUMENT;
        }
    }

    return populateInstructionMemory(programFileName, instructionMemory);
}

#endif
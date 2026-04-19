#ifndef INSTRUCTION_MEMORY_HPP
#define INSTRUCTION_MEMORY_HPP

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

#include "exit_codes.h"
#include "data_memory.hpp"
#include "register_indices.h"


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
    Parameter argc:                 An integer representing how many terminal arguments were supplied.
    Parameter argv[]:               C-style array that holds the terminal arguments.
    Parameter rf[32]:               A reference to the register file array.
    Parameter d_mem[32]:            A reference to the data memory array.
    Parameter pipelined:            A reference to the global boolean pipelined. Determines which architecture to use.
    Parameter instructionMemory:    A reference to a vector of uint32_t, which represents the machine code instructions.

    Note:                           Also handles initialization of sample cases and populating instruction memory.

    Returns:                        Success code on graceful initialization. A specific error code otherwise.                           

*/
int processArguments(int argc, char* argv[], int rf[32], int d_mem[32], bool& pipelined, std::vector<uint32_t>& instructionMemory){
    bool optionalArgsPresent;
    if (argc > 2){ optionalArgsPresent = true; }

    std::vector<std::string> arguments(argc);
    for (int i = 0; i < argc; i++){ arguments[i] = static_cast<std::string>(argv[i]); }

    std::string programFileName = arguments[1];
    if (!optionalArgsPresent) { return populateInstructionMemory(programFileName, instructionMemory); };

    for (int j = 2; j < argc; j++){
        std::string arg = arguments[j];
        if (arg == "--sample-1"){
            rf[x1] = 0x20;
            rf[x2] = 0x5;
            rf[x10] = 0x70;
            rf[x11] = 0x4;
            storeMemory(d_mem, 0x70, 0x5);
            storeMemory(d_mem, 0x74, 0x10);
            continue;
        } else if (arg == "--sample-2"){
            rf[s0] = 0x20;
            rf[a0] = 0x5;
            rf[a1] = 0x2;
            rf[a2] = 0xA;
            rf[a3] = 0xF;
            continue;
        } else if (arg == "--pipelined"){
            pipelined = true;
            continue;
        } else if (j == (argc - 1)){
            std::cerr << "Malformed argument(s). Program terminating." << std::endl;
            return ILLEGAL_ARGUMENT;
        }
    }

    return populateInstructionMemory(programFileName, instructionMemory);
}

#endif
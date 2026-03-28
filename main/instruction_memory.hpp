#ifndef INSTRUCTION_MEMORY_HPP
#define INSTRUCTION_MEMORY_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const int ERROR_CODE = -1;


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
        return ERROR_CODE;
    }

    std::string binaryString;
    while(std::getline(programFile, binaryString)){
        uint32_t machineCode = std::stoul(binaryString, nullptr, 2);    // Parse binary strings from file as binary number.
        instructionMemory.push_back(machineCode);                       // Store in instruction memory.
    }

    return 0;
}

#endif
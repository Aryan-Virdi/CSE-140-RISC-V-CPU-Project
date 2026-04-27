#ifndef DECODER_HPP
#define DECODER_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <ostream>

#include "instruction.hpp"
#include "util/bit_utils.hpp"
#include "control_unit.hpp"

// Concatenate opcode, funct3/7 into "funct7|funct3|opcode" for use as a key to a map.
uint32_t makeFormatKey(uint32_t opcode, uint32_t funct3, uint32_t funct7) {
    return opcode | (funct3 << 7) | (funct7 << 10);
}

/*
    These tables may be better optimized by using a switch-case statement
    since we are working with relatively small amounts of values per table.
    Though, the decoder already processes information quite quick. The
    switch-case idea is just something to keep in mind.

    Unordered Maps in C++ are non-sorted hash tables.
*/

// Table of opcode/funct3/funct7 to operation (R-Type)
std::unordered_map<uint32_t, Operation> R_Format_Map = {
    { makeFormatKey(0b0110011, 0b000, 0b0000000), Operation::ADD },
    { makeFormatKey(0b0110011, 0b111, 0b0000000), Operation::AND },
    { makeFormatKey(0b0110011, 0b110, 0b0000000), Operation::OR },
    { makeFormatKey(0b0110011, 0b001, 0b0000000), Operation::SLL },
    { makeFormatKey(0b0110011, 0b010, 0b0000000), Operation::SLT },
    { makeFormatKey(0b0110011, 0b011, 0b0000000), Operation::SLTU },
    { makeFormatKey(0b0110011, 0b101, 0b0100000), Operation::SRA },
    { makeFormatKey(0b0110011, 0b101, 0b0000000), Operation::SRL },
    { makeFormatKey(0b0110011, 0b000, 0b0100000), Operation::SUB },
    { makeFormatKey(0b0110011, 0b100, 0b0000000), Operation::XOR }
};

// Table of opcode/funct3/funct7 to operation (UJ-Type)
std::unordered_map<uint32_t, Operation> UJ_Format_Map = {
    { makeFormatKey(0b1101111, FUNCT3_EMPTY, FUNCT7_EMPTY), Operation::JAL }
};

// Table of opcode/funct3/funct7 to operation (I-Type)
std::unordered_map<uint32_t, Operation> I_Format_Map = {
    { makeFormatKey(0b0010011, 0b000, FUNCT7_EMPTY), Operation::ADDI },
    { makeFormatKey(0b0010011, 0b111, FUNCT7_EMPTY), Operation::ANDI },
    { makeFormatKey(0b1100111, 0b000, FUNCT7_EMPTY), Operation::JALR },
    { makeFormatKey(0b0000011, 0b000, FUNCT7_EMPTY), Operation::LB },
    { makeFormatKey(0b0000011, 0b001, FUNCT7_EMPTY), Operation::LH },
    { makeFormatKey(0b0000011, 0b010, FUNCT7_EMPTY), Operation::LW },
    { makeFormatKey(0b0010011, 0b110, FUNCT7_EMPTY), Operation::ORI }, 
    { makeFormatKey(0b0010011, 0b001, 0b0000000),    Operation::SLLI },
    { makeFormatKey(0b0010011, 0b010, FUNCT7_EMPTY), Operation::SLTI },
    { makeFormatKey(0b0010011, 0b011, FUNCT7_EMPTY), Operation::SLTIU },
    { makeFormatKey(0b0010011, 0b101, 0b0100000),    Operation::SRAI },
    { makeFormatKey(0b0010011, 0b101, 0b0000000),    Operation::SRLI },
    { makeFormatKey(0b0010011, 0b100, FUNCT7_EMPTY), Operation::XORI}
};

// Table of opcode/funct3/funct7 to operation (S-Type)
std::unordered_map<uint32_t, Operation> S_Format_Map = {
    { makeFormatKey(0b0100011, 0b000, FUNCT7_EMPTY), Operation::SB },
    { makeFormatKey(0b0100011, 0b001, FUNCT7_EMPTY), Operation::SH },
    { makeFormatKey(0b0100011, 0b010, FUNCT7_EMPTY), Operation::SW }
};

// Table of opcode/funct3/funct7 to operation (SB-Type)
std::unordered_map<uint32_t, Operation> SB_Format_Map = {
    { makeFormatKey(0b1100011, 0b000, FUNCT7_EMPTY), Operation::BEQ },
    { makeFormatKey(0b1100011, 0b101, FUNCT7_EMPTY), Operation::BGE },
    { makeFormatKey(0b1100011, 0b100, FUNCT7_EMPTY), Operation::BLT },
    { makeFormatKey(0b1100011, 0b001, FUNCT7_EMPTY), Operation::BNE }
};

/*
    Parameter opcode:  A 32-bit unsigned integer representing the instruction's opcode.
    
    Returns:           An enumerated value representing the instruction's format.
*/
Format getFormat(uint32_t opcode) {
    switch(opcode) {
        case 0b0110011: return Format::R;

        case 0b0010011: return Format::I;
        case 0b0000011: return Format::I; // lb, lw, lh
        case 0b1100111: return Format::I; // jalr

        case 0b1100011: return Format::SB;

        case 0b1101111: return Format::UJ;

        case 0b0100011: return Format::S;

        default: return Format::UNKNOWN;
    }
}

/*
    Parameter instruction:  A 32-bit unsigned integer representing machine code.
    
    Returns:                An instruction object of R format, and its fields.
*/
Instruction decode_R(uint32_t instruction){
    // Extract bit fields for R-Type instructions.
    uint32_t opcode = extractBits(instruction, 0, 6);
    uint32_t rd = extractBits(instruction, 7, 11);
    uint32_t funct3 = extractBits(instruction, 12, 14);
    uint32_t rs1 = extractBits(instruction, 15, 19);
    uint32_t rs2 = extractBits(instruction, 20, 24);
    uint32_t funct7 = extractBits(instruction, 25, 31);

    uint32_t key = makeFormatKey(opcode, funct3, funct7);   // Make the key for the instruction.
    Operation operation = R_Format_Map.at(key);             // Retrieve the value for the key.

    // Build the instruction object.
    return Instruction(Format::R, operation, (int)rd, (int)rs1, (int)rs2, (int)funct3, (int)funct7, IMM_EMPTY);
}

/*
    Parameter instruction:      A 32-bit unsigned integer representing machine code.
    
    Returns:                    An instruction object of SB format, and its fields.
*/
Instruction decode_SB(uint32_t instruction){

    // Extract bit fields for SB-type.
    uint32_t opcode = extractBits(instruction, 0, 6);

    uint32_t funct3 = extractBits(instruction, 12, 14);
    uint32_t rs1 = extractBits(instruction, 15, 19);
    uint32_t rs2 = extractBits(instruction, 20, 24);

    uint32_t key = makeFormatKey(opcode, funct3, FUNCT7_EMPTY);    // Make key for this instruction.
    Operation operation = SB_Format_Map.at(key);        // Retrieve value for that key.

    // Extract each bit from the immediate field and reorder.
    uint32_t imm_12_and_10_to_5 = extractBits(instruction, 25, 31); // 7 bits. Bits [25, 31] are placed into bits [0, 6] of return value.
                                                                    // The 12th bit of imm is the most-sig. bit of the return value.
                                                                    // The rest are bits 5-10 (least-to-most sig.).
    uint32_t imm_4_to_1_and_11 = extractBits(instruction, 7, 11);   // 5 bits Bits [7, 11] are placed into bits [0, 4] of return value.
                                                                    // The 11th bit of imm is the least-sig. bit of the return value.
                                                                    // The rest are bits 1-4 (least-to-most sig.).

    uint32_t twelfthBit = extractBits(imm_12_and_10_to_5, 6, 6);        // One bit
    uint32_t eleventhBit = extractBits(imm_4_to_1_and_11, 4, 4);        // One bit
    uint32_t fifthToTenthBits = extractBits(imm_12_and_10_to_5, 0, 5);  // Six bits
    uint32_t firstToFourthBits = extractBits(imm_4_to_1_and_11, 0, 3);  // Four bits.

    int32_t raw_immediate = ((twelfthBit << 11) | (eleventhBit << 10) | (fifthToTenthBits << 4) | (firstToFourthBits)); // 12 bits
    /*                             |                    |                        |                         |
    "->" denotes bit-shift         |                    |                        |                         |
                                   |                    |                        |                  Four bits 0-3
                                   |                    |                 Six bits 0-5 -> 4-9
                                   |              One bit 0 -> 10
                             One bit 0 -> 11
       12 bits [0, 11]
    */

    int immediate = (int)twos_complement(raw_immediate, S_SB_TYPE_IMM_BITS);    // Reread the reconstructed immediate as a signed value in 2's complement.
                                                                                // There are twelve bits of immediate.

    // Build the instruction object.
    return Instruction(Format::SB, operation, RD_EMPTY, (int)rs1, (int)rs2, (int)funct3, FUNCT7_EMPTY, immediate);
}

/*
    Parameter instruction:  A 32-bit unsigned integer representing machine code.
    
    Returns:                An instruction object of I format, and its fields.
*/
Instruction decode_I(uint32_t instruction){
    uint32_t opcode = extractBits(instruction, 0, 6);
    uint32_t rd = extractBits(instruction, 7, 11);
    uint32_t funct3 = extractBits(instruction, 12, 14);
    uint32_t rs1 = extractBits(instruction, 15, 19);
    uint32_t imm_11_to_0 = extractBits(instruction, 20, 31); // 12 bits
    uint32_t funct7 = extractBits(instruction, 25, 31); // needed for shifts

    // Determine key (shift instructions require funct7)
    uint32_t key;
    if (funct3 == 0b001 || funct3 == 0b101) { 
        key = makeFormatKey(opcode, funct3, funct7);
    } else {
        key = makeFormatKey(opcode, funct3, FUNCT7_EMPTY);
    }

    Operation operation = I_Format_Map.at(key);

    int immediate = (int)twos_complement(imm_11_to_0, I_TYPE_IMM_BITS);

    return Instruction(Format::I, operation, (int)rd, (int)rs1, RS2_EMPTY, (int)funct3, (int)funct7, immediate);
}

/*
    Parameter instruction:  A 32-bit integer unsigned representing machine code.
    
    Returns:                An instruction object of S format, and its fields.
*/
Instruction decode_S(uint32_t instruction){
    uint32_t opcode = extractBits(instruction, 0, 6);
    uint32_t funct3 = extractBits(instruction, 12, 14);
    uint32_t rs1 = extractBits(instruction, 15, 19);
    uint32_t rs2 = extractBits(instruction, 20, 24);
    uint32_t imm_11_to_5 = extractBits(instruction, 25, 31);
    uint32_t imm_4_to_0  = extractBits(instruction, 7, 11);

    uint32_t raw_immediate = (imm_11_to_5 << 5) | imm_4_to_0;

    int immediate = (int)twos_complement(raw_immediate, S_SB_TYPE_IMM_BITS);

    uint32_t key = makeFormatKey(opcode, funct3, FUNCT7_EMPTY);

    Operation operation = S_Format_Map.at(key);

    return Instruction(Format::S,operation,RD_EMPTY,(int)rs1,(int)rs2,(int)funct3,FUNCT7_EMPTY,immediate);
}

/*
    Parameter instruction:  A 32-bit unsigned integer representing machine code.
    
    Returns:                An instruction object of UJ format, and its fields.
*/
Instruction decode_UJ(uint32_t instruction){
    uint32_t opcode = extractBits(instruction, 0, 6);
    Operation operation = UJ_Format_Map.at(makeFormatKey(opcode, FUNCT3_EMPTY, FUNCT7_EMPTY));
    uint32_t rd = extractBits(instruction, 7, 11);

    uint32_t twentiethBit = extractBits(instruction, 31, 31);
    uint32_t imm_10_to_1bit = extractBits(instruction, 21, 30);
    uint32_t eleventhBit = extractBits(instruction, 20, 20);
    uint32_t imm_19_to_12 = extractBits(instruction, 12, 19);

    uint32_t raw_immediate = (twentiethBit << 20) | (imm_10_to_1bit << 1) | (eleventhBit << 11) |(imm_19_to_12 << 12); 

    int32_t immediate = twos_complement(raw_immediate, UJ_TYPE_IMM_BITS);

    return Instruction(Format::UJ, operation, (int)rd, RS1_EMPTY, RS2_EMPTY, FUNCT3_EMPTY, FUNCT7_EMPTY, (int)immediate);
}

/*
    Parameter instruction:          A 32-bit unsigned integer representing machine code.
    Parameter ctrlSignals:          A reference to an IControl interface object.
    Parameter registerFile[32]:     A read-only reference to the register file.
    
    Returns:                        An instruction object including its format type, fields, and their values.
*/
Instruction decode(uint32_t instruction, IControl& ctrlSignals, const int registerFile[32]) {
    Instruction decodedInstruction = Instruction();
    uint32_t opcode = extractBits(instruction, 0, 6);   // Read first seven bits of machine code.

    controlUnit(opcode, ctrlSignals);   // Generate control signals.
    Format type = getFormat(opcode);    // Get instruction format.

    switch(type){
        case Format::R:
            decodedInstruction = decode_R(instruction); 
            decodedInstruction.setRs1Value(registerFile[decodedInstruction.getRs1()]);
            decodedInstruction.setRs2Value(registerFile[decodedInstruction.getRs2()]);
            break;

        case Format::SB:
            decodedInstruction = decode_SB(instruction); 
            decodedInstruction.setRs1Value(registerFile[decodedInstruction.getRs1()]);
            decodedInstruction.setRs2Value(registerFile[decodedInstruction.getRs2()]);
            decodedInstruction.setImm(signExtend(decodedInstruction.getImm(), S_SB_TYPE_IMM_BITS));
            break;

        case Format::I:   
            decodedInstruction = decode_I(instruction); 
            decodedInstruction.setRs1Value(registerFile[decodedInstruction.getRs1()]);
            decodedInstruction.setImm(signExtend(decodedInstruction.getImm(), I_TYPE_IMM_BITS));
            break;

        case Format::S:     
            decodedInstruction = decode_S(instruction); 
            decodedInstruction.setRs1Value(registerFile[decodedInstruction.getRs1()]);
            decodedInstruction.setRs2Value(registerFile[decodedInstruction.getRs2()]);
            decodedInstruction.setImm(signExtend(decodedInstruction.getImm(), S_SB_TYPE_IMM_BITS));
            break;

        case Format::UJ:   
            decodedInstruction = decode_UJ(instruction); 
            decodedInstruction.setImm(signExtend(decodedInstruction.getImm(), UJ_TYPE_IMM_BITS));
            break;

        default: break;
    }

    return decodedInstruction;
}

#endif
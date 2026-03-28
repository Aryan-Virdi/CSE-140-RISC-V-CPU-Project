#ifndef DECODER_HPP
#define DECODER_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <ostream>

enum class Format {
    R, I, S, SB, UJ, UNKNOWN    // Enum represents known "classes" of formats.
};

enum class Operation {
    // Enum represents known classes of operations.
    ADD, ADDI, 
    AND, ANDI, 
    BEQ, BGE, BLT, BNE, 
    JAL, JALR, 
    LB, LH, LW, 
    OR, ORI, 
    SB, SH, 
    SLL, SLLI, 
    SLT, SLTI, SLTIU, SLTU, 
    SRA, SRAI, 
    SRL, SRLI, 
    SUB, 
    SW, 
    XOR, XORI, 
    UNKNOWN
};

std::ostream& operator<<(std::ostream& os, Format f) {  // Overrides the print operation for Format enum
    switch(f) {                                         // so that the character is printed.
        case Format::R:       return os << "R";
        case Format::I:       return os << "I";
        case Format::S:       return os << "S";
        case Format::SB:      return os << "SB";
        case Format::UJ:      return os << "UJ";
        case Format::UNKNOWN: return os << "UNKNOWN";
        default:              return os << "Unknown";
    }
}

std::ostream& operator<<(std::ostream& os, Operation o){
    // Overrides the print operator for Operation enum
    // so that the types are printed out as printable strings.
    switch(o){
        case Operation::ADD:     return os << "add";
        case Operation::ADDI:    return os << "addi";
        case Operation::AND:     return os << "and";
        case Operation::ANDI:    return os << "andi";
        case Operation::BEQ:     return os << "beq";
        case Operation::BGE:     return os << "bge";
        case Operation::BLT:     return os << "blt";
        case Operation::BNE:     return os << "bne";
        case Operation::JAL:     return os << "jal";
        case Operation::JALR:    return os << "jalr";
        case Operation::LB:      return os << "lb";
        case Operation::LH:      return os << "lh";
        case Operation::LW:      return os << "lw";
        case Operation::OR:      return os << "or";
        case Operation::ORI:     return os << "ori";
        case Operation::SB:      return os << "sb";
        case Operation::SH:      return os << "sh";
        case Operation::SLL:     return os << "sll";
        case Operation::SLLI:    return os << "slli";
        case Operation::SLT:     return os << "slt";
        case Operation::SLTI:    return os << "slti";
        case Operation::SLTIU:   return os << "sltiu";
        case Operation::SLTU:    return os << "sltu";
        case Operation::SRA:     return os << "sra";
        case Operation::SRAI:    return os << "srai";
        case Operation::SRL:     return os << "srl";
        case Operation::SRLI:    return os << "srli";
        case Operation::SUB:     return os << "sub";
        case Operation::SW:      return os << "sw";
        case Operation::XOR:     return os << "xor";
        case Operation::XORI:    return os << "xori";
        case Operation::UNKNOWN: return os << "N/A";
        default:                 return os << "Unknown";
    }
}

const int RD_EMPTY = -1;
const int RS1_EMPTY = -1;
const int RS2_EMPTY = -1;
const int FUNCT3_EMPTY = 0;
const int FUNCT7_EMPTY = 0;
const int IMM_EMPTY = -999999999; // Very small number to show that the instruction object isn't given an [expectedly] proper immediate

const int ERROR_CODE = -1;

// Encapsulates the data of the instruction as an object.
class Instruction {
    /* 
    Risc-V Instruction Bit Alignment
    
    [0, 6]
    [7, 11]
    [12, 14]
    [15, 19]
    [20, 24]
    [25, 31]

    Keeping the instructions as one object instead of multiple subclasses of Instruction is ideal for this
    case due to computational overhead related to subclass hierarchies. 

    The one-class design is better suited for this case because machine instructions are "known at runtime,"
    and the alignment of fields across different formats match. For example, the immediate field of I-type
    can be represented as the concatenation of R-type fields rs2 and funct7 (funct7 || rs2).

    */

    Format format;
    Operation operation;
    int rd;
    int rs1;
    int rs2;
    int funct3;
    int funct7;
    int imm;

    int rs1_value;
    int rs2_value;

    // Print R-Type fields.
    void print_R_Format(){
        std::cout << "Instruction Type: " << getFormat() << std::endl;
        std::cout << "Operation: " << getOperation() << std::endl;
        std::cout << "Rs1: x" << getRs1() << std::endl;
        std::cout << "Rs2: x" << getRs2() << std::endl;
        std::cout << "Rd: x" << getRd() << std::endl;
        std::cout << "Funct3: " << getFunct3() << std::endl;
        std::cout << "Funct7: " << getFunct7() << std::endl;
    }

    // Print SB-type fields.
    void print_SB_Format(){
        std::cout << "Instruction Type: " << getFormat() << std::endl;
        std::cout << "Operation: " << getOperation() << std::endl;
        std::cout << "Rs1: x" << getRs1() << std::endl;
        std::cout << "Rs2: x" << getRs2() << std::endl;
        std::cout << "Immediate: " << getImm() << std::endl;
    }

    // Print I-Type fields.
    void print_I_Format(){
        std::cout << "Instruction Type: " << getFormat() << std::endl;
        std::cout << "Operation: " << getOperation() << std::endl;
        std::cout << "Rs1: x" << getRs1() << std::endl;
        std::cout << "Rd: x" << getRd() << std::endl;
        std::cout << "Immediate: " << getImm() << std::endl;
    }

    // Print S-Type fields.
    void print_S_Format(){
        std::cout << "Instruction Type: " << getFormat() << std::endl;
        std::cout << "Operation: " << getOperation() << std::endl;
        std::cout << "Rs1: x" << getRs1() << std::endl;
        std::cout << "Rs2: x" << getRs2() << std::endl;
        std::cout << "Immediate: " << getImm() << std::endl;
    }

    // Print UJ-Type fields.
    void print_UJ_Format(){
        std::cout << "Instruction Type: " << getFormat() << std::endl;
        std::cout << "Operation: " << getOperation() << std::endl;
        std::cout << "Rd: x" << getRd() << std::endl;
        std::cout << "Immediate: " << getImm() << std::endl;
    }

    // Print unknown to show undefined format or bug.
    void print_UNKNOWN_Format(){
        std::cout << "Unknown Instruction Format. Not implemented or misread?" << std::endl;
    }

    public:
    // Default Constructor.
    Instruction(){
        this->format = Format::UNKNOWN;
        this->operation = Operation::UNKNOWN;
        this->rd = RD_EMPTY;
        this->rs1 = RS1_EMPTY;
        this->rs2 = RS2_EMPTY;
        this->funct3 = FUNCT3_EMPTY;
        this->funct7 = FUNCT7_EMPTY;
        this->imm = IMM_EMPTY;

        this->rs1_value = IMM_EMPTY;
        this->rs2_value = IMM_EMPTY;
    }

    // Parameterized Constructor.
    Instruction(Format format, Operation operation, int rd, int rs1, int rs2, int funct3, int funct7, int imm){
        this->format = format;
        this->operation = operation;
        this->rd = rd;
        this->rs1 = rs1;
        this->rs2 = rs2;
        this->funct3 = funct3;
        this->funct7 = funct7;
        this->imm = imm;

        this->rs1_value = IMM_EMPTY;
        this->rs2_value = IMM_EMPTY;
    }

    // Getters.
    Format getFormat(){ return this->format; }
    Operation getOperation(){ return this->operation; }
    int getRd(){ return this->rd; }
    int getRs1(){ return this->rs1; }
    int getRs2(){ return this->rs2; }
    int getFunct3(){ return this->funct3; }
    int getFunct7(){ return this->funct7; }
    int getImm(){ return this->imm; }

    // Setters.
    void setRs1Value(int value){ this->rs1_value = value; }
    void setRs2Value(int value){ this->rs2_value = value; }


    // Print fields according to type.
    void printInfo(){
        Format type = getFormat();

        // Optimization using switch case (though negligible).
        switch(type){
            case Format::R:  print_R_Format(); return;
            case Format::SB: print_SB_Format(); return;
            case Format::I:  print_I_Format(); return;
            case Format::S:  print_S_Format(); return;
            case Format::UJ: print_UJ_Format(); return;
            default:         print_UNKNOWN_Format(); return;
        }
    }
};

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
    Parameter instruction:      A 32-bit unsigned integer representing machine code.
    Parameter start:            An integer value representing which bit to start from.
    Parameter end:              An integer value representing which bit to end with.
    
    Returns:                    An unsigned 32-bit integer with lower bits of range [start, end]

    Description: Bit extraction function. [Range of bits] (inclusive) -> Range of bits starting from 0 bits (Return value).
*/
uint32_t extractBits(uint32_t instruction, int start, int end){
    uint32_t rightShiftedNumber = instruction >> start; // Shifts the bits of the instruction by the amount of the starting position.
                                                        // This brings the starting position bit down to the least-most significant bit.

    uint32_t mask = 1;
    uint32_t width = (end - start + 1);
    uint32_t bitMask = (mask << width) - 1;     // Ex. Width 5 -> (0b1 shift left 5: 0b100000). 0b100000 - 0b1 = 0b011111
                                                // Bitwise-AND of the five bits and 0b011111 preserves the five least-significant bits
    return (rightShiftedNumber & bitMask);      // and removes the rest (zeroes out).
}

/*
    Parameter instruction:          A 32-bit unsigned integer representing machine code.
    
    Returns:                       An enumerated value representing the machine code's format.
*/
Format getFormat(uint32_t instruction) {
    uint32_t opcode = extractBits(instruction, 0, 6);
    
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
    Parameter instruction:          A 32-bit unsigned integer representing machine code.
    
    Returns:                        An instruction object of R format, and its fields.
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
    Parameter value:    A 32-bit integer.
    Parameter bits:     Integer amount of bits to read from.
    
    Returns:            A 32-bit integer where the bits of parameter "value" is read in two's complement.
*/
int32_t twos_complement(int32_t value, int bits){
    if (value & (1 << (bits - 1))){     // Check if the sign-bit (most-sig bit) is one.
        value -= (1 << bits);           // If so, subtract that next power of 2.
    }

    return value;
}

/*
    Parameter instruction:          A 32-bit unsigned integer representing machine code.
    
    Returns:                        An instruction object of SB format, and its fields.
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

    int immediate = (int)twos_complement(raw_immediate, 12);    // Reread the reconstructed immediate as a signed value in 2's complement.
                                                                // There are twelve bits of immediate.

    // Build the instruction object.
    return Instruction(Format::SB, operation, RD_EMPTY, (int)rs1, (int)rs2, (int)funct3, FUNCT7_EMPTY, immediate);
}

/*
    Parameter instruction:          A 32-bit unsigned integer representing machine code.
    
    Returns:                        An instruction object of I format, and its fields.
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

    int immediate = (int)twos_complement(imm_11_to_0, 12);

    return Instruction(Format::I, operation, (int)rd, (int)rs1, RS2_EMPTY, (int)funct3, (int)funct7, immediate);
}

/*
    Parameter instruction:          A 32-bit integer unsigned representing machine code.
    
    Returns:                        An instruction object of S format, and its fields.
*/
Instruction decode_S(uint32_t instruction){
    uint32_t opcode = extractBits(instruction, 0, 6);
    uint32_t funct3 = extractBits(instruction, 12, 14);
    uint32_t rs1 = extractBits(instruction, 15, 19);
    uint32_t rs2 = extractBits(instruction, 20, 24);
    uint32_t imm_11_to_5 = extractBits(instruction, 25, 31);
    uint32_t imm_4_to_0  = extractBits(instruction, 7, 11);

    uint32_t raw_immediate = (imm_11_to_5 << 5) | imm_4_to_0;

    int immediate = (int)twos_complement(raw_immediate, 12);

    uint32_t key = makeFormatKey(opcode, funct3, FUNCT7_EMPTY);

    Operation operation = S_Format_Map.at(key);

    return Instruction(Format::S,operation,RD_EMPTY,(int)rs1,(int)rs2,(int)funct3,FUNCT7_EMPTY,immediate);
}

/*
    Parameter instruction:          A 32-bit unsigned integer representing machine code.
    
    Returns:                        An instruction object of UJ format, and its fields.
*/
Instruction decode_UJ(uint32_t instruction){
    uint32_t opcode = extractBits(instruction, 0, 6);
    Operation operation = UJ_Format_Map.at(makeFormatKey(opcode, FUNCT3_EMPTY, FUNCT7_EMPTY));
    uint32_t rd = extractBits(instruction, 7, 11);

    // uint32_t imm_20_and_10_to_1_and_11_and_19_to_12 = extractBits(instruction, 12, 31); // 20 bits.

    uint32_t twentiethBit = extractBits(instruction, 31, 31);
    uint32_t imm_10_to_1bit = extractBits(instruction, 21, 30);
    uint32_t eleventhBit = extractBits(instruction, 20, 20);
    uint32_t imm_19_to_12 = extractBits(instruction, 12, 19);

    uint32_t raw_immediate = (twentiethBit << 20) | (imm_10_to_1bit << 1) | (eleventhBit << 11) |(imm_19_to_12 << 12); 

    int32_t immediate = twos_complement(raw_immediate, 21); // 20 bits of immediate, but we need to account for the sign bit as well.

    return Instruction(Format::UJ, operation, (int)rd, RS1_EMPTY, RS2_EMPTY, FUNCT3_EMPTY, FUNCT7_EMPTY, (int)immediate);
}

int32_t signExtend(uint32_t bits, int originalBits){
    uint32_t leftShifted = bits << (32 - originalBits);

    return static_cast<int32_t>(leftShifted) >> (32 - originalBits); 
}

/*
    Parameter instruction:          A 32-bit unsigned integer representing machine code.
    Parameter registerFile[32]:     A read-only reference to the register file.
    
    Returns:                        An instruction object including its format type, fields, and their values.
*/
Instruction decode(uint32_t instruction, const int registerFile[32]) {
    Instruction decodedInstruction = Instruction();

    Format type = getFormat(instruction);                       // Get instruction format.

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
            break;

        case Format::I:   
            decodedInstruction = decode_I(instruction); 
            decodedInstruction.setRs1Value(registerFile[decodedInstruction.getRs1()]);
            break;

        case Format::S:     
            decodedInstruction = decode_S(instruction); 
            decodedInstruction.setRs1Value(registerFile[decodedInstruction.getRs1()]);
            decodedInstruction.setRs2Value(registerFile[decodedInstruction.getRs2()]);
            break;

        case Format::UJ:   
            decodedInstruction = decode_UJ(instruction); break;

        default:
            break;
    }

    return decodedInstruction;
}

#endif
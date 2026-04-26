#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <iostream>

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

// Defaults to indicate unused and misused fields.
const int RD_EMPTY = -1;
const int RS1_EMPTY = -1;
const int RS2_EMPTY = -1;
const int FUNCT3_EMPTY = 0;
const int FUNCT7_EMPTY = 0;
const int IMM_EMPTY = -999999999;   // Very small number to show that the instruction object isn't given an [expectedly] proper immediate

const int I_TYPE_IMM_BITS = 12;     // Amount of bits of immediate in I-type instructions.
const int S_SB_TYPE_IMM_BITS = 12;  // Amoung ot bits of immediate in S- and SB-type instructions.
const int UJ_TYPE_IMM_BITS = 21;

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
    */

    Format format;
    Operation operation;
    int rd;
    int rs1;
    int rs2;
    int funct3;
    int funct7;
    int imm;

    /*
        Actual values of source register fields. Currently, they are not intended 
        to be constructor-initialized, but are retrieved by the decoder function
        manually.
    */
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
    /* 
        Default Constructor.
        Initializes fields as all "empty"
    */
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

    /*
        Parameterized Constructor.
        All instruction fields (not source register values) are expected to be
        provided an input upon initialization. 
        Any unused fields should be provided with "empty" constants as in
        default constructor.
    */
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
    int getRs1Value(){ return this->rs1_value; }
    int getRs2Value(){ return this->rs2_value; }

    // Setters.
    void setRs1Value(int value){ this->rs1_value = value; }
    void setRs2Value(int value){ this->rs2_value = value; }
    void setImm(int32_t value){ this ->imm = value; }


    // Print fields according to type.
    void printInfo(){
        Format type = getFormat();

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

#endif
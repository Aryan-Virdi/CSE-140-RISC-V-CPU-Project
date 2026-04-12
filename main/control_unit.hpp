#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <cstdint>

const int TRUE = 1;
const int FALSE = 0;
const int UNKNOWN = -1;

// Provides an interface for the control signals.
// Will have to update to handle additional signal(s) for jal and jalr in the future.
class IControl{
    int *ctrlArray[7];

    enum ControlIndex : int {
        regWrIdx = 0,
        branchIdx = 1,
        ALUSrcIdx = 2,
        memWrIdx = 3,
        memToRegIdx = 4,
        memRdIdx = 5,
        ALUOpIdx = 6
    };

    public:
    IControl(int* regWr, int* branch, int* ALUSrc, int* memWr, int* memToReg, int* memRd, int* ALUOp){
        ctrlArray[ControlIndex::regWrIdx]     = regWr;
        ctrlArray[ControlIndex::branchIdx]    = branch;
        ctrlArray[ControlIndex::ALUSrcIdx]    = ALUSrc;
        ctrlArray[ControlIndex::memWrIdx]     = memWr;
        ctrlArray[ControlIndex::memToRegIdx]  = memToReg;
        ctrlArray[ControlIndex::memRdIdx]     = memRd;
        ctrlArray[ControlIndex::ALUOpIdx]     = ALUOp;
    }

    void updateRegWr(int value)   { *ctrlArray[ControlIndex::regWrIdx] = value;    }
    void updateBranch(int value)  { *ctrlArray[ControlIndex::branchIdx] = value;   }
    void updateALUSrc(int value)  { *ctrlArray[ControlIndex::ALUSrcIdx] = value;   }
    void updateMemWr(int value)   { *ctrlArray[ControlIndex::memWrIdx] = value;    }
    void updateMemToReg(int value){ *ctrlArray[ControlIndex::memToRegIdx] = value; }
    void updateMemRd(int value)   { *ctrlArray[ControlIndex::memRdIdx] = value;    }
    void updateALUOp(int value)   { *ctrlArray[ControlIndex::ALUOpIdx] = value;    }

    void updateAllSignals(int regWr, int branch, int ALUSrc, int ALUOp, int memWr, int memToReg, int memRd){
        updateRegWr(regWr);
        updateBranch(branch);
        updateALUSrc(ALUSrc);
        updateALUOp(ALUOp);
        updateMemWr(memWr);
        updateMemToReg(memToReg);
        updateMemRd(memRd);
    }

    int getRegWr()    const { return *ctrlArray[ControlIndex::regWrIdx];    }
    int getBranch()   const { return *ctrlArray[ControlIndex::branchIdx];   }
    int getALUSrc()   const { return *ctrlArray[ControlIndex::ALUSrcIdx];   }
    int getMemWr()    const { return *ctrlArray[ControlIndex::memWrIdx];    }
    int getMemToReg() const { return *ctrlArray[ControlIndex::memToRegIdx]; }
    int getMemRd()    const { return *ctrlArray[ControlIndex::memRdIdx];    }
    int getALUOp()    const { return *ctrlArray[ControlIndex::ALUOpIdx];    }

};

/*
    Parameter opcode:       An unsigned 32-bit integer representing an Instruction's opcode in the lower bits.
    Parameter ctrlSignals:  A reference to an IControl object, expectedly in main.cpp.

    Note:                   ALUOp is updated as shown in the lecture slides and ZyBooks
*/
void controlUnit(uint32_t opcode, IControl& ctrlSignals){
    switch(opcode){
        case 0b0110011: 
            // R-Type
            ctrlSignals.updateAllSignals(TRUE, FALSE, FALSE, 0b10, FALSE, FALSE, FALSE);
            break;
        case 0b0010011: 
            // Generic I-type
            ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, 0b10, FALSE, FALSE, FALSE);
            break;
        case 0b0000011: 
            // lw instruction
            ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, 0b00, FALSE, TRUE, TRUE);
            break;
        // case 0b1100111:
            // jalr instruction
            // break;
        case 0b1100011: 
            // SB-type
            ctrlSignals.updateRegWr(FALSE);
            ctrlSignals.updateBranch(TRUE);
            ctrlSignals.updateALUSrc(FALSE);
            ctrlSignals.updateALUOp(0b01);
            ctrlSignals.updateMemWr(FALSE);
            // "Don't care about memToReg; ignore.
            ctrlSignals.updateMemRd(FALSE);
            break;
        case 0b0100011: 
            // S-Type (sw)
            ctrlSignals.updateAllSignals(FALSE, FALSE, TRUE, 0b00, TRUE, FALSE, FALSE);
            break;
    }

  
}

/*
    Parameter ALUOp:    An integer representing the generated ALU control signal.
                        Expected to be provided from global value (main.cpp)
    Parameter funct3:   An integer representing the funct3 field.
    Parameter funct7:   An integer representing the funct7 field.

    Note:               funct3/7 fields are treated as unsigned 32-bit integers
                        elsewhere, until they are stored in an Instruction object's
                        fields. This type "promotion" is reflected in these parameters.

    Note:               This logic flow depends on the Instruction object providing
                        an "empty" funct7 field for I-type instructions.
*/
int aluControl(int ALUOp, int funct3, int funct7){
    switch(ALUOp){
        case 0b00: return 0b0010;   // Add (lw/sw)
        case 0b01: return 0b0110;   // Subtract (beq)
        case 0b10:
            switch(funct3){
                case 0b000:
                    switch(funct7){
                        case 0b0000000: return 0b0010;  // Add
                        case 0b0100000: return 0b0110;  // Subtract
                        default: return UNKNOWN;
                    }
                case 0b110:
                    switch(funct7){
                        case 0b0000000: return 0b0001;  // Logical OR
                        default: return UNKNOWN;
                    }
                case 0b111:
                    switch(funct7){
                        case 0b0000000: return 0b0000;  // Logical AND
                        default: return UNKNOWN;
                    }
                default: return UNKNOWN;
            }
        default: return UNKNOWN;
    }
}

#endif

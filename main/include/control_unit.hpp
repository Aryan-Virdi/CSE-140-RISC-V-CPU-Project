#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <cstdint>

const int TRUE = 1;
const int FALSE = 0;
const int UNKNOWN = -1;

// Provides an interface for the control signals.
// Will have to update to handle additional signal(s) for jal and jalr in the future.
class IControl{
    int* ctrlArray[9];

    enum ControlIndex : int {
        regWrIdx = 0,
        branchIdx = 1,
        ALUSrcIdx = 2,
        memWrIdx = 3,
        memToRegIdx = 4,
        memRdIdx = 5,
        ALUOpIdx = 6,
        jumpIdx = 7,
        ALUSrc2Idx = 8
    };

    public:
    IControl(int* regWr, int* branch, int* ALUSrc, int* memWr, int* memToReg, int* memRd, int* ALUOp, int* jump, int* ALUSrc2){
        ctrlArray[ControlIndex::regWrIdx]     = regWr;
        ctrlArray[ControlIndex::branchIdx]    = branch;
        ctrlArray[ControlIndex::ALUSrcIdx]    = ALUSrc;
        ctrlArray[ControlIndex::memWrIdx]     = memWr;
        ctrlArray[ControlIndex::memToRegIdx]  = memToReg;
        ctrlArray[ControlIndex::memRdIdx]     = memRd;
        ctrlArray[ControlIndex::ALUOpIdx]     = ALUOp;
        ctrlArray[ControlIndex::jumpIdx]      = jump;
        ctrlArray[ControlIndex::ALUSrc2Idx]   = ALUSrc2;
    }

    void updateRegWr(int value)   { *ctrlArray[ControlIndex::regWrIdx] = value;    }
    void updateBranch(int value)  { *ctrlArray[ControlIndex::branchIdx] = value;   }
    void updateALUSrc(int value)  { *ctrlArray[ControlIndex::ALUSrcIdx] = value;   }
    void updateMemWr(int value)   { *ctrlArray[ControlIndex::memWrIdx] = value;    }
    void updateMemToReg(int value){ *ctrlArray[ControlIndex::memToRegIdx] = value; }
    void updateMemRd(int value)   { *ctrlArray[ControlIndex::memRdIdx] = value;    }
    void updateALUOp(int value)   { *ctrlArray[ControlIndex::ALUOpIdx] = value;    }
    void updateJump(int value)    { *ctrlArray[ControlIndex::jumpIdx] = value;     }
    void updateALUSrc2(int value) { *ctrlArray[ControlIndex::ALUSrc2Idx] = value;  } 

    void updateAllSignals(int regWr, int branch, int ALUSrc, int ALUOp, int memWr, int memToReg, int memRd, int jump, int ALUSrc2){
        updateRegWr(regWr);
        updateBranch(branch);
        updateALUSrc(ALUSrc);
        updateALUOp(ALUOp);
        updateMemWr(memWr);
        updateMemToReg(memToReg);
        updateMemRd(memRd);
        updateJump(jump);
        updateALUSrc2(ALUSrc2);
    }

    int getRegWr()    const { return *ctrlArray[ControlIndex::regWrIdx];    }
    int getBranch()   const { return *ctrlArray[ControlIndex::branchIdx];   }
    int getALUSrc()   const { return *ctrlArray[ControlIndex::ALUSrcIdx];   }
    int getMemWr()    const { return *ctrlArray[ControlIndex::memWrIdx];    }
    int getMemToReg() const { return *ctrlArray[ControlIndex::memToRegIdx]; }
    int getMemRd()    const { return *ctrlArray[ControlIndex::memRdIdx];    }
    int getALUOp()    const { return *ctrlArray[ControlIndex::ALUOpIdx];    }
    int getJump()     const { return *ctrlArray[ControlIndex::jumpIdx];     }
    int getALUSrc2()  const { return *ctrlArray[ControlIndex::ALUSrc2Idx];  }

};

/*
    Parameter opcode:       An unsigned 32-bit integer representing an Instruction's opcode in the lower bits.
    Parameter ctrlSignals:  A reference to an IControl object, expectedly in main.cpp.

    Note:                   ALUOp is updated as shown in the lecture slides and ZyBooks

    Note:                   Jal and Jalr have similar but distinct datapaths. In the future, we may want to implement
                            these instructions by using the existing Branch control signal instead of making a new,
                            unique "jump" signal.

                            Both write PC+4 to rd.  Jal adds imm offset of UJ-type to PC
                                                    Jalr adds imm offset of I-type to its rs1 for branch target.
                                                    So, we can use PC+imm or Jalr-target as branch_target with "branch" set to true.
*/
void controlUnit(uint32_t opcode, IControl& ctrlSignals){
    switch(opcode){
        case 0b0110011: 
            // R-Type
            ctrlSignals.updateAllSignals(TRUE, FALSE, FALSE, 0b10, FALSE, FALSE, FALSE, FALSE, FALSE);
            break;
        case 0b0010011: 
            // Generic I-type
            ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, 0b10, FALSE, FALSE, FALSE, FALSE, FALSE);
            break;
        case 0b0000011: 
            // lw instruction (I-type)
            ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, 0b00, FALSE, TRUE, TRUE, FALSE, FALSE);
            break;
        case 0b1101111:
            // UJ-Type; jal
            ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, 0b00, FALSE, FALSE, FALSE, TRUE, TRUE);
        case 0b1100111:
            // jalr instruction (I-type)
            ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, 0b00, FALSE, FALSE, FALSE, TRUE, TRUE);
            break;
        case 0b1100011: 
            // SB-type
            ctrlSignals.updateRegWr(FALSE);
            ctrlSignals.updateBranch(TRUE);
            ctrlSignals.updateALUSrc(FALSE);
            ctrlSignals.updateALUOp(0b01);
            ctrlSignals.updateMemWr(FALSE);
            // "Don't care about memToReg; ignore.
            ctrlSignals.updateMemRd(FALSE);
            ctrlSignals.updateJump(FALSE);
            ctrlSignals.updateALUSrc2(FALSE);
            break;
        case 0b0100011: 
            // S-Type (sw)
            ctrlSignals.updateAllSignals(FALSE, FALSE, TRUE, 0b00, TRUE, FALSE, FALSE, FALSE, FALSE);
            break;
    }

  
}

/*
    Parameter ALUOp:    An integer representing the generated ALU control signal.
                        Expected to be provided from global value (main.cpp)
    Parameter funct3:   An integer representing the funct3 field.
    Parameter funct7:   An integer representing the funct7 field.

    Returns:            An integer value representing the control signals for the
                        ALU.

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
        case 0b10:                  // R-Type ALUOp
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
        case 0b11: return 0b0011;
        default: return UNKNOWN;
    }
}

#endif

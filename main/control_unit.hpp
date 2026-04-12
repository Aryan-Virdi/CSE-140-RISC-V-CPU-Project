#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <cstdint>

const int TRUE = 1;
const int FALSE = 0;

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
    Parameter opcdoe:       An unsigned 32-bit integer representing an Instruction's opcode in the lower bits.
    Parameter ctrlSignals:  A reference to an IControl object, expectedly in main.cpp.

    Note:                   ALUOp is updated as shown in the lecture slides and ZyBooks sections except in one
                            aspect. Here, it is assumed that general immediate type instructions have an
                            ALUOp of 0b11, or 3 in decimal.
*/
void controlUnit(uint32_t opcode, IControl& ctrlSignals){
    switch(opcode){
        case 0b0110011: 
            // R-Type
            ctrlSignals.updateAllSignals(TRUE, FALSE, FALSE, 0b10, FALSE, FALSE, FALSE);
            break;
        case 0b0010011: 
            // Generic I-type
            ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, 0b11, FALSE, FALSE, FALSE);
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

int aluControl(int ALUOp, uint32_t funct3, uint32_t funct7){
    int opValue = 0;    // Placeholder body
    return opValue;
}

#endif

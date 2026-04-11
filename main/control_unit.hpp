#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <cstdint>

const int TRUE = 1;
const int FALSE = 1;

// Provides an interface for the control signals.
class IControl{
    int *ctrlArray[6];

    enum ControlIndex {
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

int aluControl(int ALUOp, uint32_t funct3, uint32_t funct7){
    int opValue = 0;    // Placeholder body
    return opValue;
}

/*
    Parameter instruction:  An instruction object. This object is expected to be populated.
    Parameter ctrlSignals:  An IControl interface that packs the addresses to global control signals
                            into one object.

    Note:                   Parameter ctrlSignals is expected to be instantiated in main.cpp and passed into
                            decoder.hpp.
*/
void controlUnit(uint32_t opcode, uint32_t funct3, uint32_t funct7, IControl& ctrlSignals){
    switch(opcode){
        case 0b0110011: // R-Type
            ctrlSignals.updateAllSignals(TRUE, FALSE, FALSE, 0b10, FALSE, FALSE, FALSE);
            break;
        // case 0b0010011: // Generic I-type
        //     ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, aluControl(funct3, funct7), FALSE, FALSE, FALSE);
        //     break;
        case 0b0000011: // lw instruction
            ctrlSignals.updateAllSignals(TRUE, FALSE, TRUE, 0b00, FALSE, TRUE, TRUE);
            break;
        case 0b1100011: // SB-type
            ctrlSignals.updateRegWr(FALSE);
            ctrlSignals.updateBranch(TRUE);
            ctrlSignals.updateALUSrc(FALSE);
            ctrlSignals.updateALUOp(0b01);
            ctrlSignals.updateMemWr(FALSE);
            // "Don't care about memToReg"
            ctrlSignals.updateMemRd(FALSE);
            break;
        case 0b0100011: // S-Type (sw)
            ctrlSignals.updateAllSignals(FALSE, FALSE, TRUE, 0b00, TRUE, FALSE, FALSE);
    }

  
}

#endif
#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <cstdint>

// Provides an interface for the control signals.
class IControl{
    int *ctrlArray[6];

    enum ControlIndex {
        regWrIdx = 0,
        branchIdx = 1,
        ALUSrcIdx = 2,
        memWrIdx = 3,
        memToRegIdx = 4,
        memRdIdx = 5
    };

    public:
    IControl(int* regWr, int* branch, int* ALUSrc, int* memWr, int* memToReg, int* memRd){
        ctrlArray[ControlIndex::regWrIdx]     = regWr;
        ctrlArray[ControlIndex::branchIdx]    = branch;
        ctrlArray[ControlIndex::ALUSrcIdx]    = ALUSrc;
        ctrlArray[ControlIndex::memWrIdx]     = memWr;
        ctrlArray[ControlIndex::memToRegIdx]  = memToReg;
        ctrlArray[ControlIndex::memRdIdx]     = memRd;
    }

    void updateRegWr(int value)   { *ctrlArray[ControlIndex::regWrIdx] = value;    }
    void updateBranch(int value)  { *ctrlArray[ControlIndex::branchIdx] = value;   }
    void updateALUSrc(int value)  { *ctrlArray[ControlIndex::ALUSrcIdx] = value;   }
    void updateMemWr(int value)   { *ctrlArray[ControlIndex::memWrIdx] = value;    }
    void updateMemToReg(int value){ *ctrlArray[ControlIndex::memToRegIdx] = value; }
    void updateMemRd(int value)   { *ctrlArray[ControlIndex::memRdIdx] = value;    }

    int getRegWr()    const { return *ctrlArray[ControlIndex::regWrIdx];    }
    int getBranch()   const { return *ctrlArray[ControlIndex::branchIdx];   }
    int getALUSrc()   const { return *ctrlArray[ControlIndex::ALUSrcIdx];   }
    int getMemWr()    const { return *ctrlArray[ControlIndex::memWrIdx];    }
    int getMemToReg() const { return *ctrlArray[ControlIndex::memToRegIdx]; }
    int getMemRd()    const { return *ctrlArray[ControlIndex::memRdIdx];    }

};

/*
    Parameter opcode:       An unsigned integer of 32 bits that represents the opcode of an instruction.
    Parameter ctrlSignals:  An IControl interface that packs the addresses to global control signals
                            into one object.

    Note:                   Parameter ctrlSignals is expected to be instantiated in main.cpp and passed into
                            decoder.hpp.
*/
void controlUnit(uint32_t opcode, IControl ctrlSignals){
    // Needs to generate ctrl signals using opcode. May have to add funct fields as params.
    
}

#endif
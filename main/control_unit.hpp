#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

// Provides an interface for the control signals.
class IControl{
    int *ctrlArray[6];

    const enum ControlIndex {
        regWrIdx = 1,
        branchIdx = 2,
        ALUSrcIdx = 3,
        memWrIdx = 4,
        memToRegIdx = 5,
        memRdIdx = 6
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

};

#endif
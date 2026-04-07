#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

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

};

#endif
#ifndef PIPELINE_REGISTERS_HPP
#define PIPELINE_REGISTERS_HPP

class PipelineRegister {
    
};

class IF_ID{
    int regWr;
    int branch;
    int aluSrc;
    int aluOp;
    int memWr;
    int memToReg;
    int memRd;
    int jump;

    void updateRegWr(int value)   { this->regWr = value;    }
    void updateBranch(int value)  { this->branch = value;   }
    void updateALUSrc(int value)  { this->aluSrc = value;   }
    void updateMemWr(int value)   { this->memWr = value;    }
    void updateMemToReg(int value){ this->memToReg = value; }
    void updateMemRd(int value)   { this->memRd = value;    }
    void updateALUOp(int value)   { this->aluOp = value;    }
    void updateJump(int value)    { this->jump = value;     }

    public:
    IF_ID(){}

    void updateAllSignals(int regWr, int branch, int ALUSrc, int ALUOp, int memWr, int memToReg, int memRd, int jump){
        updateRegWr(regWr);
        updateBranch(branch);
        updateALUSrc(ALUSrc);
        updateALUOp(ALUOp);
        updateMemWr(memWr);
        updateMemToReg(memToReg);
        updateMemRd(memRd);
        updateJump(jump);
    }

    int getRegWr()    const { return this->regWr;     }
    int getBranch()   const { return this->branch;    }
    int getALUSrc()   const { return this->aluSrc;    }
    int getMemWr()    const { return this->memWr;     }
    int getMemToReg() const { return this-> memToReg; }
    int getMemRd()    const { return this->memRd;     }
    int getALUOp()    const { return this-> aluOp;    }
};

class ID_EXE{};

class EXE_MEM{};

class MEM_WB{};

#endif
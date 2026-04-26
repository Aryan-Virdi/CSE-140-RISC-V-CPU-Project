#ifndef PIPELINE_REGISTERS_HPP
#define PIPELINE_REGISTERS_HPP

#include <cstdint>
#include "../control_unit.hpp"

/*
    The four pipeline registers, responsible for holding intermediate
    information and carrying relevant instruction info forwards.

    Pipeline registers are defined as shown in 
    Lecture-4_Processor-4, slides 4-5.
*/


/*
    At the beginning of the datapaths, just after instruction fetch.
    Input will be read and then decoded.
*/
class IF_ID {
    // Current program counter
    int PC = 0;

    // Machine code instruction
    uint32_t instruction = 0;

    void updatePC(int value)         { this->PC = value;         } 
    void updateInstr(uint32_t value){ this->instruction = value; }

    public:
    IF_ID(){}

    void updateInfo(int PC, uint32_t instruction){
        updatePC(PC);
        updateInstr(instruction);
    }

    int getPC()         const { return this->PC;          }
    uint32_t getInstr() const { return this->instruction; }
};

/*
    After decoding, the control signals will be placed
    into this buffer. Other decoding results will be placed
    into this buffer before execution as well.

    Note that rs2 is saved regardless of operand 2's source.
*/

class ID_EXE {
    // Previously generated ontrol signals
    int regWr = 0;
    int branch = 0;
    int aluSrc = 0;
    int aluOp = 0;
    int memWr = 0;
    int memToReg = 0;
    int memRd = 0;
    int jump = 0;

    // Operands from register file.
    int operand1 = 0;
    int operand2 = 0;
    int signExtImm = 0;

    // Relvant registers
    int rs2 = 0;
    int rd = 0;

    void updateRegWr(int value)     { this->regWr = value;       }
    void updateBranch(int value)    { this->branch = value;      }
    void updateALUSrc(int value)    { this->aluSrc = value;      }
    void updateMemWr(int value)     { this->memWr = value;       }
    void updateMemToReg(int value)  { this->memToReg = value;    }
    void updateMemRd(int value)     { this->memRd = value;       }
    void updateALUOp(int value)     { this->aluOp = value;       }
    void updateJump(int value)      { this->jump = value;        }
    void updateRs2(int value)       { this->rs2 = value;         }
    void updateRd(int value)        { this->rd = value;          }
    void updateOp1(int value)       { this->operand1 = value;    }
    void updateOp2(int value)       { this->operand2 = value;    }
    void updateSignExtImm(int value){ this->signExtImm = value;  }

    public:
    ID_EXE(){}

    void updateInfo(IF_ID if_id_reg, IControl controlUnit, int rs2, int rd, int operand1, int operand2, int immediate){
        updateRegWr(controlUnit.getRegWr());
        updateBranch(controlUnit.getBranch());
        updateALUSrc(controlUnit.getALUSrc());
        updateALUOp(controlUnit.getALUOp());
        updateMemWr(controlUnit.getMemWr());
        updateMemToReg(controlUnit.getMemToReg());
        updateMemRd(controlUnit.getMemRd());
        // updateJump(controlUnit.getJump());
        updateRs2(rs2);
        updateRd(rd);
        updateOp1(operand1);
        updateOp2(operand2);
        updateSignExtImm(immediate);
    }

    int getRegWr()      const { return this->regWr;       }
    int getBranch()     const { return this->branch;      }
    int getALUSrc()     const { return this->aluSrc;      }
    int getMemWr()      const { return this->memWr;       }
    int getMemToReg()   const { return this-> memToReg;   }
    int getMemRd()      const { return this->memRd;       }
    int getALUOp()      const { return this->aluOp;       }
    int getJump()       const { return this->jump;        }
    int getRs2()        const {return this->rs2;          }
    int getRd()         const { return this->rd;          }
    int getOp1()        const { return this->operand1;    }
    int getOp2()        const { return this->operand2;    }
    int getImm()        const { return this->signExtImm;  }
};

/*
    All results of execution are placed into this pipline
    register. Branch target is stored here, so it will be
    read and used in EXE stage.
*/

class EXE_MEM {
    // Previously generated control signals
    int regWr = 0;
    int branch = 0;
    int memWr = 0;
    int memToReg = 0;
    int memRd = 0;
    int jump = 0;

    // Relevant registers
    int rs2 = 0;
    int rd = 0;

    // ALU results
    int aluZero = 0;
    int aluResult = 0;

    int branchTarget = 0;

    void updateRegWr(int value)        { this->regWr = value;        }
    void updateBranch(int value)       { this->branch = value;       }
    void updateMemWr(int value)        { this->memWr = value;        }
    void updateMemToReg(int value)     { this->memToReg = value;     }
    void updateMemRd(int value)        { this->memRd = value;        }
    void updateJump(int value)         { this->jump = value;         }
    void updateRs2(int value)          { this->rs2 = value;          }
    void updateRd(int value)           { this->rd = value;           }
    void updateALUZero(int value)      { this->aluZero = value;      }
    void updateALUResult(int value)    { this->aluResult = value;    }
    void updateBranchTarget(int value) { this->branchTarget = value; }

    public:
    EXE_MEM(){}

    void updateInfo(ID_EXE id_exe_reg, int aluZero, int aluResult, int branchTarget){
        updateRegWr(id_exe_reg.getRegWr());
        updateBranch(id_exe_reg.getBranch());
        updateMemWr(id_exe_reg.getMemWr());
        updateMemToReg(id_exe_reg.getMemToReg());
        updateMemRd(id_exe_reg.getMemRd());
        updateJump(id_exe_reg.getJump());
        updateRs2(id_exe_reg.getRs2());
        updateRd(id_exe_reg.getRd());
        updateALUZero(aluZero);
        updateALUResult(aluResult);
        updateBranchTarget(branchTarget);
    }

    int getRegWr()        const { return this->regWr;        }
    int getBranch()       const { return this->branch;       }
    int getMemWr()        const { return this->memWr;        }
    int getMemToReg()     const { return this-> memToReg;    }
    int getMemRd()        const { return this->memRd;        }
    int getRs2()          const {return this->rs2;           }
    int getRd()           const { return this->rd;           }
    int getALUZero()      const { return this->aluZero;      }
    int getALUResult()    const { return this->aluResult;    }
    int getBranchTarget() const { return this->branchTarget; }
};

/*
    The final pipeline register. The data regarding writes
    is stored here and then used for WB stage.
*/

class MEM_WB {
    // Previously generated control signals
    int regWr = 0;
    int memToReg = 0;

    // Destination register
    int rd = 0;

    // ALU result
    int aluResult = 0;

    // MEM result
    int memData = 0;

    void updateRegWr(int value)     { this->regWr = value;       }
    void updateMemToReg(int value)  { this->memToReg = value;    }
    void updateRd(int value)        { this->rd = value;          }
    void updateALUResult(int value) { this->aluResult = value;   }
    void updateMemData(int value)   { this->memData = value;     }

    public:
    MEM_WB(){}

    void updateInfo(EXE_MEM exe_mem_reg, int memData){
        updateRegWr(exe_mem_reg.getRegWr());
        updateMemToReg(exe_mem_reg.getMemToReg());
        updateRd(exe_mem_reg.getRd());
        updateALUResult(exe_mem_reg.getALUResult());
        updateMemData(memData);
    }

    int getRegWr()      const { return this->regWr;       }
    int getMemToReg()   const { return this-> memToReg;   }
    int getRd()         const { return this->rd;          }
    int getALUResult()  const { return this->aluResult;   }
    int getMemData()    const { return this->memData;     }
};

#endif
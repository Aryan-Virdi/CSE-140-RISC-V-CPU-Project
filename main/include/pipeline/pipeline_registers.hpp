#ifndef PIPELINE_REGISTERS_HPP
#define PIPELINE_REGISTERS_HPP

#include <cstdint>

#include "../util/bit_utils.hpp"
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
    // Flag for register having valid data (not stale)
    bool valid = false;

    // Current program counter
    int PC = 0;

    // PC+4
    int nextPC = 0;

    // Machine code instruction
    uint32_t instruction = 0;

    // Source registers
    int rs1 = 0;
    int rs2 = 0;

    void updatePC(int value)         { this->PC = value;          } 
    void updateNextPC(int value)     { this->nextPC = value;      }
    void updateInstr(uint32_t value) { this->instruction = value; }
    void updateRs1(int value)        { this->rs1 = value;         }
    void updateRs2(int value)        { this->rs2 = value;         }

    public:
    IF_ID(){}
    
    void updateValid(bool value)     { this->valid = value;       }

    void updateInfo(int PC, int nextPC, uint32_t instruction){
        updatePC(PC);
        updateNextPC(nextPC);
        updateInstr(instruction);
        updateRs1(static_cast<int>(extractBits(instruction, 15, 19)));
        updateRs2(static_cast<int>(extractBits(instruction, 20, 24)));
    }

    int getPC()         const { return this->PC;          }
    int getNextPC()     const { return this->nextPC;      }
    uint32_t getInstr() const { return this->instruction; }
    int getRs1()        const { return this->rs1;         }
    int getRs2()        const { return this->rs2;         }

    bool getValid()     const { return this->valid;       }
};

/*
    After decoding, the control signals will be placed
    into this buffer. Other decoding results will be placed
    into this buffer before execution as well.

    Note that rs2 is saved regardless of operand 2's source.
*/
class ID_EXE {
    // Flag for register having valid data (not stale)
    bool valid = false;

    // Previously generated control signals
    int regWr = 0;
    int branch = 0;
    int aluSrc = 0;
    int aluSrc2 = 0;
    int aluOp = 0;
    int memWr = 0;
    int memToReg = 0;
    int memRd = 0;
    int jump = 0;
    int PCSrc = 0;

    // PC info
    int PC = 0;
    int nextPC = 0;

    // Operands from register file.
    int operand1 = 0;
    int operand2 = 0;
    int signExtImm = 0;

    // Relvant registers
    int rs1 = 0;
    int rs2 = 0;
    int rd = 0;

    // Instruction info
    int funct3 = 0;
    int funct7 = 0;
    int rs2Value = 0;

    void updatePC(int value)        { this->PC = value;          }
    void updateNextPC(int value)    { this->nextPC = value;      }
    void updateRegWr(int value)     { this->regWr = value;       }
    void updateBranch(int value)    { this->branch = value;      }
    void updateALUSrc(int value)    { this->aluSrc = value;      }
    void updateALUSrc2(int value)   { this->aluSrc2 = value;     }
    void updateMemWr(int value)     { this->memWr = value;       }
    void updateMemToReg(int value)  { this->memToReg = value;    }
    void updateMemRd(int value)     { this->memRd = value;       }
    void updateALUOp(int value)     { this->aluOp = value;       }
    void updateJump(int value)      { this->jump = value;        }
    void updatePCSrc(int value)     { this->PCSrc = value;       }
    void updateRs1(int value)       { this->rs1 = value;         }
    void updateRs2(int value)       { this->rs2 = value;         }
    void updateRd(int value)        { this->rd = value;          }
    void updateOp1(int value)       { this->operand1 = value;    }
    void updateOp2(int value)       { this->operand2 = value;    }
    void updateSignExtImm(int value){ this->signExtImm = value;  }
    void updateFunct3(int value)    { this->funct3 = value;      }
    void updateFunct7(int value)    { this->funct7 = value;      }
    void updateRs2Value(int value)  { this->rs2Value = value;    }

    public:
    ID_EXE(){}

    void updateInfo(const IF_ID& if_id_reg, const IControl& controlUnit, int rd, int operand1, int operand2, int immediate, int funct3, int funct7, int rs2Value){
        updatePC(if_id_reg.getPC());
        updateNextPC(if_id_reg.getNextPC());
        updateRegWr(controlUnit.getRegWr());
        updateBranch(controlUnit.getBranch());
        updateALUSrc(controlUnit.getALUSrc());
        updateALUSrc2(controlUnit.getALUSrc2());
        updateALUOp(controlUnit.getALUOp());
        updateMemWr(controlUnit.getMemWr());
        updateMemToReg(controlUnit.getMemToReg());
        updateMemRd(controlUnit.getMemRd());
        updateJump(controlUnit.getJump());
        updatePCSrc(controlUnit.getPCSrc());
        updateRs1(if_id_reg.getRs1());
        updateRs2(if_id_reg.getRs2());
        updateRd(rd);
        updateOp1(operand1);
        updateOp2(operand2);
        updateSignExtImm(immediate);
        updateFunct3(funct3);
        updateFunct7(funct7);
        updateRs2Value(rs2Value);
        updateValid(if_id_reg.getValid());
    }

    void updateValid(bool value){ this->valid = value;    }

    int getPC()         const { return this->PC;          }
    int getNextPC()     const { return this->nextPC;      }
    int getRegWr()      const { return this->regWr;       }
    int getBranch()     const { return this->branch;      }
    int getALUSrc()     const { return this->aluSrc;      }
    int getALUSrc2()    const { return this->aluSrc2;     }
    int getMemWr()      const { return this->memWr;       }
    int getMemToReg()   const { return this-> memToReg;   }
    int getMemRd()      const { return this->memRd;       }
    int getALUOp()      const { return this->aluOp;       }
    int getJump()       const { return this->jump;        }
    int getPCSrc()      const { return this->PCSrc;       }
    int getRs1()        const { return this->rs1;         }
    int getRs2()        const { return this->rs2;         }
    int getRd()         const { return this->rd;          }
    int getOp1()        const { return this->operand1;    }
    int getOp2()        const { return this->operand2;    }
    int getImm()        const { return this->signExtImm;  }
    int getFunct3()     const { return this->funct3;      }
    int getFunct7()     const { return this->funct7;      }
    int getRs2Value()   const { return this->rs2Value;    }

    bool getValid()     const { return this->valid;       }

void NOP(){
        regWr = 0;
        memRd = 0;
        memWr = 0;
        memToReg = 0;
        aluSrc = 0;
        aluOp = 0;
        branch = 0;
        rd = 0;
        rs2 = 0;
        valid = false;
    }
};

/*
    All results of execution are placed into this pipline
    register. Branch target is stored here, so it will be
    read and used in EXE stage.
*/
class EXE_MEM {
    // Flag for register having valid data (not stale)
    bool valid = false;

    // PC info
    int PC = 0;
    int nextPC = 0;

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

    int rs2Value = 0;

    // ALU results
    int aluZero = 0;
    int aluResult = 0;

    int branchTarget = 0;

    void updatePC(int value)           { this->PC = value;           }
    void updateNextPC(int value)       { this->nextPC = value;       }
    void updateRegWr(int value)        { this->regWr = value;        }
    void updateBranch(int value)       { this->branch = value;       }
    void updateMemWr(int value)        { this->memWr = value;        }
    void updateMemToReg(int value)     { this->memToReg = value;     }
    void updateMemRd(int value)        { this->memRd = value;        }
    void updateJump(int value)         { this->jump = value;         }
    void updateRs2(int value)          { this->rs2 = value;          }
    void updateRs2Value(int value)     { this->rs2Value = value;     }
    void updateRd(int value)           { this->rd = value;           }
    void updateALUZero(int value)      { this->aluZero = value;      }
    void updateALUResult(int value)    { this->aluResult = value;    }
    void updateBranchTarget(int value) { this->branchTarget = value; }

    public:
    EXE_MEM(){}

    void updateInfo(ID_EXE id_exe_reg, int aluZero, int aluResult, int branchTarget){
        updatePC(id_exe_reg.getPC());
        updateNextPC(id_exe_reg.getNextPC());
        updateRegWr(id_exe_reg.getRegWr());
        updateBranch(id_exe_reg.getBranch());
        updateMemWr(id_exe_reg.getMemWr());
        updateMemToReg(id_exe_reg.getMemToReg());
        updateMemRd(id_exe_reg.getMemRd());
        updateJump(id_exe_reg.getJump());
        updateRs2(id_exe_reg.getRs2());
        updateRs2Value(id_exe_reg.getRs2Value());
        updateRd(id_exe_reg.getRd());
        updateALUZero(aluZero);
        updateALUResult(aluResult);
        updateBranchTarget(branchTarget);
        updateValid(id_exe_reg.getValid());
    }

    void updateValid(bool value){ this->valid = value;       }

    int getPC()           const { return this->PC;           }
    int getNextPC()       const { return this->nextPC;       }
    int getRegWr()        const { return this->regWr;        }
    int getBranch()       const { return this->branch;       }
    int getMemWr()        const { return this->memWr;        }
    int getMemToReg()     const { return this-> memToReg;    }
    int getMemRd()        const { return this->memRd;        }
    int getRs2()          const { return this->rs2;          }
    int getRs2Value()     const { return this->rs2Value;     }
    int getRd()           const { return this->rd;           }
    int getALUZero()      const { return this->aluZero;      }
    int getALUResult()    const { return this->aluResult;    }
    int getBranchTarget() const { return this->branchTarget; }

    bool getValid()       const { return this->valid;        }
};

/*
    The final pipeline register. The data regarding writes
    is stored here and then used for WB stage.
*/
class MEM_WB {
    // Flag for register having valid data (not stale)
    bool valid = false;

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
        updateValid(exe_mem_reg.getValid());
    }

    void updateValid(bool value){ this->valid = value;    }
    int getRegWr()      const { return this->regWr;       }
    int getMemToReg()   const { return this-> memToReg;   }
    int getRd()         const { return this->rd;          }
    int getALUResult()  const { return this->aluResult;   }
    int getMemData()    const { return this->memData;     }

    bool getValid()     const { return this->valid;       }
};

bool pipelineDrained(int PC, int instrCount, IF_ID if_id, ID_EXE id_exe, EXE_MEM exe_mem, MEM_WB mem_wb){
    bool outOfBounds = ((PC / 4) >= instrCount);
    bool pipelineRegistersEmpty = (
        !if_id.getValid()   &&
        !id_exe.getValid()  &&
        !exe_mem.getValid() &&
        !mem_wb.getValid()
    );
    return (outOfBounds && pipelineRegistersEmpty);
}

class PipelineObject {
    IF_ID* if_id_reg;
    ID_EXE* id_exe_reg;
    EXE_MEM* exe_mem_reg;
    MEM_WB* mem_wb_reg;
    int* PC;
    int instructionCount;

    public:
    PipelineObject(IF_ID* if_id, ID_EXE* id_exe, EXE_MEM* exe_mem, MEM_WB* mem_wb, int* PC, int instrCount){
        this->if_id_reg = if_id;
        this->id_exe_reg = id_exe;
        this->exe_mem_reg = exe_mem;
        this->mem_wb_reg = mem_wb;
        this->PC = PC;
        this->instructionCount = instrCount;
    }

    bool pipelineDrained(){
        bool outOfBounds = ((*(this->PC) / 4) >= (this->instructionCount));
        bool pipelineRegistersEmpty = (
            !this->if_id_reg->getValid()   &&
            !this->id_exe_reg->getValid()  &&
            !this->exe_mem_reg->getValid() &&
            !this->mem_wb_reg->getValid()
        );
        return (outOfBounds && pipelineRegistersEmpty);
    }
};

#endif
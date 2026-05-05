#ifndef HAZARD_DETECTION_HPP
#define HAZARD_DETECTION_HPP

#include "pipeline_registers.hpp"

class HazardDetectionUnit{
    IF_ID* if_id_reg;
    ID_EXE* id_exe_reg;
    EXE_MEM* exe_mem_reg;

    public:
    HazardDetectionUnit(IF_ID* if_id_reg, ID_EXE* id_exe_reg){
        this->if_id_reg = if_id_reg;
        this->id_exe_reg = id_exe_reg;
        this->exe_mem_reg = exe_mem_reg;
    }

    // If true, stall pipeline.
    bool stallPipeline(){
        bool memReadAtEXE = static_cast<bool>(id_exe_reg->getMemRd());
        int id_rd = id_exe_reg->getRd();
        int if_rs1 = if_id_reg->getRs1();
        int if_rs2 = if_id_reg->getRs2();
        bool destRegIsAnOperand = ((id_rd == if_rs1 || id_rd == if_rs2));

        return (memReadAtEXE && destRegIsAnOperand);
    }
};

#endif

#ifndef HAZARD_DETECTION_HPP
#define HAZARD_DETECTION_HPP

#include "pipeline_registers.hpp"

class HazardDetectionUnit{
    IF_ID* if_id_reg;
    ID_EXE* id_exe_reg;

    public:
    HazardDetectionUnit(IF_ID* if_id_reg, ID_EXE* id_exe_reg){
        this->if_id_reg = if_id_reg;
        this->id_exe_reg = id_exe_reg;
    }

    // If true, stall pipeline.
    bool stallPipeline(){
        bool memReadAtEXE = static_cast<bool>(id_exe_reg->getMemRd());
        int rd = id_exe_reg->getRd();
        int rs1 = if_id_reg->getRs1();
        int rs2 = if_id_reg->getRs2();
        bool destRegIsAnOperand = ((rd == rs1 || rd == rs2));

        return (memReadAtEXE && destRegIsAnOperand);
    }
};

#endif
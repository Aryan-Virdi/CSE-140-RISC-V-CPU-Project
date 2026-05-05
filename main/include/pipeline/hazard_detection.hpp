#ifndef HAZARD_DETECTION_HPP
#define HAZARD_DETECTION_HPP

#include "pipeline_registers.hpp"

class HazardDetectionUnit {
    IF_ID*   if_id_reg;
    ID_EXE*  id_exe_reg;
    EXE_MEM* exe_mem_reg;

    //From Lecture Notes:
    // ID/EX.MemRead and
    // ((ID/EX.RegisterRd == IF/ID.RegisterRs1) or
    //  (ID/EX.RegisterRd == IF/ID.RegisterRs2))
    // -> stall the pipeline
    bool load_check_hazard() {
        if (!id_exe_reg->getValid()) return false;
        int rs1 = if_id_reg->getRs1();
        int rs2 = if_id_reg->getRs2();
        int load_destination = id_exe_reg->getRd();

        if (id_exe_reg->getMemRd() == 1)
            if (load_destination == rs1 || load_destination == rs2)
                return true;

        return false;
}

    // Private — only used internally by stallPipeline()
    // Using Read-After-Write format
   bool RAW_hazard() {
    int rs1 = if_id_reg->getRs1(); 
    int rs2 = if_id_reg->getRs2(); 

    // Check EXE conflict
    if (id_exe_reg->getValid()) {
        int exe_destination = id_exe_reg->getRd();
        if (exe_destination != 0 && id_exe_reg->getRegWr() == 1) {
            if (exe_destination == rs1 || exe_destination == rs2) {
                return true;
            }
        }
    }

    // Check MEM conflict
    if (exe_mem_reg->getValid()) {
        int mem_destination = exe_mem_reg->getRd();
        if (mem_destination != 0 && exe_mem_reg->getRegWr() == 1) {
            if (mem_destination == rs1 || mem_destination == rs2) {
                return true;
            }
        }
    }

    return false;
}

    public:
    HazardDetectionUnit(IF_ID* if_id_reg, ID_EXE* id_exe_reg, EXE_MEM* exe_mem_reg){
        this->if_id_reg = if_id_reg;
        this->id_exe_reg = id_exe_reg;
        this->exe_mem_reg = exe_mem_reg;
    }

    bool stallPipeline(){
        if (load_check_hazard()) return true;
        if (RAW_hazard())        return true;
        return false;
    }
};

#endif

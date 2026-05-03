#ifndef HAZARD_DETECTION_HPP
#define HAZARD_DETECTION_HPP

#include "pipeline_registers.hpp"

class HazardDetectionUnit {
    IF_ID*   if_id_reg;
    ID_EXE*  id_exe_reg;
    EXE_MEM* exe_mem_reg;
}

public:
    HazardDetectionUnit(IF_ID* if_id_reg, ID_EXE* id_exe_reg){
        this->if_id_reg = if_id_reg;
        this->id_exe_reg = id_exe_reg;
    }

    bool stallPipeline() {
        int  if_rs1 = if_id_reg->getRs1();
        int  if_rs2 = if_id_reg->getRs2();
        bool useRs2 = if_id_reg->useRs2();

        // Load-use hazard: 1 stall
        if (id_exe_reg->getMemRd()) {
            int  load_dest = id_exe_reg->getRd();
            bool hazard    = (load_dest != 0) && ((load_dest == if_rs1) || (useRs2 && load_dest == if_rs2));
            if (hazard){
                return true;
            }
        }

        // RAW hazard stall 1: producing instruction in EX stage
        if (id_exe_reg->getRegWr() && !id_exe_reg->getMemRd()) {
            int  exe_dest = id_exe_reg->getRd();
            bool hazard   = (exe_dest != 0) && ((exe_dest == if_rs1) || (useRs2 && exe_dest == if_rs2));
            if (hazard) {
                return true;
            }
        }

        // RAW hazard stall 2: producing instruction in MEM stage
        if (exe_mem_reg->getRegWr()) {
            int  mem_dest = exe_mem_reg->getRd();
            bool hazard   = (mem_dest != 0) && ((mem_dest == if_rs1) || (useRs2 && mem_dest == if_rs2));
            if (hazard) {
                return true;
            }

        return false;
    }
};

#endif

#ifndef EXECUTE_HPP
#define EXECUTE_HPP

int ALU(int alu_intake_1, int alu_intake_2, int alu_control, int& alu_zero){
    int alu_result = 0;

    switch (alu_control) {
        case 0b0000: 
            alu_result = alu_intake_1 & alu_intake_2; 
            break; // AND

        case 0b0001: 
            alu_result = alu_intake_1 | alu_intake_2; 
            break; // OR

        case 0b0010: 
            alu_result = alu_intake_1 + alu_intake_2; 
            break; // ADD

        case 0b0110: 
            alu_result = alu_intake_1 - alu_intake_2; 
            break; // SUB

        case 0b0111: 
            alu_result = (alu_intake_1 < alu_intake_2) ? 1 : 0; 
            break; // SLT

        case 0b1100: 
            alu_result = ~(alu_intake_1 | alu_intake_2); 
            break; // NOR

        case 0b0011:
            alu_result = alu_intake_1;
            break; // jal/jalr; alu_intake_1 should be PC+4 in this case.
        default: 
            alu_result = 0; 
            break; // UNDEFINED
    }

    alu_zero = (alu_result == 0);
    return alu_result;
}

int programCounterAdder(int sign_extension_offset, int& pc_plus_4, int alu_zero, bool branch, bool jump, bool PCSrc, int rs1){
    // int branchAndJumpMuxOutput = ((branch && alu_zero) || jump) ? (sign_extension_offset << 1) : 4;
    // int PCSrcMuxOutput = PCSrc ? rs1 : pc;

    return pc_plus_4 + (sign_extension_offset << 1);   
}

int execute(int alu_intake_1, int alu_intake_2, int alu_control, int sign_extension_offset, int pc_plus_4, int& alu_zero, bool branch, bool jump, bool PCSrc, int rs1, int& branch_target) {
    int alu_result = ALU(alu_intake_1, alu_intake_2, alu_control, alu_zero);
    branch_target = programCounterAdder(sign_extension_offset, pc_plus_4, alu_zero, branch, jump, PCSrc, rs1);

    return alu_result;
}

#endif
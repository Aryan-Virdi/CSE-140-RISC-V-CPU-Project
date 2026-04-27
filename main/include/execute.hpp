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

void programCounterAdder(int sign_extension_offset, int pc, int pc_plus_4, bool branch, bool jump, bool PCSrc, int rs1, int& branch_target){
    int baseAddrHolder = (PCSrc ? rs1 : (jump ? pc : pc_plus_4));
    int offset = branch ? (sign_extension_offset << 1) : sign_extension_offset;

    branch_target = baseAddrHolder + offset;
}

int execute(int alu_intake_1, int alu_intake_2, int alu_control, int sign_extension_offset, int pc, int pc_plus_4, int& alu_zero, bool branch, bool jump, bool PCSrc, int rs1, int& branch_target) {
    programCounterAdder(sign_extension_offset, pc, pc_plus_4, branch, jump, PCSrc, rs1, branch_target);

    return ALU(alu_intake_1, alu_intake_2, alu_control, alu_zero);
}

// Branch target:
// Is always computed here.
// The fetch stage will choose between PC+4 and this value
// based on the branch control signal and alu_zero

// sign_extension_offset:
// Comes specifically from the instruction’s immediate field
// Is always sign-extended
// Used only for branch address calculation

// alu_intake_2 -> goes into the ALU
// sign_extension_offset -> goes into branch address logic
#endif
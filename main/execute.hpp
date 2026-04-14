#ifndef EXECUTE_HPP
#define EXECUTE_HPP

int execute(int alu_intake_1, int alu_intake_2, int alu_control, int pc_plus_4, int& alu_zero, int& branch_target) {
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

        default: 
            alu_result = 0; 
            break; // UNDEFINED
    }

    alu_zero = (alu_result == 0);
    branch_target = pc_plus_4 + (alu_intake_2 << 1);   

    return alu_result;
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

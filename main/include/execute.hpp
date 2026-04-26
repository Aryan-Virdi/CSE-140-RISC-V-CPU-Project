#ifndef EXECUTE_HPP
#define EXECUTE_HPP

int execute(int alu_intake_1, int alu_intake_2, int alu_control, int sign_extension_offset, int pc_current, int& alu_zero, int& branch_target, bool is_jal, bool is_jalr, int alu_result) {
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
        alu_result = (alu_intake_1 < alu_intake_2); 
        break; // SLT

        case 0b1100: 
        alu_result = ~(alu_intake_1 | alu_intake_2); 
        break; // NOR

        default: 
        alu_result = 0; 
        break; // UNDEFINED
    }

    alu_zero = (alu_result == 0);

    // Branch targeting for normal branches
    branch_target = pc_current + (sign_extension_offset << 1);

    // JAL is for pc based jumping
    if (is_jal) {
        branch_target = pc_current + sign_extension_offset;
    }

    // JALR is for register based jumps
    if (is_jalr) {
        branch_target = (alu_intake_1 + sign_extension_offset) & ~1;
    }

    return alu_result;
}

#endif

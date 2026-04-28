#ifndef EXECUTE_HPP
#define EXECUTE_HPP

/*
    Parameter alu_intake_1:     Operand 1
    Parameter alu_intake_2:     Operand 2
    Parameter alu_control:      The ALU Ctrl code from the ALU Control Unit.
    Parameter alu_zero:         A reference to the alu_zero variable.

    Return:                     Result of operation defined for alu_control.

    Note:                       Sets alu_zero to "1" or "TRUE" if alu_result is zero.
*/
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

/*
    Parameter sign_extended_offset:     The [sign extended] immediate from decoding.
    Parameter currPC:                   The copy of the current cycle's PC value.
    Parameter pc_plus_4:                The copy of the incremented PC.
    Parameter alu_zero:                 The copy of alu_zero. Treated as a boolean here.
    Parameter branch:                   The branch control signal.
    Parameter jump:                     The jump control signal.
    Parameter PCSrc:                    The PCSrc control signal.
    Parameter operand1:                 The operand from rs1. Use to execute register-relative addressing.

    Return:                             PC+4 (default; no jump, no branch), rs1 + sign_extended_offset (jalr target), or PC + sign_extended_offset (jal target).
*/
int programCounterAdder(int sign_extended_offset, int currPC, int pc_plus_4, bool alu_zero, bool branch, bool jump, bool PCSrc, int operand1){
    bool branch_taken = (branch && static_cast<bool>(alu_zero));
    bool jalr = (jump && PCSrc);
    bool jal = jump;

    if (branch_taken){
       return (pc_plus_4 + (sign_extended_offset << 1));
    } else if (jalr){
        return (operand1 + sign_extended_offset);
    } else if (jal){
        return (currPC + (sign_extended_offset << 1));
    }

    return pc_plus_4;
}

/*
    Parameter alu_intake_1:             Operand 1
    Parameter alu_intake_2:             Operand 2
    Parameter alu_control:              The ALU Ctrl signal from the ALU Control Unit.
    Parameter sign_extended_offset:     The [sign extended] immediate from decoding.
    Parameter curr_pc:                  The copy of the current cycle's PC value.
    Parameter pc_plus_4:                The copy of the incremented PC.
    Parameter alu_zero:                 The copy of alu_zero. Treated as a boolean here.
    Parameter branch:                   The branch control signal.
    Parameter jump:                     The jump control signal.
    Parameter PCSrc:                    The PCSrc control signal.
    Parameter branch_target:            Reference to the branch_target variable. This will update it.

    Return:                             The result of the ALU operation.

    Note:                               Will modify branch target with either PC+4, rs1 + sign_extended_offset, or PC + sign_extended_offset.

*/
int execute(int alu_intake_1, int alu_intake_2, int alu_control, int sign_extended_offset, int curr_pc, int pc_plus_4, int& alu_zero, bool branch, bool jump, bool PCSrc, int& branch_target) {
    int alu_result = ALU(alu_intake_1, alu_intake_2, alu_control, (alu_zero));
    branch_target = programCounterAdder(sign_extended_offset, curr_pc, pc_plus_4, alu_zero, branch, jump, PCSrc, alu_intake_1);

    return alu_result;
}

#endif
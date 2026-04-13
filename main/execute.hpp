#ifndef EXECUTE_HPP
#define EXECUTE_HPP

void execute(int operand1, int operand2, int alu_ctrl, int sign_extension_offset, int pc_plus_4, int& alu_zero, int& branch_target) {
    int result = 0;

    switch (alu_ctrl) {
        case 0b0000: result = operand1 & operand2; 
        break;
        case 0b0001: result = operand1 | operand2; 
        break;
        case 0b0010: result = operand1 + operand2; 
        break;
        case 0b0110: result = operand1 - operand2; 
        break;
        case 0b0111: result = (operand1 < operand2) ? 1 : 0; 
        break;
        case 0b1100: result = ~(operand1 | operand2); 
        break;
        default: result = 0; 
        break;
    }

    alu_zero = (result == 0);
    branch_target = pc_plus_4 + (sign_extension_offset << 1);
}

#endif

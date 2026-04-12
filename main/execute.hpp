#ifndef EXECUTE_HPP
#define EXECUTE_HPP

extern int alu_zero;
extern int branch_target;

void Execute(int operand1, int operand2, int alu_ctrl, int sign_ext_offset, int pc_plus_4);

#endif

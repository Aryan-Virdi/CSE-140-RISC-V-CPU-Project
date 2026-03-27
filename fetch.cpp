#include <cstdint>

uint32_t fetch(uint32_t &pc, uint8_t memory[]) {
    uint32_t instruction = 0;

    instruction |= (uint32_t)memory[pc];
    instruction |= (uint32_t)memory[pc + 1] << 8;
    instruction |= (uint32_t)memory[pc + 2] << 16;
    instruction |= (uint32_t)memory[pc + 3] << 24;

    pc += 4;  // Move to next instruction

    return instruction;
}
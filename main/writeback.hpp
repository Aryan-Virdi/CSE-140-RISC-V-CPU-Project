#ifndef WRITEBACK_HPP
#define WRITEBACK_HPP

int total_clock_cycles = 0; // Global variable to track total clock cycles

void writeback(int alu_result, int mem_read_data, bool reg_write, bool mem_to_reg, int &write_data) {
    if (reg_write) {
        write_data = mem_to_reg ? mem_read_data : alu_result;
        total_clock_cycles++; // Increment clock cycles for writeback stage
    }

}

#endif

// Writeback stage: selects data to write into register
// mem_to_reg = 1 -> memory, 0 -> ALU

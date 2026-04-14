#ifndef WRITEBACK_HPP
#define WRITEBACK_HPP

#include "printing.hpp"

void writeback(int alu_result, int mem_read_data, bool reg_write, bool mem_to_reg, int register_file[32], int destination_register, int& total_clock_cycles, PrintEvent& printQueue){
    if (reg_write) {
        int value = mem_to_reg ? mem_read_data : alu_result;
        register_file[destination_register] = value;

        printQueue.addPrintEvent(LocationType::registerFile, destination_register, value);
    }

    total_clock_cycles++; // Increment clock cycles for writeback stage
}

#endif

// Writeback stage: selects data to write into register
// mem_to_reg = 1 -> memory, 0 -> ALU

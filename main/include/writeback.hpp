#ifndef WRITEBACK_HPP
#define WRITEBACK_HPP


void writeback(int alu_result, int mem_read_data, int result_source,bool reg_write, int register_file[32], int destination_register, int& total_clock_cycles, PrintEvent& printQueue, int pc_plus_4
){
    if (!reg_write) {
        total_clock_cycles++;
        return;
    }

    int value;

    switch (result_source) {
        case 0: 
        value = alu_result; 
        break;

        case 1: 
        value = mem_read_data; 
        break;

        case 2: 
        value = pc_plus_4; 
        break;   // jal/jalr

        default: 
        value = 0; 
        break;
    }

    register_file[destination_register] = value;

    storeMemory(register_file, x0, 0);

    if (destination_register != x0) {
        printQueue.addPrintEvent(LocationType::registerFile, destination_register, value);
    }

    total_clock_cycles++;
}

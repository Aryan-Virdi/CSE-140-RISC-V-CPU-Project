#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#include "init.hpp"
#include "data_memory.hpp"
#include "exit_codes.h"
#include "control_unit.hpp"
#include "fetch.hpp"
#include "instruction.hpp"
#include "decoder.hpp"
#include "execute.hpp"
#include "writeback.hpp"
#include "printing.hpp"
#include "pipeline/pipeline_registers.hpp"
#include "pipeline/hazard_detection.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

int total_clock_cycles = 0;

// CPU's program counter
int PC = 0;
int nextPC = 0;
int currPC = 0; // "Local" copy of PC before being incremented by fetch for use after.

// CPU's register file. 32 registers
int rf[32];

// Data Memory
int d_mem[32];

// Computer's instruction memory
vector<uint32_t> instructionMemory;

// Control signals
int regWrite = 0;
int branch = 0;
int ALUSrc = 0;
int memWrite = 0;
int memToReg = 0;
int memRead = 0;
int ALUOp = 0;
int jump = 0;
int ALUSrc2 = 0;
int PCSrc = 0;

// Global control signal interface.
IControl controlSignals = IControl(&regWrite, &branch, &ALUSrc, &memWrite, &memToReg, &memRead, &ALUOp, &jump, &ALUSrc2, &PCSrc);

// Execution signals for use in comparison/branching.
int alu_zero = 0;
int branch_target = 0;

// Queue for printing modifications.
// Not related to a concrete object on the chip; just a descriptor.
PrintEvent printQueue = PrintEvent();

// Jal and Jalr control signals are apparently working but the PC is not being incremented correctly.
// Either branch is indeed being taken but not incremented correctly or something else is incorrect.

void singleCycleCPU(){
    while ((PC/4) < instructionMemory.size()){

        /* FETCH STAGE BEGIN */
        
        uint32_t currInstruction = fetch(PC, nextPC, currPC, instructionMemory, branch_target, static_cast<bool>(branch), static_cast<bool>(alu_zero), static_cast<bool>(jump), printQueue);

        /* FETCH STAGE END */
        /* DECODE STAGE BEGIN */

        Instruction instruction = decode(currInstruction, controlSignals, rf);

        /* DECODE STAGE END */
        /* EXECUTION STAGE BEGIN */

        int alu_ctrl = aluControl(ALUOp, instruction.getFunct3(), instruction.getFunct7());
        int operand1 = (static_cast<bool>(ALUSrc2) ? nextPC : instruction.getRs1Value());   // First operand of ALU operation is from rs1 if ALUSrc2 is false. Otherwise PC.
                                                                                            // Depends on ALUSrc2 being true if and only if instruction is jalr.
                                                                                    
        int operand2 = (static_cast<bool>(ALUSrc) ? instruction.getImm() : instruction.getRs2Value());  // Second operand of ALU operation is from immediate if ALUSrc is true, otherwise from rs2.
                                                                                                        // Logic depends on ALUSrc being true if and only if the instruction is an I-Type.

        // if (ALUSrc2){std::cout << "alu_ctrl=" << alu_ctrl << " op1=" << operand1 << " op2=" << operand2 << " pc=" << PC << " nextPC=" << nextPC << std::endl;}  // Debug
        int aluResult = execute(operand1, operand2, alu_ctrl, instruction.getImm(), currPC, PC, alu_zero, static_cast<bool>(branch), static_cast<bool>(jump), static_cast<bool>(PCSrc), instruction.getRs1Value(), branch_target);
        if (static_cast<bool>(branch) && static_cast<bool>(alu_zero) || static_cast<bool>(jump)) {
            PC = branch_target;
        } else {
            PC = nextPC;
        }
        printQueue.addPrintEvent(LocationType::programCounter, EMPTY_IDX, PC);

        /* EXECUTION STAGE END */
        /* MEM STAGE BEGIN */

        int data = mem(d_mem, aluResult, instruction.getRs2Value(), static_cast<bool>(memWrite), printQueue);   // Returns an actual d_mem value if memWrite is true.
                                                                                                                // The value in this function call is the second source register because
                                                                                                                // memory should only be written into by store-word, which provides the data in RS2.

        /* MEM STAGE END */
        /* WRITEBACK STAGE BEGIN */

        writeback(aluResult, data, static_cast<bool>(regWrite), static_cast<bool>(memToReg), rf, instruction.getRd(), total_clock_cycles, printQueue);

        /* WRITEBACK STAGE END */

        cout << "total_clock_cycles " << total_clock_cycles << " :" << endl;
        printQueue.printModifications();    // Print this cycle's modifications.
    }
    
    cout << "program terminated:" << endl << "total execution time is " << total_clock_cycles << " cycles" << endl;
}

void pipelinedCPU(){
    IF_ID   if_id_buffer   = IF_ID();
    ID_EXE  id_exe_buffer  = ID_EXE();
    EXE_MEM exe_mem_buffer = EXE_MEM();
    MEM_WB  mem_wb_buffer  = MEM_WB();

    HazardDetectionUnit hazardDetectionUnit(&if_id_buffer, &id_exe_buffer);

    cout << "Hello! o/" << endl;
}

/*
    Parameter argc: Number of arguments passed in by terminal.
    Parameter argv: char pointer array containing those arguments. Should be in the form of:
                            argv[0]:    program name
                            argv[1]:    input file containing machine code instruction(s)
                            argv[2-3]:  optional sample and architecture flags

    Returns:        Successful code upon graceful completion. Specific error code otherwise.
*/
int main(int argc, char* argv[]) {
    if (argc < 2){ 
        cerr << "Input file name not provided. Program terminating." << endl;
        return ILLEGAL_ARGUMENT;
    }

    bool pipelined = false;

    // All 32 registers initialized to zero.
    for (int i = 0; i < 32; i++){ rf[i] = 0; d_mem[i] = 0; }  // Global arrays should be initialized to zero automatically, but here it is done manually just in case.

    // Handle terminal arguments accordingly.
    int processedCode = processArguments(argc, argv, rf, d_mem, pipelined, instructionMemory);
    if (processedCode != SUCCESS){ return processedCode; }  // If there was an error, terminate the program entirely.

    if (pipelined){
        pipelinedCPU();
    } else {
        singleCycleCPU();
    }

    return SUCCESS;
}
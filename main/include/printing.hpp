#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <vector>
#include <iostream>
#include <string>

// Used as index parameter for program counter events.
const int EMPTY_IDX = -1;
std::vector<std::string> regNames = {
    "zero",
    "ra",
    "sp",
    "gp",
    "tp",
    "t0",
    "t1",
    "t2",
    "s0",
    "s1",
    "a0",
    "a1",
    "a2",
    "a3",
    "a4",
    "a5",
    "a6",
    "a7",
    "s2",
    "s3",
    "s4",
    "s5",
    "s6",
    "s7",
    "s8",
    "s9",
    "s10",
    "s11",
    "t3",
    "t4",
    "t5",
    "t6"  
}; 

// Enumerates types of locations that can broadcast their changes.
enum class LocationType{
    registerFile, dataMemory, programCounter
};

// Convenience wrapper to store such modification events.
struct ValueLocation{
    LocationType location;
    int idx;
    int value;

    // Boolean helpers to determine which of the units broadcasted the change.
    bool isRegFile(){
        return (this->location == LocationType::registerFile);
    }

    bool isDataMem(){
        return (this->location == LocationType::dataMemory);
    }

    bool isProgramCounter(){
        return (this->location == LocationType::programCounter);
    }
};

// Actual interface for adding and executing printable events.
class PrintEvent{
    std::vector<ValueLocation> modificationQueue;
    bool printEnabled = true;

    /*
        Parameter location:     The location type where the change occurred.

        Description:            Checks the element in the queue for which unit broadcasted it.
                                Prints the modification's information as formatted in the instructions.

        Note:                   Private so that individual elements can not be printed without printing
                                every element and then clearing.
    */
    void printModification(ValueLocation location, bool withConventionNames){
        if (location.isRegFile()){
            std::string regName = "x" + std::to_string(location.idx);
            if (withConventionNames){ regName = (regNames.at(location.idx)); }

            std::cout << regName << " is modified to 0x" << std::hex << location.value << std::dec << std::endl;
            return; 
        }

        if (location.isDataMem()){ 
            std::cout << "memory 0x" << std::hex << location.idx << " is modified to 0x" << location.value << std::dec << std::endl;
            return; 
        }

        if (location.isProgramCounter()){ 
            std::cout << "pc is modified to 0x" << std::hex << location.value << std::dec << std::endl;
            return; 
        }
    }

    /*
        Description:    Empties the queue.
        Note:           Private so that the queue can only
                        be predictably cleared.
    */
    void clearQueue(){ this->modificationQueue.clear(); }

    public:
    /*
        Default constructor to reserve some initial
        space for the queue (2 elements to start with).
    */
    PrintEvent(){ this->modificationQueue.reserve(2); }

    /*
        Parameter modifiedLocation:     The type of modification that happened (PC or memory unit).
        Parameter idx:                  The associated index for a register file or data memory modification.
                                        A dummy value should be provided for a PC modification.
        Parameter value:                The associated value of the modification.
    */
    void addPrintEvent(LocationType modifiedLocation, int idx, int value){
        if (!printEnabled) { return; }
        ValueLocation location = {modifiedLocation, idx, value};
        this->modificationQueue.push_back(location);
    }

    /*
        Description:    Prints the modification information for every change in the queue.
                        The queue is cleared upon completion.
    */
    void printModifications(bool withConventionNames){
        if (!printEnabled) { return; }
        for (int idx = this->modificationQueue.size() - 1; idx >= 0; idx--){ 
            printModification(modificationQueue[idx], withConventionNames); 
        }
        clearQueue();
        std::cout << std::endl;
    }

    void allowPrint(bool shouldPrint){
        this->printEnabled = shouldPrint;
    }
};

#endif
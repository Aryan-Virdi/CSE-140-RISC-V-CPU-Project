#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <vector>
#include <iostream>

// Used as index parameter for program counter events.
const int EMPTY_IDX = -1;

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

    /*
        Parameter index:    The i-th element of the queue.

        Description:        Checks the element in the queue for which unit broadcasted it.
                            Prints the modification's information as formatted in the instructions.

        Note:               Private so that individual elements can not be printed without printing
                            every element.
    */
    void printModification(ValueLocation location){
        if (location.isRegFile()){ 
            std::cout << "x" << location.idx << " is modified to 0x" << std::hex << location.value << std::endl; 
            return; 
        }

        if (location.isDataMem()){ 
            std::cout << "memory 0x" << std::hex << location.idx << " is modified to 0x" << location.value << std::endl; 
            return; 
        }

        if (location.isProgramCounter()){ 
            std::cout << "pc is modified to 0x" << std::hex << location.value << std::endl; 
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
        space for the queue (8 elements to start with).
    */
    PrintEvent(){ this->modificationQueue.reserve(8); }

    /*
        Parameter modifiedLocation:     The type of modification that happened (PC or memory unit).
        Parameter idx:                  The associated index for a register file or data memory modification.
                                        A dummy value should be provided for a PC modification.
        Parameter value:                The associated value of the modification.
    */
    void addPrintEvent(LocationType modifiedLocation, int idx, int value){
        ValueLocation location = {modifiedLocation, idx, value};
        this->modificationQueue.push_back(location);
    }

    /*
        Description:    Prints the modification information for every change in the queue.
                        The queue is cleared upon completion.
    */
    void printModifications(){
        for (int idx = 0; idx < this->modificationQueue.size(); idx++){ 
            printModification(modificationQueue[idx]); 
        }
        clearQueue();
        std::cout << std::endl;
    }
};

#endif
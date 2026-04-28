#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <vector>
#include <iostream>
#include <string>

// Used as index parameter for program counter events.
const int EMPTY_IDX = -1;

struct NamePairs {
    int regIdx;
    std::string regName;

    std::string getName(){ return this->regName; }
};

std::vector<NamePairs> regNames = {
    {0,  "zero" },
    {1,  "ra"   },
    {2,  "sp"   },
    {3,  "gp"   },
    {4,  "tp"   },
    {5,  "t0"   },
    {6,  "t1"   },
    {7,  "t2"   },
    {8,  "s0"   },
    {9,  "s1"   },
    {10, "a0"   },
    {11, "a1"   },
    {12, "a2"   },
    {13, "a3"   },
    {14, "a4"   },
    {15, "a5"   },
    {16, "a6"   },
    {17, "a7"   },
    {18, "s2"   },
    {19, "s3"   },
    {20, "s4"   },
    {21, "s5"   },
    {22, "s6"   },
    {23, "s7"   },
    {24, "s8"   },
    {25, "s9"   },
    {26, "s10"  },
    {27, "s11"  },
    {28, "t3"   },
    {29, "t4"   },
    {30, "t5"   },
    {31, "t6"   }
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
            if (withConventionNames){ regName = (regNames.at(location.idx).getName()); }

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
        ValueLocation location = {modifiedLocation, idx, value};
        this->modificationQueue.push_back(location);
    }

    /*
        Description:    Prints the modification information for every change in the queue.
                        The queue is cleared upon completion.
    */
    void printModifications(bool withConventionNames){
        for (int idx = this->modificationQueue.size() - 1; idx >= 0; idx--){ 
            printModification(modificationQueue[idx], withConventionNames); 
        }
        clearQueue();
        std::cout << std::endl;
    }
};

#endif
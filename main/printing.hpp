#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <vector>
#include <iostream>

/*
    I now realize this entire system is most likely overkill.
    We can replace it with normal cout statements later if desired.
*/

// Used as index parameter for program counter events.
const int EMPTY_IDX = -1;

// Enumerates types of locations that can broadcast their changes.
enum class LocationType{
    registerFile, dataMemory, programCounter
};

// Convenience wrapper to store such modification events.
struct valueLocation{
    LocationType location;
    int idx;
    int value;

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
    std::vector<valueLocation> modificationQueue;

    void printModification(int index){
        valueLocation location = modificationQueue[index];

        if (location.isRegFile()){ std::cout << "x" << location.idx << " is modified to " << std::hex << location.value << std::endl; }

        if (location.isDataMem()){ std::cout << "memory " << std::hex << location.idx << " is modified to " << location.value << std::endl; }

        if (location.isProgramCounter()){ std::cout << "pc is modified to " << std::hex << location.value << std::endl; }
    }

    void clearBuffer(){ modificationQueue.clear(); }

    public:
    PrintEvent(){
        this->modificationQueue.reserve(8);
    }

    void addPrintEvent(LocationType memoryType, int idx, int value){
        valueLocation location = {memoryType, idx, value};
        modificationQueue.push_back(location);
    }

    void printModifications(){
        for (int idx = 0; idx << modificationQueue.size(); idx++){ printModification(idx); }
        clearBuffer();
    }
};

#endif
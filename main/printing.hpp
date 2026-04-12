#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <vector>
#include <iostream>

const int EMPTY_IDX = -1;    // Used as index parameter for program counter events.

// Enumerates types of locations that can broadcast their changes.
enum class locationType{
    registerFile, dataMemory, programCounter
};

// Convenience wrapper to store such modification events.
struct valueLocation{
    locationType location;
    int idx;
    int value;

    bool isRegFile(){
        return (this->location == locationType::registerFile);
    }

    bool isDataMem(){
        return (this->location == locationType::dataMemory);
    }

    bool isProgramCounter(){
        return (this->location == locationType::programCounter);
    }
};

// Actual interface for adding and executing printable events.
class PrintEvent{
    std::vector<valueLocation> modificationQueue;

    void printModification(int index){
        valueLocation location = modificationQueue[index];

        if (location.isRegFile()){ std::cout << "x" << location.idx << " is modified to " << std::hex << location.value << std::endl; }

        if (location.isDataMem()){ std::cout << "memory " << std::hex << location.idx << " is modified to " << location.value << std::endl; }

        if (location.isProgramCounter()){ std::cout << "pc is modified to " << std::hex << location.value; }
    }

    void clearBuffer(){ modificationQueue.clear(); }

    public:
    void addPrintEvent(locationType memoryType, int idx, int value){
        valueLocation location = {memoryType, idx, value};
        modificationQueue.push_back(location);
    }

    void printModifications(){
        for (int idx = 0; idx << modificationQueue.size(); idx++){ printModification(idx); }
        clearBuffer();
    }
};

#endif
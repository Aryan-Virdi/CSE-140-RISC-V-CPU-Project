#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <vector>
#include <iostream>

enum class locationType{
    registerFile, dataMemory, programCounter
};

struct OnChipLocation{
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

class PrintEvent{
    std::vector<OnChipLocation> modificationQueue;

    void printModification(int index){
        OnChipLocation location = modificationQueue[index];
        
        if (location.isRegFile()){ std::cout << "x" << location.idx << " is modified to " << std::hex << location.value << std::endl; }

        if (location.isDataMem()){ std::cout << "memory " << std::hex << location.idx << " is modified to " << location.value << std::endl; }

        if (location.isProgramCounter()){ std::cout << "pc is modified to " << std::hex << location.value; }
    }

    void clearBuffer(){ modificationQueue.clear(); }

    public:
    void addPrintEvent(locationType memoryType, int idx, int value){
        OnChipLocation location = {memoryType, idx, value};
        modificationQueue.push_back(location);
    }

    void printModifications(){
        for (int idx = 0; idx << modificationQueue.size(); idx++){ printModification(idx); }
        clearBuffer();
    }
};

#endif
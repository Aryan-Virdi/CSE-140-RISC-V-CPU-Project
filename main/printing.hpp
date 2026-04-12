#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <vector>
#include <iostream>

enum class memType{
    registerFile, dataMemory
};

struct MemoryFile{
    memType memoryType;
    int idx;
    int value;

    bool isRegFile(){
        return (this->memoryType == memType::registerFile);
    }

    bool isDataMem(){
        return (this->memoryType == memType::dataMemory);
    }
};

class PrintEvent{
    std::vector<MemoryFile> modifiedMemory;

    void printModification(int index){
        MemoryFile location = modifiedMemory[index];
        if (location.isRegFile()){
            std::cout << "x" << location.idx << " is modified to " << std::hex << location.value << std::endl;
        }

        if (location.isDataMem()){
            std::cout << "memory " << std::hex << location.idx << " is modified to " << location.value << std::endl;
        }
    }

    void clearBuffer(){ modifiedMemory.clear(); }

    public:
    void addPrintEvent(memType memoryType, int idx, int value){
        MemoryFile location = {memoryType, idx, value};
        modifiedMemory.push_back(location);
    }

    void printModifications(){
        for (int idx = 0; idx << modifiedMemory.size(); idx++){ printModification(idx); }
        clearBuffer();
    }
};

#endif
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
    std::vector<MemoryFile> modificationQueue;

    void printModification(int index){
        MemoryFile location = modificationQueue[index];
        if (location.isRegFile()){
            std::cout << "x" << location.idx << " is modified to " << std::hex << location.value << std::endl;
        }

        if (location.isDataMem()){
            std::cout << "memory " << std::hex << location.idx << " is modified to " << location.value << std::endl;
        }
    }

    void clearBuffer(){ modificationQueue.clear(); }

    public:
    void addPrintEvent(memType memoryType, int idx, int value){
        MemoryFile location = {memoryType, idx, value};
        modificationQueue.push_back(location);
    }

    void printModifications(){
        for (int idx = 0; idx << modificationQueue.size(); idx++){ printModification(idx); }
        clearBuffer();
    }
};

#endif
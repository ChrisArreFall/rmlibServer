//
// Created by Chris on 10/1/17.
//

#ifndef RMLIBSERVER_MEMORY_H
#define RMLIBSERVER_MEMORY_H

#include "dataStructures/List.h"
#include "grapher/FileUpdater.h"
/**
 *
 */
class Memory{
public:
    List memory;
    int memorySize;
    int contador;
    FileUpdater file;
    inline void addMemory(int key, string valor);
    inline void deleteMemory(int key);
    inline bool search(int key);
    inline Memory(){
        memorySize = 0;
        contador=6;
    }
};
/**
 *
 * @param key
 * @param valor
 */
inline void Memory::addMemory(int key, string valor) {
    memorySize+=valor.size();
    cout<< "Tamaño de memoria del servidor usada: "<<memorySize<<endl;
    file.updater(contador,memorySize,"/Users/chris/ProgramaPrueba/cmake-build-debug/memoria.txt");
    memory.insertFirst(key,valor);
    contador++;
}
/**
 *
 * @param key
 */
inline void Memory::deleteMemory(int key) {
    memorySize-=memory.search(key)->value.size();
    file.updater(contador,memorySize,"/Users/chris/ProgramaPrueba/cmake-build-debug/memoria.txt");
    memory.deleteWithKey(key);
    contador++;
}
inline bool Memory::search(int key){
    return memory.searchBool(key);
}

#endif //RMLIBSERVER_MEMORY_H

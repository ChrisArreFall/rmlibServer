//
// Created by Chris on 9/30/17.
//

#ifndef RMLIBSERVER_MENSAJE_H
#define RMLIBSERVER_MENSAJE_H

#include <string>
#include <cstring>

using namespace std;


struct Mensaje {


public:
    string valor;
    int key;
    int estadoDeMemoria;
    bool busqueda;
    bool eliminar;

    Mensaje(){
        estadoDeMemoria = 0;
        key = 0;
        estadoDeMemoria = 0;
        busqueda = false;
        eliminar = false;
        valor = "";
    }
};


#endif //RMLIBSERVER_MENSAJE_H

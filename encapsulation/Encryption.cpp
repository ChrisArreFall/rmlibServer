//
// Created by Chris on 9/30/17.
//

#include "Encryption.h"
/**
 *
 * @param valor
 * @return
 */
string Encryption::start(string valor) {
    char key[] = {'K', 'C', 'Q'};
    string datos = valor; //iniciar la variable
    for (int i = 0; i < datos.size(); i++) { //for loop for scrambling bits in the string
        datos[i] = valor[i] ^ key[i % (sizeof(key) / sizeof(char))]; //hace un desacomodo de las palabras
    }
    return datos;
}
#include <iostream>

#include <thread>
#include <iostream>
#include "connection/ClientSocket.h"
#include "connection/ServerPrimary.h"
#include "connection/ServerBackup.h"

using namespace std;
int main(int argc, char *argv[]) {
    cout << "Bienvenido al servidor de memoria.\n";
    cout << "Cual sera el Puerto de este servidor?\n";
    int port;
    cin >> port;
    cout << "Que tipo de servidor sera este?\n";
    cout << "Digite 1 para Activo\n";
    cout << "Digite 2 para Pasivo\n";
    int activoPasivo;
    cin >> activoPasivo;
    if(activoPasivo == 1){

        int portBackup;
        int ipBackup;
        //cout << "Cual es el ip del servidor passivo?\n";
        //cin >> ipBackup;
        cout << "Cual es el Puerto del servidor Pasivo?\n";
        cin >> portBackup;
        ServerPrimary primary;
        primary.serverStart(port,portBackup);
        cout << "Se ha iniciado el servidor Activo exitosamente!\n";
    }
    else{
        ServerBackup backup;
        backup.serverStart(port);
        cout << "Se ha iniciado el servidor Pasivo exitosamente!\n";
    }


    return 0;
}
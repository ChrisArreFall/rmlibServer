#include <iostream>

#include <thread>
#include <iostream>
#include "connection/ClientSocket.h"
#include "connection/ServerSocket.h"

using namespace std;
void startServer(int port){


    Server server;
    server.serverStart(port);
}
void startClient(int port){

    tcp_client client;
    client.conn("localhost",port);
}

int main(int argc, char *argv[]) {
    cout << "Bienveido al servidor de memoria.\n";
    cout << "Cual sera el Puerto de este servidor?\n";
    int port;
    cin >> port;
    cout << "Que tipo de servidor sera este?\n";
    cout << "Digite 1 para Activo\n";
    cout << "Digite 2 para Pasivo\n";
    int activoPasivo;
    cin >> activoPasivo;
    std::thread threadServer(startServer,port);
    threadServer.join();
    if(activoPasivo == 1){

        int portClient;
        int ipClient;
        cout << "Cual es el ip del servidor passivo?\n";
        cin >> ipClient;
        cout << "Cual es el Puerto del servidor Pasivo?\n";
        cin >> portClient;
        std::thread threadCliente(startClient,portClient);
        threadCliente.detach();
        cout << "Se ha iniciado el servidor Activo exitosamente!\n";
    }
    else{

        cout << "Se ha iniciado el servidor Pasivo exitosamente!\n";
    }


    return 0;
}
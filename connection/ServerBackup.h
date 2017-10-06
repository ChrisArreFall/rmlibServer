//
// Created by Chris on 10/4/17.
//

#ifndef RMLIBSERVER_SERVERBACKUP_H
#define RMLIBSERVER_SERVERBACKUP_H



#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <json.hpp>
#include "../dataStructures/List.h"
#include "../encapsulation/Message.h"
#include "../Memory.h"
#include "ServerPrimary.h"
#define BACKLOG 10

class ServerBackup{
public:
    inline ServerBackup() = default;

    Memory memory;
    inline int serverStart(int port);
};
/**
 *
 * @param port
 * @return
 */
inline int ServerBackup::serverStart(int port) {
    struct sockaddr_in server;
    struct sockaddr_in dest;
    int status, socket_fd, client_fd, num;
    socklen_t size;


    char buffer[10241];
    char *buff;
//  memset(buffer,0,sizeof(buffer));
    int yes = 1;


    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Socket failure!!\n");
        exit(1);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    memset(&server, 0, sizeof(server));
    memset(&dest, 0, sizeof(dest));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    if ((::bind(socket_fd, (struct sockaddr *) &server, sizeof(struct sockaddr))) == -1) { //sizeof(struct sockaddr)
        fprintf(stderr, "Binding Failure\n");
        exit(1);
    }

    if ((listen(socket_fd, BACKLOG)) == -1) {
        fprintf(stderr, "Listening Failure\n");
        exit(1);
    }
    Encryption encryption;
    while(1) {

        size = sizeof(struct sockaddr_in);

        if ((client_fd = accept(socket_fd, (struct sockaddr *)&dest, &size))==-1 ) {
            perror("accept");
            exit(1);
        }
        printf("Server got connection from client %s\n", inet_ntoa(dest.sin_addr));

        while(1) {

            if ((num = recv(client_fd, buffer, 1024,0))== -1) {
                perror("recv");
                exit(1);
            }
            else if (num == 0) {
                printf("Connection closed\n");
                //So I can now wait for another client
                break;
            }
            buffer[num] = '\0';
            printf("\n Server:Msg Received %s\n", encryption.start(buffer).c_str());
            nlohmann::json j;
            Mensaje mensaje;
            from_json(nlohmann::json::parse(buffer),mensaje);
            //cout<< mensaje.valor<<endl;
            //Elimina un dato
            if(mensaje.eliminar){
                cout<<"eliminar....";
                //hace reset de el buffer para no tener problemas
                memset(buffer, '\0', sizeof(buffer));
                //si se va a eliminar
                //primero revisa si ya esta en la memoria
                if(memory.search(mensaje.key)){
                    //si es cierto entonces tiene que eliminarla de memoria
                    memory.deleteMemory(mensaje.key);
                    //tiene que crear un mensaje con la cantidad de memoria actual de los servidores
                    Mensaje eliminoExito;
                    eliminoExito.estadoDeMemoria = memory.memorySize;
                    //hay que mandar de vuelta el mensaje para que el cliente cambie la cantidad de memoria en los servidores
                    to_json(j,eliminoExito);
                    //retorna al cliente el valor de la memoria del servidor
                    strcpy(buffer, j.dump().c_str());
                }
                else{
                    //si no esta entonces retorna false
                    string resultado = "false";
                    strcpy(buffer, resultado.c_str());
                }
            }
            //Buscar un dato
            else if(mensaje.busqueda) {
                cout<<"buscar...."<<endl;
                //revisa si esta en memoria
                if (memory.search(mensaje.key)) {
                    cout<<"Se ha encontrado en memoria local..."<<endl;
                    //si es asi entonces crea el mensaje de retorno
                    Mensaje temp;
                    //le asigna el valor del dato deseado
                    temp.valor = memory.memory.search(mensaje.key)->value;
                    //lo convierte a json
                    to_json(j,temp);
                    //hace un reset del buffer para eliminar basura
                    memset(buffer, '\0', sizeof(buffer));
                    //y lo mete en el buffer
                    strcpy(buffer,j.dump().c_str());
                }
                else{
                    cout<<"No se ha encontrado en memoria..."<<endl;
                    memset(buffer, '\0', sizeof(buffer));
                    string resultado = "{\"busqueda\":false,\"eliminar\":false,\"estadoDeMemoria\":0,\"key\":0,\"valor\":\"{\\\"carne\\\":0,\\\"nombre\\\":\\\"No se encuentra en memoria...\\\",\\\"ponderado\\\":0}\"}";
                    strcpy(buffer, resultado.c_str());
                }
            }
            //agregar un dato
            else {
                cout<<"agregar....";
                //resetea variable para evitar basura
                memset(buffer, '\0', sizeof(buffer));
                Mensaje agregoExcito;
                //revisa si ya esta en la memoria
                if(memory.search(mensaje.key)){
                    memory.deleteMemory(mensaje.key);
                    memory.addMemory(mensaje.key,mensaje.valor);

                    agregoExcito.estadoDeMemoria = memory.memorySize;
                    //hay que mandar de vuelta el mensaje para que el cliente cambie la cantidad de memoria de los servidores
                    to_json(j,agregoExcito);
                    //retorna al cliente el valor de la memoria del servidor
                    strcpy(buffer, j.dump().c_str());
                }
                // si no esta en memoria lo agrega
                else{
                    memory.addMemory(mensaje.key, mensaje.valor);
                    agregoExcito.estadoDeMemoria = memory.memorySize;
                    //hay que mandar de vuelta el mensaje para que el cliente cambie la cantidad de memoria de los servidores

                    to_json(j,agregoExcito);
                    cout<<"Retorno: "<<encryption.start(j.dump().c_str()).c_str()<<endl;
                    //retorna al cliente el valor de la memoria del servidor
                    strcpy(buffer, j.dump().c_str());
                }

            }
            if ((send(client_fd,buffer, strlen(buffer),0))== -1){
                fprintf(stderr, "Failure Sending Message\n");
                close(client_fd);
                break;
            }
            string encryptedMsg = buffer;

            printf("Server:Msg being sent: %s\nNumber of bytes sent: %zu",encryption.start(encryptedMsg).c_str(), strlen(buffer));
            memset(buffer, '\0', sizeof(buffer));
            cout<<"\n------------------------------------------------------------------------------------------------"<<endl;
        } //End of Inner While...
        //Close Connection Socket
        close(client_fd);
    } //Outer While

    close(socket_fd);
    return 0;
} //End of main


#endif //RMLIBSERVER_SERVERBACKUP_H

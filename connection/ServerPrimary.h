//
// Created by Chris on 9/30/17.
//

#ifndef RMLIBSERVER_SERVERSOCKET_H
#define RMLIBSERVER_SERVERSOCKET_H

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
#include "ClientSocket.h"
#include "../encapsulation/Encryption.h"

#define BACKLOG 10

class ServerPrimary{
public:
    inline ServerPrimary() = default;
    tcp_client client;
    Memory memory;
    inline int serverStart(int port,int backupPort);
};
/**
 *
 * @param j
 * @param m
 */
inline void to_json(nlohmann::json& j, const Mensaje& m) {
    j = nlohmann::json{{"estadoDeMemoria", m.estadoDeMemoria},
                       {"valor",           m.valor},
                       {"key",             m.key},
                       {"busqueda",        m.busqueda},
                       {"eliminar",        m.eliminar},
    };
}
/**
 *
 * @param j
 * @param m
 */
inline void from_json(const nlohmann::json& j, Mensaje& m){
    m.estadoDeMemoria = j.at("estadoDeMemoria").get<int>();
    m.valor = j.at("valor").get<string>();
    m.key = j.at("key").get<int>();
    m.busqueda = j.at("busqueda").get<bool>();
    m.eliminar = j.at("eliminar").get<bool>();
}
/**
 *
 *
 * @param port
 * @param backupPort
 * @return
 */
inline int ServerPrimary::serverStart(int port,int backupPort) {
    client.conn("localhost",backupPort);
    //client.send_data("data");
    //Mensaje init;
    //from_json(nlohmann::json::parse(client.receive(1024)),init);
    //memory.memorySize=init.estadoDeMemoria;

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
            printf("\n Server:Msg Received %s\n", buffer);
            Encryption encryption;
            nlohmann::json j;
            Mensaje mensaje;
            from_json(nlohmann::json::parse(buffer),mensaje);
            cout<< "Incoming Message: "<< mensaje.valor<<endl;

            //--------------- Elimina en memoria y se manda al server Backup para que se elimine tambien -----------------------------
            if(mensaje.eliminar){
                //mando el mensaje al servidor backup
                client.send_data(nlohmann::json::parse(buffer).dump());
                cout<<"Sending to backup: "<<nlohmann::json::parse(buffer).dump()<<endl;
                //reseteo el buffer para eliminar basura
                memset(buffer, '\0', sizeof(buffer));
                //reviso si el dato esta en la memoria local
                if(memory.search(mensaje.key)){
                    cout<<"Se encontro el dato en memoria...."<<endl;
                    //si es cierto entonces tiene que eliminarla de memoria
                    memory.memory.deleteWithKey(mensaje.key);
                    //tiene que crear un mensaje con la cantidad de memoria actual de los servidores
                    Mensaje eliminoExito;
                    eliminoExito.estadoDeMemoria = memory.memorySize;
                    //hay que mandar de vuelta el mensaje para que el cliente cambie la cantidad de memoria en los servidores
                    to_json(j,eliminoExito);
                    //retorna al cliente el valor de la memoria del servidor
                    strcpy(buffer, j.dump().c_str());
                }
                //si no esta en memoria local
                else{
                    cout<<"No se encontro el dato en memoria.... Buscando en backup...."<<endl;
                    //busca a ver si lo tiene el servidor backup
                    string receive = client.receive(1024);
                    cout<<"Se ha recibido mensaje del backup: "<< receive<<endl;
                    //recibe el resultado del servidor backup
                    //si es "false" entonces tampoco esta en el servidor backup
                    if(receive == "falso"){
                        //manda al cliente que no se pudo encontrar en nigun servidor
                        string resultado = "falso";
                        strcpy(buffer, resultado.c_str());
                    }
                    //si lo pudo eliminar de la memoria
                    else {
                        //hace un forward del mensaje del backup server
                        strcpy(buffer, receive.c_str());
                    }
                }
            }
            //--------------- Busca el key en memoria, si no esta lo busca en el server backup -----------------------------
            else if(mensaje.busqueda) {
                cout<<"Buscando dato..."<<endl;
                //revisa si el el dato esta en la memoria local
                if (memory.search(mensaje.key)) {
                    cout<<"Se ha encontrado dato en memoria... Actualizando..."<<endl;
                    //si es asi entonces crea el mensaje de retorno
                    Mensaje temp;
                    //le asigna el valor del dato deseado
                    temp.valor = memory.memory.search(mensaje.key)->value;
                    //lo convierte a json
                    to_json(j,temp);
                    //hace un reset del buffer para eliminar basura
                    memset(buffer, '\0', sizeof(buffer));
                    //y lo mete en el buffer
                    strcpy(buffer, j.dump().c_str());
                }
                //si el dato no esta en memoria local
                else{
                    cout<<"No se ha encontrado el dato en memoria local... se buscara en el servidor backup"<<endl;
                    //tiene que enviarlo al servidor backup
                    client.send_data(nlohmann::json::parse(buffer).dump());
                    cout<<"Enviando busqueda a backup: "<<encryption.start(nlohmann::json::parse(buffer).dump()).c_str()<<endl;
                    //si no esta en la memoria del servidor backup

                    string retorno = client.receive(1024);
                    cout<< "Se ha recibido una respuesta del backup: "<<encryption.start(retorno).c_str()<<endl;
                    if(retorno=="falso"){
                        //devuelve un false para que el cliente se de cuenta que no esta
                        string resultado = "falso";
                        strcpy(buffer, resultado.c_str());
                    }
                    //si esta en la memoria del servidor backup
                    else{
                        //hace un forward del mensaje al cliente
                        memset(buffer, '\0', sizeof(buffer));
                        cout<<"Buffer: "<<buffer<<endl;
                        strcpy(buffer, retorno.c_str());
                    }
                }
            }

            //--------------- Guarde en memoria y se manda al server Backup -----------------------------
            else {
                cout<<"Se agregara dato a memoria... "<<endl;
                //manda el mensaje al servidor backup para que se agregue tambien
                client.send_data(nlohmann::json::parse(buffer).dump());
                cout << "Enviando dato a servidor backup: "<<encryption.start(nlohmann::json::parse(buffer).dump()).c_str()<<endl;
                memset(buffer, '\0', sizeof(buffer));
                //hace un reset del buffer para eliminar basura
                Mensaje agregoExcito;
                //revisa que el dato este en memoria local
                //si es cierto
                if(memory.search(mensaje.key)){
                    cout<<"Dato se encuentra en memoria local..."<<endl;
                    //entonces lo elimina de la memoria
                    //memory.deleteMemory(mensaje.key);
                    //y lo vuelve a agregar
                    //memory.addMemory(mensaje.key,mensaje.valor);
                    memory.memory.deleteWithKey(mensaje.key);
                    memory.memory.insertFirst(mensaje.key,mensaje.valor);
                    cout<<"Actualizando..."<<endl;
                    agregoExcito.estadoDeMemoria = memory.memorySize;
                    //hay que mandar de vuelta el mensaje para que el cliente cambie la cantidad de memoria de los servidores
                    to_json(j,agregoExcito);
                    //retorna al cliente el valor de la memoria del servidor
                    strcpy(buffer, j.dump().c_str());
                }
                else{
                    cout<<"Dato no se encuentra en memoria local..."<<endl;
                    //si es cierto entonces agrega el dato a memoria
                    //memory.addMemory(mensaje.key, mensaje.valor);
                    memory.memory.insertFirst(mensaje.key,mensaje.valor);
                    agregoExcito.estadoDeMemoria = memory.memorySize;
                    cout<<"Agregando..."<<endl;
                    //hay que mandar de vuelta el mensaje para que el cliente cambie la cantidad de memoria de los servidores

                    to_json(j,agregoExcito);
                    //retorna al cliente el valor de la memoria del servidor
                    strcpy(buffer, j.dump().c_str());
                }
                string recibido = client.receive(1024);
                cout <<"Mensaje del backup recibido: "<<encryption.start(recibido).c_str()<<endl;
            }
            if ((send(client_fd,buffer, strlen(buffer),0))== -1) {
                fprintf(stderr, "Failure Sending Message\n");
                close(client_fd);
                break;
            }
            string encryptedMsg = buffer;
            printf("Server:Msg being sent: %s\nNumber of bytes sent: %zu",encryption.start(encryptedMsg).c_str(), strlen(buffer));
            memset(buffer, '\0', sizeof(buffer));
            cout<<"\n--------------------------------------------------------------------------------------------------"<<endl;
        } //End of Inner While...
        //Close Connection Socket
        close(client_fd);
    } //Outer While

    close(socket_fd);
    return 0;
} //End of main

#endif //RMLIBSERVER_SERVERSOCKET_H

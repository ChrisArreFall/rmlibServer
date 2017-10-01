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

#define BACKLOG 10

class Server{
public:
    inline Server() = default;

    Memory memory;
    inline int serverStart(int port);
};

inline void to_json(nlohmann::json& j, const Mensaje& m) {
    j = nlohmann::json{{"estadoDeMemoria", m.estadoDeMemoria},
                       {"valor",           m.valor},
                       {"key",             m.key},
                       {"busqueda",        m.busqueda},
                       {"eliminar",        m.eliminar},
    };
}
inline void from_json(const nlohmann::json& j, Mensaje& m){
    m.estadoDeMemoria = j.at("estadoDeMemoria").get<int>();
    m.valor = j.at("valor").get<string>();
    m.key = j.at("key").get<int>();
    m.busqueda = j.at("busqueda").get<bool>();
    m.eliminar = j.at("eliminar").get<bool>();
}

inline int Server::serverStart(int port) {
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
            nlohmann::json j;
            Mensaje mensaje;
            from_json(nlohmann::json::parse(buffer),mensaje);
            cout<< mensaje.valor<<endl;
            if(mensaje.eliminar){
                memset(buffer, '\0', sizeof(buffer));
                if(memory.search(mensaje.key)){
                    memory.deleteMemory(mensaje.key);
                    string resultado = "Eliminado Satisfacotiamente...\0";
                    strcpy(buffer, resultado.c_str());
                }
                else{
                    string resultado = "No esta en lista...\0";
                    strcpy(buffer, resultado.c_str());
                }
            }
            else if(mensaje.busqueda) {
                if (memory.search(mensaje.key)) {
                    Mensaje temp;
                    temp.valor = memory.memory.search(mensaje.key)->value;
                    to_json(j,temp);
                    memset(buffer, '\0', sizeof(buffer));
                    strcpy(buffer, j.dump().c_str());
                }
                else{
                    string resultado = "No esta en lista...\0";
                    strcpy(buffer, resultado.c_str());
                }
            }
            else {
                memset(buffer, '\0', sizeof(buffer));
                memory.addMemory(mensaje.key, mensaje.valor);
                string resultado = "Se ha agregado satisfactoriamente...\0";
                strcpy(buffer, resultado.c_str());

            }
            if ((send(client_fd,buffer, strlen(buffer),0))== -1) {
                fprintf(stderr, "Failure Sending Message\n");
                close(client_fd);
                break;
            }
            printf("Server:Msg being sent: %s\nNumber of bytes sent: %zu",buffer, strlen(buffer));
        } //End of Inner While...
        //Close Connection Socket
        close(client_fd);
    } //Outer While

    close(socket_fd);
    return 0;
} //End of main

#endif //RMLIBSERVER_SERVERSOCKET_H

/*
 nlohmann::json j;
            Mensaje mensaje;
            from_json(nlohmann::json::parse(buffer),mensaje);
            if(mensaje.eliminar){
                if(memory.search(mensaje.key)){
                    memory.deleteMemory(mensaje.key);
                }
                else{

                }
            }
            else if(mensaje.busqueda){
                if(memory.search(mensaje.key)){
                    string msg = memory.memory.search(mensaje.key)->value;
                    strcpy(buffer,msg.c_str());
                    if ((send(client_fd,buffer, strlen(buffer),0))== -1) {
                        fprintf(stderr, "Failure Sending Message\n");
                        close(client_fd);
                        break;
                    }
                    printf("Server:Msg being sent: %s\nNumber of bytes sent: %zu",buffer, strlen(buffer));
                }
            }
            else {
                memory.addMemory(mensaje.key,mensaje.valor);
            }

 */
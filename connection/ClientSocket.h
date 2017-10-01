//
// Created by Chris on 9/30/17.
//

#ifndef RMLIBSERVER_CLIENTSOCKET_H
#define RMLIBSERVER_CLIENTSOCKET_H



#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent

using namespace std;

/**
    TCP Client class
*/
class tcp_client
{
private:
    std::string address;
    int port;
    struct sockaddr_in server;

public:
    tcp_client();
    int sock;
    bool conn(string, int);
    bool send_data(string data);
    string receive(int);
};

#endif //RMLIBSERVER_CLIENTSOCKET_H

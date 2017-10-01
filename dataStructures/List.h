//
// Created by Chris on 9/30/17.
//

#ifndef RMLIBSERVER_LIST_H
#define RMLIBSERVER_LIST_H


#include <vector>
#include<iostream>
//#include<conio.h>
#include<cstdlib>
using namespace std;
class Node {
public:
    int key;
    string value;
    int tamano;
    Node* next;
};


class List :public Node {

    Node *first,*last;
public:
    List() {
        first=NULL;
        last=NULL;
    }
    void create(int key, string value);
    void insertOnPos(int pos,int key, string value);
    void insertFirst(int key,string value);
    void insertLast(int key,string value);
    void deleteWithPos(int pos);
    void deleteFirst();
    void deleteLast();
    void deleteWithKey(int key);
    void display();
    Node *search(int);
    bool searchBool(int key);
};

#endif //RMLIBSERVER_LIST_H

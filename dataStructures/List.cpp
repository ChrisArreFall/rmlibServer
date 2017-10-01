
//
// Created by Chris on 9/26/17.
//


#include "List.h"


void List::create(int key, string value)
{
    Node *temp;
    temp=new Node;
    temp->key=key;
    temp->value=value;
    temp->tamano= value.size();
    temp->next=nullptr;
    if(first==nullptr)
    {
        first=temp;
        last=first;
    }

    else
    {
        last->next=temp;
        last=temp;
    }
}
void List::insertOnPos(int pos,int key, string value){
    Node *prev,*cur;
    prev=nullptr;
    cur=first;
    int count=1;
    Node *temp=new Node;
    temp->key=key;
    temp->value=value;
    temp->tamano=value.size();
    temp->next=nullptr;

    while(count!=pos)
    {
        prev=cur;
        cur=cur->next;
        count++;
    }
    if(count==pos)
    {
        prev->next=temp;
        temp->next=cur;
    }
    else
        cout<<"Not Able to Insert"<<endl;

}
void List::deleteWithPos(int pos)
{
    Node *prev=nullptr,*cur=first;
    int count=1;


    while(count!=pos)
    {
        prev=cur;
        cur=cur->next;
        count++;
    }
    if(pos==1){
        deleteFirst();
    }
    else if(count==pos) {
        cout<<"\nDeleted Element in position: "<<pos<<endl;
        prev->next=cur->next;
    }
    else
        cout<<"Not Able to Delete"<<endl;


}



void List::insertFirst(int key,string value){
    Node *prev,*cur;
    prev=nullptr;
    cur=first;
    int count=1;
    Node *temp=new Node;
    temp->key=key;
    temp->value=value;
    temp->tamano=value.size();
    temp->next=nullptr;
    temp->next=first;
    first=temp;

}
void List::insertLast(int key,string value){

    Node *prev,*cur;
    prev=nullptr;
    cur=first;
    int count=1;
    Node *temp=new Node;
    temp->key=key;
    temp->value=value;
    temp->tamano=value.size();
    temp->next=nullptr;
    last->next=temp;
    last=temp;
}
void List::deleteFirst(){
    Node *prev=nullptr,*cur=first;
    int count=1;
    if(first!=nullptr) {
        cout<<"Deleted Element with key: "<<first->key<<endl;
        first=first->next;
    }
    else
        cout<<"Not Able to Delete"<<endl;

}
void List::deleteLast(){
    Node *prev=nullptr,*cur=first;
    int count=1;
    while(cur!=last)
    {
        prev=cur;
        cur=cur->next;
    }
    if(cur==last)
    {
        cout<<"Deleted Element with key: "<<cur->key<<endl;
        prev->next=nullptr;
        last=prev;
    }
    else
        cout<<"Not Able to Delete"<<endl;
}

void List::deleteWithKey(int key){
    int value,pos=0;
    bool flag=false;
    if(first==nullptr)
    {
        cout<<"Error: List is Empty"<<endl;
    }
    Node *temp;
    temp=first;
    while(temp!=nullptr)
    {
        pos++;
        if(temp->key==key)
        {
            flag=true;
            deleteWithPos(pos);
            cout<<"\nKey "<<key<<" was deleted at position: "<<pos<< endl;

        }
        temp=temp->next;
    }
    if(!flag)
    {
        cout<<"Element "<<key<<" not Found in the List"<<endl;
    }
}



void List::display() {
    Node *temp=first;
    if(temp==nullptr)
    {
        cout<<"List is Empty"<<endl;
    }
    while(temp!=nullptr)
    {
        cout<<"The value: "<< value << " has the key: " << temp->key << " with the size: " << temp->tamano<<endl;
        temp=temp->next;
    }
    cout<<"End of list..."<<endl;
}
Node *List::search(int key) {
    int value,pos=0;
    bool flag=false;
    if(first==nullptr)
    {
        cout<<"Error: List is Empty"<<endl;
        return nullptr;
    }
    Node *temp;
    temp=first;
    while(temp!=nullptr)
    {
        pos++;
        if(temp->key==key)
        {
            flag=true;
            cout<<"Element "<<key<<" is Found at "<<pos<<" Position"<<endl;
            return temp;
        }
        temp=temp->next;
    }
    if(!flag)
    {
        cout<<"Element "<<key<<" not Found in the List"<<endl;
    }
}

bool List::searchBool(int key) {
    int value,pos=0;
    bool flag=false;
    if(first==nullptr)
    {
        cout<<"\nError: List is Empty"<<endl;
        return nullptr;
    }
    Node *temp;
    temp=first;
    while(temp!=nullptr)
    {
        pos++;
        if(temp->key==key)
        {
            flag=true;
            cout<<"\nElement "<<key<<" is Found at "<<pos<<" Position"<<endl;
            return true;
        }
        temp=temp->next;
    }
    if(!flag)
    {
        cout<<"\nElement "<<key<<" not Found in the List"<<endl;
        return false;
    }
}
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Node{
private:
    string method, name, type;
    Node* next, *prev;
    Node(){};
    Node(string method, string name, string type){
        this->method = method;
        this->name = name;
        this->type = type;
        this->next = NULL;
        this->prev = NULL;
    };
    ~Node(){};
    friend class SymbolTable;
};

class SymbolTable{
private:
    Node *head, *tail;
    int count;
public:
    SymbolTable() {
        this->head = NULL;
        this->tail = NULL;
        this->count = 0;
    }
    void run(string filename);
    void insert(Node* &head, Node* &tail, Node* T, string dataLine, int level);
    void assign(Node* &head, Node* &tail, Node* T, string dataLine, int level);
    void lookup(Node* &head, Node* &tail, Node* T, string dataLine, int level);
    void print(Node* &head, Node* &tail, int level);
    void rprint(Node* &head, Node* &tail, int level);

    /*--------------------------------SUPPORT FUNCTION------------------------------*/

    bool isEmpty(Node* &head);
    bool contains(Node* head, Node *T);
    void print(Node* head);
    /*--------------------------------SUPPORT FUNCTION------------------------------*/
    
    // ~SymbolTable(){
    //     delete this->head;
    //     delete this->tail;
    //     this->head = NULL;
    //     this->tail = NULL;
    //     this->count = 0;
    // };
};
#endif
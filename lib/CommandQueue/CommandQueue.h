#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "Config.h"

class CommandQueue {
private:
    // Kich thuoc hang doi (Tuy chinh theo dung luong RAM)
    static const int QUEUE_SIZE = 20; 
    Instruction queue[QUEUE_SIZE];
    int head;
    int tail;
    int count;

public:
    CommandQueue();
    
    // Day lenh vao hang doi (Tra ve false neu day)
    bool push(Instruction cmd);
    
    // Lay lenh ra khoi hang doi
    Instruction pop();
    
    // Kiem tra hang doi trong hoac day
    bool isEmpty();
    bool isFull();
};

#endif
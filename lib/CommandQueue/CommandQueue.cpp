#include "CommandQueue.h"

CommandQueue::CommandQueue() {
    head = 0;
    tail = 0;
    count = 0;
}

bool CommandQueue::push(Instruction cmd) {
    if (isFull()) {
        return false;
    }
    
    queue[tail] = cmd;
    tail = (tail + 1) % QUEUE_SIZE;
    count++;
    
    return true;
}

Instruction CommandQueue::pop() {
    Instruction emptyCmd = {CMD_FEED, 0.0f};
    if (isEmpty()) {
        return emptyCmd;
    }
    
    Instruction cmd = queue[head];
    head = (head + 1) % QUEUE_SIZE;
    count--;
    
    return cmd;
}

bool CommandQueue::isEmpty() {
    return count == 0;
}

bool CommandQueue::isFull() {
    return count == QUEUE_SIZE;
}
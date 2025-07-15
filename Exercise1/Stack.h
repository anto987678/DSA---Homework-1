#ifndef STACK_H
#define STACK_H
#include <iostream>
#define MAX_STACK_SIZE 100

// kinda generic stack
template<typename T>
class Stack {
private:
    T data[MAX_STACK_SIZE];
    int topIndex;   // top elem index (starts at -1)

public:
    Stack() {
        topIndex = -1;  // stack's empty at init
    }

    void push(T item) {
        if (topIndex >= MAX_STACK_SIZE - 1) {
            std::cout << "Stack is full!\n";
            return;
        }
        topIndex++;
        data[topIndex] = item;    // pushing on top
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    T pop() {
        if (isEmpty()) {
            std::cout << "Stack is empty!\n";
            return T(); // default val if empty
        }
        // grab, then drop
        T popped = data[topIndex];
        topIndex--;
        return popped;
    }

    T peek() {
        if (isEmpty()) {
            std::cout << "Stack is empty!\n";
            return T();
        }
        return data[topIndex];
    }
};

#endif // STACK_H

#pragma once

using namespace std;
#include <iostream>

template <class T>
class Stack
{
private:
    T *stack;
    int top;
    int capacity;

public:
    Stack(int capacity);
    ~Stack();

    void push(T element);
    T *pop();
    T peek();
    bool isEmpty();
    bool isFull();
};

template <class T>
Stack<T>::Stack(int capacity)
{
    this->capacity = capacity;
    this->stack = new T[capacity];
    this->top = -1;
}

template <class T>
void Stack<T>::push(T element)
{
    if (this->isFull())
    {
        cout << "Stack Overflow" << endl;
    }
    else
    {
        this->stack[++this->top] = element;
    }
}

template <class T>
T *Stack<T>::pop()
{
    if (this->isEmpty())
    {
        cout << "Stack Underflow" << endl;
        return 0;
    }
    else
    {
        return &this->stack[this->top--];
    }
}

template <class T>
T Stack<T>::peek()
{
    if (this->isEmpty())
    {
        cout << "Stack Underflow" << endl;
        return 0;
    }
    else
    {
        return this->stack[this->top];
    }
}

template <class T>
bool Stack<T>::isEmpty()
{
    return this->top == -1;
}

template <class T>
bool Stack<T>::isFull()
{
    return this->top == this->capacity - 1;
}

template <class T>
Stack<T>::~Stack()
{
}

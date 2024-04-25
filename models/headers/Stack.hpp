#pragma once

using namespace std;
#include <iostream>

template <class T> // Template class for a stack
class Stack
{
private:
    T *stack;     // Array to store elements of the stack
    int top;      // Pointer to the top element of the stack
    int capacity; // Maximum capacity of the stack

public:
    Stack(int capacity); // Constructor to initialize a stack with a given capacity
    ~Stack();            // Destructor to free memory allocated for the stack

    void push(T element); // Pushes an element onto the stack
    T *pop();             // Pops an element from the stack
    T peek();             // Returns the top element of the stack without removing it
    bool isEmpty();       // Checks if the stack is empty
    bool isFull();        // Checks if the stack is full
};

/**
 * @brief Constructor to initialize a stack with a given capacity.
 *
 * @param capacity The maximum capacity of the stack.
 */
template <class T>
Stack<T>::Stack(int capacity)
{
    this->capacity = capacity;
    this->stack = new T[capacity]; // Allocate memory for the stack
    this->top = -1;                // Initialize top pointer to -1 (empty stack)
}

/**
 * @brief Pushes an element onto the stack.
 *
 * @param element The element to be pushed onto the stack.
 */
template <class T>
void Stack<T>::push(T element)
{
    if (this->isFull())
    {
        cout << "Stack Overflow" << endl; // Print an error message if stack is full
    }
    else
    {
        this->stack[++this->top] = element; // Increment top pointer and insert element
    }
}

/**
 * @brief Pops an element from the stack.
 *
 * @return Pointer to the popped element, or null if stack is empty.
 */
template <class T>
T *Stack<T>::pop()
{
    if (this->isEmpty())
    {
        cout << "Stack Underflow" << endl; // Print an error message if stack is empty
        return nullptr;                    // Return null pointer
    }
    else
    {
        return &this->stack[this->top--]; // Decrement top pointer and return pointer to popped element
    }
}

/**
 * @brief Returns the top element of the stack without removing it.
 *
 * @return The top element of the stack.
 */
template <class T>
T Stack<T>::peek()
{
    if (this->isEmpty())
    {
        cout << "Stack Underflow" << endl; // Print an error message if stack is empty
        return 0;                          // Return default value
    }
    else
    {
        return this->stack[this->top]; // Return the top element of the stack
    }
}

/**
 * @brief Checks if the stack is empty.
 *
 * @return True if the stack is empty, otherwise false.
 */
template <class T>
bool Stack<T>::isEmpty()
{
    return this->top == -1; // Return true if top pointer is -1, indicating an empty stack
}

/**
 * @brief Checks if the stack is full.
 *
 * @return True if the stack is full, otherwise false.
 */
template <class T>
bool Stack<T>::isFull()
{
    return this->top == this->capacity - 1; // Return true if top pointer is at capacity-1
}

/**
 * @brief Destructor to free memory allocated for the stack.
 */
template <class T>
Stack<T>::~Stack()
{
}

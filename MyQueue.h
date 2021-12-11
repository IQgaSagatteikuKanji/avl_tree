#include <iostream>
#include "HeadListRAII_new.h"


template<class Type>
class Stack
{
    HeadList<Type> stack;

public:

    void init(){};
    bool is_empty();
    void push(const Type&);
    void pop(Type&);
    void clear();
};

template<class Type>
class MyQueue
{
    HeadList<Type> queue;
    iterator<Type> tail;
    
public:
    MyQueue(){tail = queue.before_beg();};
    void init(){};
    bool is_empty();
    bool push(const Type&);
    bool pop(Type&);
    void clear();
};

template<class Type>
bool Stack<Type>::is_empty(){
    return stack.is_empty();
}

template<class Type>
void Stack<Type>::push(const Type& val){
     stack.push_front(val);
}

template<class Type>
void Stack<Type>::pop(Type& val){
    
    stack.pop_front(val);
}

template<class Type>
void Stack<Type>::clear(){
    stack.clear();
}

template<class Type>
bool MyQueue<Type>::is_empty(){
    return queue.is_empty();
}

template<class Type>
bool MyQueue<Type>::push(const Type& val){
    tail.add_next(val);
    tail++;
    return false;
}

template<class Type>
bool MyQueue<Type>::pop(Type& val){
    if(queue.beg() == tail)
        tail = queue.before_beg();
    return queue.pop_front(val);
}

template<class Type>
void MyQueue<Type>::clear(){
    queue.clear();
    tail = queue.before_beg();
}
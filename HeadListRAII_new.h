#pragma once
template<class T>
struct NODE
{
    T data;
    NODE<T>* next;
};

template<class T> class HeadList;


template<class T>
class iterator
{
    NODE<T>* ptr = nullptr;
public:
    friend class HeadList<T>;

    T& operator*();

    iterator& operator++();
    iterator operator++(int);
    bool is_empty();
    void delete_next();
    void add_next(const T&);
    bool operator==(iterator<T> second){return ptr == second.ptr;};
};

template<class T>
class HeadList
{
    NODE<T>* head = nullptr;

public:
    friend class iterator<T>;

    void push_front(const T&);
    bool pop_front(T&);
    HeadList();
    HeadList<T>& operator=(const HeadList<T>&);
    HeadList(const HeadList&);
    ~HeadList();
    void clear();
    bool is_empty();
    iterator<T> beg();
    iterator<T> before_beg();
};



template<class T>
iterator<T>& iterator<T>::operator++(){
    if(ptr != nullptr)
        ptr = ptr->next;
    return *this;
}

template<class Type>
iterator<Type> iterator<Type>::operator++(int){
    iterator<Type> return_iter(*this);
    if(ptr != nullptr)
        ptr = ptr->next;
    
    return return_iter;
} 

template<class Type>
Type& iterator<Type>::operator*(){
    return ptr->data;
}

template<class Type>
bool iterator<Type>::is_empty(){
    return ptr == nullptr;
}

template<class Type>
void iterator<Type>::delete_next(){
    NODE<Type>* holder = ptr->next;
    if(holder != nullptr){
        ptr->next = holder->next;
        delete holder;
        holder = nullptr;
    }
}

template<class Type>
void iterator<Type>::add_next(const Type& elem){
    NODE<Type> (*holder) = ptr->next;
    ptr->next = new NODE<Type>;
    ptr->next->data = elem;
    ptr->next->next = holder;
}



template<class T>
HeadList<T>::HeadList(){
    head = new NODE<T>;
    head->next = nullptr;
}

template<class T>
bool HeadList<T>::is_empty(){
    return head->next == nullptr;
}

template<class T>
iterator<T> HeadList<T>::beg(){
    iterator<T> iter;
    iter.ptr = head->next;
    return iter;
}

template<class T>
iterator<T> HeadList<T>::before_beg(){
    iterator<T> iter;
    iter.ptr = head;
    return iter;
}

template<class T>
HeadList<T>& HeadList<T>::operator=(const HeadList<T>& source){
    if(&source == this) return *this;

    clear();
    const NODE<T> (*src) = source.head->next;
    NODE<T>* dest = head;

    while(src != nullptr){
        dest->next = new NODE<T>;
        dest = dest->next;
        dest->data = src->data;
        dest->next = nullptr;
        src= src->next;
    }
    return *this;
}

template<class T>
HeadList<T>::HeadList(const HeadList<T>& source){
    head = new NODE<T>;
    head->next = nullptr;

    const NODE<T> (*src) = source.head->next;
    NODE<T>* dest = head;

    while(src != nullptr){
        dest->next = new NODE<T>;
        dest = dest->next;
        dest->data = src->data;
        dest->next = nullptr;
        src= src->next;
    }
}


template <class Type>
void HeadList<Type>::push_front(const Type& val){
    NODE<Type>* holder = head->next;
    head->next = new NODE<Type>;
    head->next->data = val;
    head->next->next = holder;

    holder = nullptr;
}

template<class Type>
bool HeadList<Type>::pop_front(Type& return_value){
    if(head->next == nullptr) return true;
    NODE<Type>* holder = head->next;

    bool errors_ocurred = is_empty(); 

    if(!errors_ocurred){
        head->next = holder->next;
        return_value = holder->data;
        delete holder;
        holder = nullptr;
    }
    return errors_ocurred;
}

template<class Type>
void HeadList<Type>::clear(){
    NODE<Type> *holder;
while(head->next != nullptr){
    holder = head->next;
    head->next = holder->next;
    delete holder;
}
holder = nullptr;
}

template<class Type>
HeadList<Type>::~HeadList(){
    clear();
    delete head;
}
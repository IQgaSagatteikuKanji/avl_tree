#pragma once
#include "MyQueue.h"

enum balance{
    LEFT_HEAVY = -2,
    LEFT_BALANCED = -1,
    BALANCED = 0,
    RIGHT_BALANCED = 1,
    RIGHT_HEAVY =2
};

template<class Type>
struct TreeNode{
    Type data;
    size_t height_of_subtree = 0;
    TreeNode    *left,
                *right;
};

//doesnt work, not implemented
// template<class Type>
// class tree_iterator{
//     Stack<TreeNode<Type>*> route;

// public:
//     friend AVL_tree<Type>;

//     //iterator& operator++();
//     //iterator& operator--();
    
//     Type& operator*();
// };

template<class Type>
class AVL_tree{
    TreeNode<Type>* root;

    void rotate_left(TreeNode<Type>* &);
    void rotate_right(TreeNode<Type>* &);
    void LR_rotation(TreeNode<Type>* &);
    void RL_rotation(TreeNode<Type>* &);

    bool add_node(TreeNode<Type>* &, const Type &);
    void simple_delete(TreeNode<Type> * &);
    TreeNode<Type> * balance_path(Stack<TreeNode<Type>*> &, TreeNode<Type>* &);
    balance calculate_balance(TreeNode<Type>* );
    size_t calculate_height(TreeNode<Type>* );
    void clear(TreeNode<Type> * &);
public:
    //friend tree_iterator<Type>;
    AVL_tree():root(nullptr){};
    AVL_tree& operator=(const AVL_tree&) = delete;
    AVL_tree(const AVL_tree&) = delete;
    ~AVL_tree();

    bool empty();
    void clear();
    bool insert(const Type &);
    void erase(const Type &);

    bool contains(const Type &); 

    void print_by_layers();

    //iterator<Type> beg();
    //iterator<Type> end();
};


template<class Type>
void AVL_tree<Type>::print_by_layers(){
    std::cout<<root->height_of_subtree<<"\n";
    
    MyQueue<TreeNode<Type>*> queue;
    queue.push(root);
    TreeNode<Type>* iter;
    bool code = false;

    while(!queue.is_empty() && !code){
        code = queue.pop(iter);
        std::cout<<iter->data<<" ";
        if(iter->left != nullptr){
            queue.push(iter->left);
        }
        if(iter->right != nullptr)
            queue.push(iter->right);
    }
}
template<class Type>
bool AVL_tree<Type>::empty(){
    return root == nullptr;
}


template<class Type>
bool AVL_tree<Type>::contains(const Type& elem){
    TreeNode<Type>* iter = root;
    bool already_contains =false;

    while(iter != nullptr && !already_contains){
        if(iter->data == elem)
            already_contains = true;
        
        if(elem > iter->data)
            iter = iter->right;
        else if(elem < iter->data)
            iter= iter->left;
    }

    return already_contains;
}

template<class Type>
void AVL_tree<Type>::rotate_left(TreeNode<Type> *&root){
    TreeNode<Type>* right_child_of_root = root->right;
    TreeNode<Type>* moved_child = right_child_of_root->left;
    TreeNode<Type> *buffer = root;
    
    root = right_child_of_root;
    root->left = buffer;
    buffer->right = moved_child;
    calculate_height(buffer);
    calculate_height(root);
}

template<class Type>
void AVL_tree<Type>::rotate_right(TreeNode<Type> *&root){
    TreeNode<Type>* left_child_of_root = root->left;
    TreeNode<Type>* moved_child = left_child_of_root->right;
    TreeNode<Type> *buffer = root;
    
    root = left_child_of_root;
    root->right = buffer;
    buffer->left = moved_child;
    calculate_height(buffer);
    calculate_height(root);
}

template<class Type>
void AVL_tree<Type>::LR_rotation(TreeNode<Type>* & root){
    rotate_right(root->right);
    rotate_left(root);
}

template<class Type>
void AVL_tree<Type>::RL_rotation(TreeNode<Type>* & root){
    rotate_left(root->left);
    rotate_right(root);
}

template<class Type>
void AVL_tree<Type>::clear(TreeNode<Type> * &root){
    if(root == nullptr) return;
    clear(root->left);
    clear(root->right);
    delete root;
}

template<class Type>
void AVL_tree<Type>::clear(){
    clear(root);
    root = nullptr;
}

template<class Type>
AVL_tree<Type>::~AVL_tree(){
    clear();
}

template<class Type>
void AVL_tree<Type>::simple_delete(TreeNode<Type>* &root){
    if(root->left == nullptr && root->right == nullptr){
        delete root;
        root = nullptr;
    }
    else{
        TreeNode<Type>* first_config_left = root->left,
                        *first_config_right = root->right;

        Stack<TreeNode<Type>*> path;
        path.push(root);

        TreeNode<Type> * iter = root;
        if(iter->left == nullptr){
            iter = iter->right;
            delete root;
            root = iter;
            return;
        }

        iter = iter->left;
        if(iter->right == nullptr){
            iter->right = root->right;
            delete root;
            root = iter;
            return;
        }

        while(iter->right != nullptr){
            path.push(iter);
            iter = iter->right;
        }

        TreeNode<Type>* holder = root;
        path.pop(iter);
        path.push(iter);

        root->data = iter->right->data;
        delete iter->right;
        iter->right = nullptr;
        
        
        root->left = first_config_left;
        root->right = first_config_right;

        root = balance_path(path,root);
    }
}

template<class Type>
void AVL_tree<Type>::erase(const Type& value){
    TreeNode<Type>* iter = root;
    Stack<TreeNode<Type> *> path;

    if(root->data == value){
        simple_delete(root);
        return;
    }

    bool found = false;
    while(iter != nullptr && !found){
        if(iter->data == value){
            found = true;
            TreeNode<Type> * parent;
            TreeNode<Type> **ptr_to_iter;
            path.pop(parent);
            path.push(parent);

            if(parent->right == iter)
                ptr_to_iter = &(parent->right);
            else ptr_to_iter = &(parent->left);
            std::cout<<"deleting "<<(*ptr_to_iter)->data;
            simple_delete(*ptr_to_iter);
            root = balance_path(path, root);
        }
        else{
            path.push(iter);
            if(value > (iter)->data)
                iter = (iter)->right;
            else iter = ((iter)->left);
        }
    }
}

template<class Type>
TreeNode<Type>* AVL_tree<Type>::balance_path(Stack<TreeNode<Type>*> & path, TreeNode<Type>* &root){
    TreeNode<Type> *iter, *buffer, **ptr_to_iter;
    balance balance_factor;

    while(!path.is_empty()){
        path.pop(iter);
        calculate_height(iter);

        balance_factor = calculate_balance(iter);
        //std::cout<<iter->data<<":bf = "<<balance_factor<<"\n";
        if(balance_factor == LEFT_HEAVY || balance_factor == RIGHT_HEAVY){
            if(!path.is_empty()){
            path.pop(buffer);
            path.push(buffer);
            if(buffer->left == iter)
                ptr_to_iter = &(buffer->left);
            if(buffer->right == iter)
                ptr_to_iter = &(buffer->right);
            }   else ptr_to_iter = &root;
        }

        if(balance_factor == RIGHT_HEAVY){
            TreeNode<Type>* subtree = iter->right;
            balance subtree_balfactor = calculate_balance(subtree);
            if(subtree_balfactor >= BALANCED){
                rotate_left(*ptr_to_iter);
            } else LR_rotation(*ptr_to_iter);
        }

        if(balance_factor == LEFT_HEAVY){
            TreeNode<Type> *subtree = iter->left;
            balance subtree_balfactor = calculate_balance(subtree);
            if(subtree_balfactor <=BALANCED)
                rotate_right(*ptr_to_iter);
            else RL_rotation(*ptr_to_iter);
        }
    }
    return root;
}
template<class Type>
balance AVL_tree<Type>::calculate_balance(TreeNode<Type>* node){
    int height_of_right = 0,
        height_of_left = 0;
    if(node->right != nullptr)
            height_of_right = calculate_height(node->right);
        
    if(node->left != nullptr)
            height_of_left =  calculate_height(node->left);

    return (balance) (height_of_right - height_of_left);
}

template<class Type>
size_t AVL_tree<Type>::calculate_height(TreeNode<Type>* node){
        int height_of_right = 0,
        height_of_left = 0;
        if(node->right != nullptr)
            height_of_right = node->right->height_of_subtree;
        
        if(node->left != nullptr)
            height_of_left = node->left->height_of_subtree;

        if(height_of_left>height_of_right){
            node->height_of_subtree = height_of_left+1;
            return height_of_left+1;
        }

        node->height_of_subtree = height_of_right+1;
        return height_of_right+1;
}

template<class Type>
bool AVL_tree<Type>::insert(const Type &value){
    TreeNode<Type> *iter = root;
    Stack<TreeNode<Type>*> path;
    bool added = false;

    while(iter != nullptr && !added){
        if((iter)->data == value)
            added = true;
        else{
            path.push(iter);
            if(value > (iter)->data)
                iter = (iter)->right;
            else iter = ((iter)->left);
        }
    }

    if(!added){
        if(root == nullptr)
            add_node(root, value);
        
        if(!path.is_empty()){
            path.pop(iter);
            path.push(iter);
            if(value> iter->data)
                add_node(iter->right, value);
            else add_node(iter->left, value);

            root = balance_path(path, root);
        }
    }

    return !added;
}

template<class Type>
bool AVL_tree<Type>::add_node(TreeNode<Type> * &place ,const Type &value){

    place = new TreeNode<Type>;
    if(place != nullptr){
        place->data = value;
        place->height_of_subtree = 0;
        place->left = nullptr;
        place->right = nullptr;
    }
    return place == nullptr;
}
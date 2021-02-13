//List342.h
//CSS 342 Program3
//Implemented by Kray Nguyen on 11/16/2020
#include "Child.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

// This templatized class store different types of object and built for a fully ordered list
// implementation of linked list functions are included to perform list modification
template <class ItemType>
class List342 {
friend ostream& operator<< (ostream &outStream, const List342<ItemType> &other){
    Node* current = other.head;
    while(current != nullptr){
        cout << *current -> data;
        current = current -> next;
    }
    return outStream;
}
public:
    // constructors - Destructor
    List342();
    List342(const List342 &other);
    ~List342();

    // required functions
    bool BuildList(string fileName);
    bool Insert(ItemType *obj);
    bool Remove(ItemType target, ItemType &result);
    bool Peek(ItemType target, ItemType &result) const;
    bool isEmpty() const;
    void DeleteList();
    bool Merge(List342 &list1);

    // operator overloads
    List342<ItemType> & operator=(const List342<ItemType> &other);
    List342<ItemType> operator+(const List342<ItemType> &other);
    List342<ItemType> & operator+=(List342<ItemType> &other);
    bool operator==(const List342<ItemType> &other) const;
    bool operator!=(const List342<ItemType> &other) const;

private:
    struct Node{
        ItemType *data;
        Node *next;
    };
    Node* head;
};

template <class ItemType>
List342<ItemType>::List342(){
    this -> head = nullptr;
}

template <class ItemType>
List342<ItemType>::List342(const List342<ItemType> &other){
    this -> head = nullptr;
    *this = other;
}

template <class ItemType>
List342<ItemType>::~List342(){
    DeleteList();
    delete head;
}

// Given a string as a filename to open and read objects from the files,
// then build an ordered list
// Precondition: list passed in is well performed for the object being read into
// Postcondition: returns true if the list are built sucessfully
template <class ItemType>
bool List342<ItemType>:: BuildList(string fileName){
    ifstream openFile;
    openFile.open(fileName);
    if(openFile.is_open()){
        while(!openFile.eof()){
            ItemType item;
            openFile >> item;
            Insert(&item);
        }
        openFile.close();
    } else {
        cout << "File name not found" << endl;
        return false;
    }
    return true;
}

// This function inserts an object in the list in the correct place
// Precondition: object passed in is valid
// Postcondition: return true if insert successfully, no duplicates are allowed
template <class ItemType>
bool List342<ItemType>::Insert(ItemType *object){
    
    bool retVal = true;
    Node* tempNode = new Node; 
    ItemType *obj = new ItemType(*object);
    
    Node* currentNode;
    Node* previousNode;
    tempNode -> data = obj;

    // insert at the front if list is empty
    if(isEmpty() || *head -> data > *tempNode -> data){
        tempNode -> next = head;
        head = tempNode;
    } // for the remaining cases... 
    else {
        currentNode = head -> next;
        previousNode = head;

        // traverse through linked list and insert in correct order
        while(currentNode != nullptr && *tempNode -> data > *currentNode -> data){
            previousNode = currentNode;
            currentNode = currentNode -> next;
        }
        tempNode -> next = currentNode;
        previousNode -> next = tempNode;
    }

    // After that, traverse and remove duplicates, then return false
    currentNode = head;
    if(currentNode != nullptr && currentNode -> next != nullptr){
        while(currentNode != nullptr && currentNode -> next != nullptr){
            if(*(currentNode -> data) == (*(currentNode -> next -> data))){
                tempNode = currentNode -> next ;
                currentNode -> next = currentNode->next->next;
                delete tempNode->data;
                delete tempNode;
                retVal = false;
            } else {
                currentNode = currentNode -> next;
            }
        }
    }
    //delete tempNode;
    return retVal;
}

// This function removes all items from the list with appropriate deallocation
template <class ItemType>
void List342<ItemType>::DeleteList(){

   Node* temp;
   while(head != nullptr) {
       temp = head;
       head = head -> next;
       if (temp -> data != nullptr) {
            delete temp -> data;
       }
       delete temp;
       temp = nullptr;
   }
   head = nullptr;
}

// This function removes target item from the list
// Precondition: target passed in by value, result passed in by reference
// Postcondition: result is updated equals to item, return true if the item was found and removed 
template <class ItemType>
bool List342<ItemType>::Remove(ItemType target, ItemType &result){
    if(head == nullptr){
        return false;
    } else {
        Node* currentNode = head;
        Node* previousNode = head;
        // traverse through the linked list
        while(currentNode != nullptr){
            if(*(currentNode->data) == target){
                result = *currentNode->data;
                // remove at the front of the linked list
                if(currentNode == head){
                    head = head -> next;
                    delete currentNode->data;
                    delete currentNode;
                    previousNode = head;
                } // for the remaining cases...
                else {
                    previousNode -> next = currentNode -> next;
                    delete currentNode->data;
                    delete currentNode;
                    currentNode = previousNode -> next;
                }
                return true;
            } // continue traverse if doesn't find a matching
            else {
                previousNode = currentNode;
                currentNode = currentNode -> next;
            }
        }
    }
    return false;
}

// This function looks for the target item
// Precondition: target passed in by value, result passed in by reference
// Postcondition: result is updated equals to the item, the item is not removed from the lsit
// return true if the item was found 
template <class ItemType>
bool List342<ItemType>::Peek(ItemType target, ItemType &result) const{
    Node* currentNode = head;
    while(currentNode -> next != nullptr){
        if(*(currentNode->data) == target){
            result = *(currentNode -> data);
            return true;
        } else {
            currentNode = currentNode -> next;
        }
    }
    return false;
}

template <class ItemType>
bool List342<ItemType>::isEmpty() const{
    if(this -> head == nullptr){
        return true;
    }
    return false;
}

// this function merges 2 linked list
template <class ItemType>
bool List342<ItemType>::Merge(List342 &list1){
    Node* tempNode = nullptr;
    Node* currentList1;
    Node* currentList2;
    currentList1 = this -> head;
    currentList2 = list1.head;

    if(this == &list1 || list1.head == nullptr){
        return false;
    }

    if(this -> head == nullptr){
        this -> head = list1.head;
        list1.head = nullptr;
        return true;
    }
    // determine which head is larger 
    if(*head -> data > *list1.head -> data){
        this->head = list1.head;
    }
    // algortihm compares the head of 2 lists each traversal and relinking the node
    // as necessary
    while(currentList1 != nullptr && currentList2 != nullptr){
        if(*currentList1 -> data < *currentList2 -> data){
            tempNode = currentList1;
            currentList1 = currentList1 -> next;
        } else {
            if(tempNode != nullptr){
                tempNode -> next = currentList2;
            }
            tempNode = currentList2;
            currentList2 = currentList2 -> next;
            tempNode -> next = currentList1;
        }
    }
    // when first list is empty, simply relink the node to second list
    if(currentList1 == nullptr){
        tempNode -> next = currentList2;
    }
    
    // remove duplicates after merging
    Node* currentNode = this -> head;
    Node* storeNode; 
    if(currentNode != nullptr && currentNode -> next != nullptr){
        while(currentNode != nullptr && currentNode -> next != nullptr){
            if(*currentNode -> data == *currentNode -> next -> data){
                storeNode = currentNode->next;
                currentNode -> next = currentNode->next->next;
                //delete storeNode->data;
                delete storeNode->data;
                delete storeNode;
            } else {
                currentNode = currentNode -> next;
            }
        }
    }
    list1.head = nullptr;

    return true;
}

// Assignment makes dep copy of all new memory
template <class ItemType>
List342<ItemType>& List342<ItemType>::operator=(const List342<ItemType> &other){
    if(this == &other || other.head == nullptr){
        return *this;
    }
    // delete the current list first before doing deep copy
    this -> DeleteList();
    Node* currentNode = other.head;
    while(currentNode != nullptr){
        this->Insert(currentNode->data);
        currentNode = currentNode -> next;
    }
    return *this;
}

// adds the 2 lists together
// precondition: lists are sorted
// Postcondition: return list is also sorted. returns a new list after adding.
template <class ItemType>
List342<ItemType> List342<ItemType>::operator+(const List342<ItemType> &other){
    List342<ItemType> returnList1 = *this; 
    List342<ItemType> returnList2 = other;
    returnList1 += returnList2;
    return returnList1;
}

// adds the 2 lists together
// precondition: lists are sorted
// Postcondition: return list is also sorted.
template <class ItemType>
List342<ItemType>& List342<ItemType>::operator+=(List342<ItemType> &other){
    List342 <ItemType> temp = other;
    if(other.head == nullptr){
        return *this;
    } else {
        DeleteList();
        this->Merge(temp);
    }
    return *this;
}

template <class ItemType>
bool List342<ItemType>::operator==(const List342<ItemType> &other) const{
    Node* current_lhs = this -> head;
    Node* current_rhs = other.head;
    while(current_lhs != NULL && current_rhs != NULL){
        if(*current_lhs -> data != *current_rhs -> data){
            return false;
        }
        current_lhs = current_lhs -> next;
        current_rhs = current_rhs -> next;
    }
    return true;
}

template <class ItemType>
bool List342<ItemType>::operator!=(const List342<ItemType> &other) const{
    Node* current_lhs = this -> head;
    Node* current_rhs = other.head;

    while(current_lhs != NULL && current_rhs != NULL){
        if(current_lhs -> data == current_rhs -> data){
            return false;
        }
        current_lhs = current_lhs -> next;
        current_rhs = current_rhs -> next;
    }
    return true;
}

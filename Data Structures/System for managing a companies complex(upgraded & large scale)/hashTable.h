#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include "library2.h"

#define MULTIPLY 2
#define FIRST_SIZE 10

template<typename K, typename D>
class Ht_item
{
    private:
        K* key;
        D* data;
        Ht_item<K,D>* next;

    public:
        Ht_item(K* key_, D* data_): key(key_), data(data_), next(nullptr){}
        ~Ht_item()
        {
            if(key!=nullptr)
                delete key;
            key=nullptr;
            if(data!=nullptr)
                delete data;
            data=nullptr;
        }

        K* getKey() { return key; }
        K getKeyV() { return *key; }
        D* getData() { return data; }
        D getDataV() { return *data; }
        Ht_item<K,D>* getNext() {return next; }

        void setKey(K* key_) { key=key_; }
        void setKeyV(K key_) { *key=key_; }
        void setData(D* data_) { data=data_; }
        void setDavaV(D data_) {*data = data_; }
        void setNext(Ht_item<K,D>* next_) {next=next_; }

        /*
        void operator=( Ht_item<K,D>* h2)
        {
            this = *h2;
        }
        */

        StatusType AddNode(Ht_item<K,D>* newNode){
            if( this->getKeyV() == newNode->getKeyV() )//key already exist
                return FAILURE;
            if(this->getNext()==nullptr) //only 1 object in the list
            {
                this->setNext(newNode);
                return SUCCESS;
            }
            Ht_item<K,D>* iter = this->getNext();
            while(iter->getNext()!=nullptr) //advance to last node in the list
            {
                if(iter->getKeyV() == newNode->getKeyV()) //key exist in the list
                    return FAILURE;
                iter=iter->getNext();
            }
            if( iter->getKeyV() == newNode->getKeyV())
                return FAILURE;    
            iter->setNext(newNode);
            return SUCCESS;
        }

        friend bool operator==(const Ht_item<K,D>& h1, const Ht_item<K,D>& h2)
        {
            if(h1.key==h2.key)
                return true;
            return false;
        }
        


};        


template<typename K, typename D>
class hashTable
{
    private:
        Ht_item<K,D> **arr;
        int counter;
        int size;

    public:
        hashTable(): arr(nullptr), counter(0), size(FIRST_SIZE) {
            try {
                arr= createArray(FIRST_SIZE);
            } catch (const std::bad_alloc& e) {
                throw;
            }
        } 
        ~hashTable() 
        {
            if(arr!=nullptr)
            {
                deleteArray(arr,size);
            }
        }

        Ht_item<K,D>** getArr() {return arr; }
        int getCounter() {return counter; }
        int getSize() {return size; }
        
        void addCounter() {counter++; }
        void decreaseCounter() {counter--; }
        void setCounter(int counter_) { counter=counter_; }
        void setSize(int num) {size=num; }


        StatusType insert(K k, D d);
        StatusType remove(K key);
        D* find(K key);

        void deleteArray(Ht_item<K,D>** array, int size);
        Ht_item<K,D>** createArray(int size);
        int hashFunction(K key);
        StatusType reHash();
        StatusType copyArray(Ht_item<K,D>** arrayToCopy , int size);
        StatusType mergeHTs(hashTable<K,D>* targetHt);
};

template<typename K, typename D>
void hashTable<K,D>::deleteArray(Ht_item<K,D>** array, int size){
    if(array==nullptr)
        return;
    for(int i=0 ; i<size ; i++)
    {
        if(array[i]!=nullptr)
        {
            Ht_item<K,D>* toDelete = array[i];
            Ht_item<K,D>* temp = nullptr;
            while(toDelete)
            {
                temp=toDelete->getNext();
                delete toDelete;
                toDelete=temp;
            }
            array[i]=nullptr;
        }
            
    }
    delete[] array;
    array=nullptr;
}

template<typename K, typename D>
Ht_item<K,D>** hashTable<K,D>::createArray(int size){
    try{
        Ht_item<K,D>** array = new Ht_item<K,D> *[size];
        for (int i = 0; i < size; i++) {
            array[i] = nullptr;
        }
        return array;
    }
    catch (const std::bad_alloc& e) {
        return nullptr;
    }

}

template<typename K, typename D>
int hashTable<K,D>::hashFunction(K key){
    return key % this->getSize();
}

template<typename K, typename D>
StatusType hashTable<K,D>::reHash(){
    int arraySize = getSize();
    if(getCounter()+1 == arraySize) //expand
    {
        try
        {
            Ht_item<K,D>** temp = getArr();
            this->arr=nullptr;
            this->arr=createArray(arraySize * MULTIPLY);
            this->setCounter(0);
            this->setSize(arraySize * MULTIPLY);
            arraySize = arraySize * MULTIPLY;
            this->copyArray(temp, (arraySize / MULTIPLY));
            deleteArray(temp, (arraySize / MULTIPLY));
            return SUCCESS;
        }
        catch(const std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
        
    }
    if(getCounter() == (arraySize/MULTIPLY))  //shrink
    {
        try
        {
            Ht_item<K,D>** temp = this->arr;
            this->arr=nullptr;
            this->arr=createArray(arraySize / MULTIPLY);
            this->setCounter(0);
            this->setSize(arraySize / MULTIPLY);
            arraySize = arraySize/MULTIPLY;
            this->copyArray(temp, (arraySize * MULTIPLY));
            deleteArray(temp, (arraySize * MULTIPLY));
            return SUCCESS;
        }
        catch(const std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
        
    }
    //no need to do reHash
    return SUCCESS;
}

template<typename K, typename D>
StatusType hashTable<K,D>::insert(K k, D d){
    //K* key_;
    if((getCounter()+1) == getSize())
    {
        if(reHash()==FAILURE)
            return FAILURE;               
    }
    try
    {
        K *key_ = new K(k);
        if(key_==nullptr)
            return ALLOCATION_ERROR;
        D *data_ = new D(d);
        if(data_==nullptr)
            return ALLOCATION_ERROR;
        Ht_item<K,D>* node = new Ht_item<K,D>(key_,data_);
        if(!node)
            return ALLOCATION_ERROR;
        int index = hashFunction(k);
        Ht_item<K,D>** array = getArr();
        Ht_item<K,D>* iter = array[index];
        if(iter==nullptr)
        {
            array[index] = node;
            addCounter();
            return SUCCESS;
        }
        if(iter->AddNode(node)==FAILURE)
        {
            delete node;
            return FAILURE;
        }
        addCounter();
        return SUCCESS;  
    }
    catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
}

template<typename K, typename D>
StatusType hashTable<K,D>::copyArray(Ht_item<K,D>** arrayToCopy , int size){
    int index=0;
    while(index < size)
    {
        Ht_item<K,D>* item = arrayToCopy[index];
        while(item != nullptr)
        {
            this->insert(item->getKeyV(), item->getDataV());
            item=item->getNext();
        }
        index++;
    }
    
    return SUCCESS;
}

template<typename K, typename D>
D* hashTable<K,D>::find(K key){
    int index = hashFunction(key); 
    Ht_item<K,D>** array = getArr();
    Ht_item<K,D>* current = array[index];
    if(current==nullptr) 
        return nullptr;
    while(current)
    {
        if( current->getKeyV() == key ) 
            return current->getData();
        current=current->getNext();
    } 

    return nullptr;
}

template<typename K, typename D>
StatusType hashTable<K,D>::remove(K key){
    int index = hashFunction(key);
    Ht_item<K,D>** array = getArr();
    Ht_item<K,D>* current = array[index];
    if(!current) //doesn't exist
        return FAILURE;
    Ht_item<K,D>* next = current->getNext();
    if(current->getKeyV() == key ) //Incase we need to delete the first object in the list
    {
        delete current;
        decreaseCounter();
        array[index]=next;
        if(reHash() == FAILURE)
            return FAILURE;
        return SUCCESS;
    }
    
    while(next!=nullptr)
    {
        if(next->getKeyV() == key )
        {
            Ht_item<K,D>* temp = next->getNext(); //remmember who next before deleting
            delete next;
            current->setNext(temp);
            decreaseCounter();
            if(reHash() == FAILURE)
                return FAILURE;
            return SUCCESS;                 
        }
        current = next;
        next= current->getNext();
    }

    return FAILURE; // key doesn't exist in the list
    
}

template<typename K, typename D>
StatusType hashTable<K,D>::mergeHTs(hashTable<K,D>* targetHt)
{
    int sizeOfArr = targetHt->getSize();
    Ht_item<K,D>** arr = targetHt->getArr();
    
    for(int i=0 ; i<sizeOfArr ; i++)
    {
        Ht_item<K,D>* iter = arr[i];
        while(iter != nullptr)
        {
            try
            {
                this->insert(iter->getKeyV() , iter->getDataV());
                iter=iter->getNext();
            }
            catch(const std::bad_alloc& e)
            {
                return ALLOCATION_ERROR;
            }
            
        }
    }

    return SUCCESS;
}


#endif
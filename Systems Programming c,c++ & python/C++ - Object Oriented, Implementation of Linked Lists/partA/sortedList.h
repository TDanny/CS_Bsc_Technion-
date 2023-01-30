#ifndef SORTEDLIST_H_
#define SORTEDLIST_H_

#include <stdexcept>
#include <stdbool.h>
#include <assert.h>


namespace mtm 
{
/**
 * Node: class present "block" in sorted list chain
 *      data - present the data that saved in "node"
 *      next - pointer to next node in the list chain
 **/
    template<class T>
    class Node
    {
        public:
        T data;
         Node<T>* next; 
        /**
         * Node: constuctor, null to data, and nullptr for next;
         **/
        Node(); 
        /**
         *  ~Node: distructor of Node, call to distructor of Data(T)  
         **/
        ~Node();
        /**
         * Node: copy-constructor of Node , copy from data by value
         *       next is point to nullptr
         * 
         * @param data: an node you want to copy the details from
         **/
        Node(const T& data);
        /**
         * operator=: copy from "old" to *this node by value
         * 
         * @param to_copy: an node you want to copy the details from
         * @return:
         *          *this - the node you copy to, possible to concatenate
         **/
        Node& operator=(const Node& to_copy);
    };
    //constuctor
    template<class T>
    Node<T>::Node():
    next(nullptr)
    {}

    //distructor
    template<class T>
    Node<T>::~Node()
    {}
    //copy constructor
    template<class T>
    Node<T>::Node (const T& data):
    data(data)
    {
        this->next = nullptr;
    }
    //operator =
    template<class T>
    Node<T>& Node<T>::operator=(const Node<T>& to_copy) 
    {
        if (this == &to_copy) 
        {
            return *this;
        }
        T new_data = to_copy.data;
        delete(this.data);
        this.data = new_data;
        return *this;
    }

//////////////////////////////////////////////////////////////////
/**
 * SortedList: class present generic sorted list by "T", with block of class "Node"
 *      head - present the first node in the list
 **/
    template<class T>
    class SortedList
    {
        private:
        Node<T>* head;

        /**
         * emptyList: get a list and delete all the nodes
         * @param list: list you want to empty
         **/
        void emptyList(SortedList& list);
        /**
         * copyAndAllocate: get 2 list and delete the data from frist one and copy all the data by value from second one  
         * 
         * @param list: exam you want to copy to
         * @param list_to_copy: exam you want to copy from
         **/
        void copyAndAllocate(SortedList& list,const SortedList& list_to_copy);
        
        public:    
        /**
         * const_iterator - class present the iterator of sorted List
         **/
        class const_iterator;
         /**
         *  SortedList: empty constructor that point the head to nullptr
         **/
        SortedList();
        /**
         *  ~SortedList: distructor of list, call to node distructor also  
         **/
        ~SortedList();
        /**
         * SortedList: copy-constructor of list , copy from old by value
         * 
         * @param old: an list you want to copy the details from
         **/
        SortedList(const SortedList&old);
        /**
         * operator=: copy from "list" to *this list by value
         * 
         * @param list: an list you want to copy the details from
         * @return:
         *          *this - the list you copy to
         **/
        SortedList& operator=(const SortedList& list);
        /**
         * operator==: comapre details of 2 list , this and list
         * 
         * @param list: an list you want to compare the details from
         * @return:
         *          true - if both lists have the same node (by value)
         *          false - lists are no identical
         **/
        bool operator==(const SortedList& list)const;
        /**
         * operator!=: comapre details of 2 list , this and list
         * 
         * @param list: an list you want to compare the details from
         * @return:
         *          false - if both lists have the same node (by value)
         *          true - lists are no identical
         **/
        bool operator!=(const SortedList& list)const;
        /**
         * insert: create new node with "data" and insert to the list in sorted way - by operator '<' of "T"
         * 
         * @param data: generic T details you want to create node with and insert to the list
         **/
        void insert(const T& data);
        /**
         * remove: remove the node the iterator point to, the list still sorted
         * 
         * @param iterator: const iterator that point to the index that user want to remove from the list
         **/
        void remove(const const_iterator& iterator); // interantor in there
        /**
         * length: check the length of sorted list
         * 
         * @return - int number that present the length of the list (0 if empty)
         **/
        int length()const;
        /**
         * filter: create new list with specific filter on the nodes
         * @param function - the function the check with data (in nodes) needed to enter to new list
         * @return - SortedList - new sorted list that present *this list with funtion filter
         **/
        template<class Predict>
        SortedList filter(Predict function)const;
        /**
         * filter: create new with the same nodes but every node (data) change by "function"  
         * @param function - the function the change the data (in nodes) that needed to enter to new list
         * @return - SortedList - new sorted list that present *this list with funtion setting
         **/
        template<class Predict>
        SortedList apply(Predict function)const;
        /**
         * begin: point the the start of the list  
         * @return - const_iterator that point to the start of the list
         **/
        const_iterator begin() const;
        /**
         * end: point the the end of the list 
         * @return - const_iterator that point to the end of the list
         **/
        const_iterator end() const;
    };


    template<class T>
    SortedList<T>::SortedList() :
    head(NULL) 
    {}

    template<class T>
    SortedList<T>::~SortedList()
    {
        emptyList(*this);
       // delete this;
    }

    template<class T>
    SortedList<T>::SortedList (const SortedList<T>& list) :
    head(nullptr)
    {
        copyAndAllocate(*this,list);
    }

    template<class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList<T>& list) 
    {
        if (this == &list) 
        {
            return *this;
        }
        emptyList(*this);
        copyAndAllocate(*this,list);
        return *this;
    }

    template<class T>
    bool SortedList<T>::operator==(const SortedList<T>& list) const
    {
        Node<T>* this_ptr = this->head;
        Node<T>* list_ptr = list.head;
        if(this->length() != list.length())
        {
            printf("not same length\n");
            return false;
        }
        for (int i = 0; i < this->length(); i++)
        {
            if(this_ptr->data!=list_ptr->data)
            {
                printf("length\n");
                return false;
            }
            this_ptr = this_ptr->next;
            list_ptr = list_ptr->next;
        }
        return true;
        
    }
    template<class T>
    bool SortedList<T>::operator!=(const SortedList& list) const
    {
        return !(this==list);
    }

    template<class T>
    void SortedList<T>::emptyList(SortedList& list)
    {
        
        Node<T>* to_delete = head;
        while(this->head)
        {
            to_delete = head;
            this->head = to_delete->next;
            delete to_delete;
        }
    }

    template<class T>
    void SortedList<T>::copyAndAllocate(SortedList<T>& list ,const SortedList<T>& list_to_copy)
    {
        list.head = NULL;
        for(Node<T>* i = list_to_copy.head; i != NULL; i = i->next) 
        {
            list.insert(i->data);
        }
    }


    template<class T>
    void SortedList<T>::insert(const T& data) 
    {
        Node<T>* ptr_node_after;
        Node<T>*ptr_node_before;
        Node<T>* new_node = new Node<T>(data);
        int counter=0;
        for(Node<T>* i = this->head; i != nullptr; i = i->next)
        {
            if (data < i->data)
            {
                if(counter==0) //first
                {
                    ptr_node_after = this->head;
                    this->head = new_node;
                    new_node->next = ptr_node_after;
                    return;
                }
                ptr_node_before->next = new_node;
                new_node->next = i;
                return;
            }
            counter++;
            ptr_node_before = i;
        }
        if(counter==0)
        {
            this->head = new_node;
            return;
        }
        ptr_node_before->next = new_node;
    }

    template<class T>
    void SortedList<T>::remove(const const_iterator& iterator) 
    {
        Node<T>* to_delete = this->head;
        Node<T>* ptr_node_after;
        Node<T>* ptr_node_before;
        if(this->head != nullptr)
        {
            ptr_node_after = to_delete->next;
        }
        else
        {
            ptr_node_after = nullptr;
        }
        if(iterator.index == 0)
        {
            delete(this->head);
            this->head = ptr_node_after;
            return;
        }
        for(int i=0;i<iterator.index;i++)
        {
            ptr_node_before = to_delete;
            to_delete=to_delete->next;
            ptr_node_after = ptr_node_after->next;
        }
        delete(to_delete);
        ptr_node_before->next = ptr_node_after;
    }

    template<class T>
    int SortedList<T>::length() const
    {
        int counter=0;
        for(Node<T>* ptr=this->head;ptr != NULL;ptr=ptr->next)
        {
            counter++;
        }
        return counter;
    }
    template<class T>
    typename SortedList<T>::const_iterator SortedList<T>::begin() const
    {
        return const_iterator(this,0);
    }
    template<class T>
    typename SortedList<T>::const_iterator SortedList<T>::end() const
    {
        return const_iterator(this,this->length());
    }

    
    template<class T>
    template<class Predict>
    SortedList<T> SortedList<T>::filter(Predict function)const
    {
        SortedList<T> new_list;
        for(auto iterator = begin();iterator != end();iterator++)
        {
            T data = *iterator;
            if(function(data))
            {
                new_list.insert(*iterator);
            }
        }
        return new_list;
    }

    template<class T>
    template<typename Predict>
    SortedList<T> SortedList<T>::apply(Predict function)const
    {
        SortedList<T> new_list;
        for(auto iterator = begin();iterator != end();iterator++)
        {
            T data = *iterator;
            new_list.insert(function(data));
        }
        return new_list;
    }


    ///////////////////////////////////////Iterator////////////////////////////////////////
/**
 * const_iterator: class that present const iterator that point to specific node in the list
 *      list - present the list that iterator point to+
 *      index - integer to present the index of node the iterator point to
 **/
    template <class T>
    class SortedList<T>::const_iterator
    {
        const SortedList<T>* list;
        int index;
        /**
         * const_iterator: constructor of iterator
         * 
         * @param list: present the list the iterator point to
         * @param index: present the index of the node the iterator point to
         **/
        const_iterator(const SortedList<T>* list, int index); //constructor
        friend class SortedList<T>;
        public:
        /**
         * const_iterator: copy-constructor of iterator , point like 'iterator'
         *       
         * 
         * @param iterator: an iterator you want to copy the details from
         **/
        const_iterator(const const_iterator& iterator); // copy constructor
        /**
         * operator=: copy from "iterator" to *this 
         * 
         * @param iterator: an iterator you want to copy 
         * @return:
         *          *this - the iterator you copy to
         **/
        const_iterator& operator=(const const_iterator&iterator) = default;//Placement
        /**
         * operator==: comapre iterators, most point to the same list
         * 
         * @param iterator: an iterator you want to compare the details from
         * @return:
         *          true - if both iterators look to the same index in the same like
         *          false - node are not look to the same index
         **/
        bool operator==(const const_iterator& iterator) const;
        /**
         * operator!=: comapre iterators, most point to the same list
         * 
         * @param iterator: an iterator you want to compare the details from
         * @return:
         *          false - if both iterators look to the same index in the same like
         *          true - node are not look to the same index
         **/
        bool operator!=(const const_iterator& ) const;
        /**
         * operator*: get the details of the node the iterator point to
         * 
         * @return:
         *          T - value of class variable in the node on the list the iterator point to
         **/
        const T& operator*() const; //get value of iterator
        /**
         * operator++: promote to iterator to next value 
         * 
         * @return:
         *          const_iterator - pointer of this, for case user want to concatenate
         **/
        const_iterator& operator++(); // promote the iterator to next node
        /**
         * operator++(int): promote to iterator to next value 
         * 
         * @return:
         *          const_iterator - return the last place on iterator without promote the index
         **/
        const_iterator operator++(int); // promote the iterator to next node
    };

    template<class T>
    SortedList<T>::const_iterator::const_iterator(const const_iterator& iterator):
    list(iterator.list), index(iterator.index)
    {}

    template<class T>
    SortedList<T>::const_iterator::const_iterator(const SortedList<T>* list, int index):
    list(list), index(index)
    {}

    template<class T>
    const T& SortedList<T>::const_iterator::operator*() const 
    {
        assert((index>=0 && index<= list->length()) || (index==0 && list->length()==0));
        Node<T>* ptr_node = list->head;
        for(int i=0;i<index;i++)
        {
            ptr_node = ptr_node->next;
        }
        return ptr_node->data;
    }

    template<class T>
    typename SortedList<T>::const_iterator& SortedList<T>::const_iterator::operator++()
    {
        if(this == nullptr)
        {
            throw std::out_of_range("out of range");
        }
        ++index;
        return *this;
    }

    template<class T>
    typename SortedList<T>::const_iterator SortedList<T>::const_iterator::operator++(int)
    {
        if(this == nullptr)
        {
            throw std::out_of_range("out of range");
        }
        const_iterator result = *this;
        ++*this;
        return result;
    }

    template<class T>
    bool SortedList<T>::const_iterator::operator==(const const_iterator& i) const
    {
        assert(list == i.list);
         //Compare between 2 difeerent sets is a bug
        return index == i.index;
    }

    template<class T>
    bool SortedList<T>::const_iterator::operator!=(const const_iterator& i) const
    {
        return !(*this == i);
    }

}
    
#endif

#ifndef UNION_FIND_H
#define UNION_FIND_H_

#include <iostream>
#include "unode.h"

using namespace std;

class UnionFind
{
    private:
        Unode **arr;
        int arrSize;
        void destroyArr(int cur_index)
        {
            if(cur_index>0 && arr)
            {
                int i=0;
                for(i=0;i<cur_index;i++)
                {
                    if(arr[i])
                        delete arr[i];
                    arr[i]=nullptr;
                }
            }
            delete[] arr;
            arr=nullptr;
        }

        void shrink(Unode* root, int id)
        {
            Unode* curr=arr[id-1];
            int p_size=0, cur_size=0;
            while(!curr->is_root())
            {
                Unode* parent = curr->getParent();
                Unode* tmp = parent;
                double new_value= curr->getValue();
                while(!tmp->is_root())
                {
                    new_value+=tmp->getValue();
                    tmp=tmp->getParent();
                }
                curr->setParent(root);
                curr->setValue(new_value);
                cur_size=curr->getSubNodes();
                curr->setSubNodes(cur_size-p_size);
                p_size=cur_size;
                curr=parent;
            }
        }

    public:
        UnionFind(int size) {
            arrSize=size;
            arr=new Unode*[size];
            for(int i=0;i<size;i++)
            {
                try {
                    arr[i]=new Unode(i+1);
                }
                catch(std::bad_alloc& e)
                {
                    destroyArr(i);
                    throw;
                }
            }
        }
        ~UnionFind()
        {
            destroyArr(this->arrSize);
        }

        Unode** getArr() { return arr; }
        int getSize() { return arrSize; }

        void setArr(Unode **arr_) { arr=arr_; }
        void setSize(int size_) { arrSize=size_; }

        Unode* find_assist(int id)
        {
            Unode* node = arr[id-1];
            while(!node->is_root())
                node=node->getParent();
            shrink(node,id);
            return node;
        }
        Company* find(int id)
        {
            return find_assist(id)->getData();
        }
        int Union(int f_id, int s_id , double factor)
        {
            Unode* first = find_assist(f_id); 
            Unode* second = find_assist(s_id);
            if(first==second)
                return 3;
            else if(first->is_bigger(second))
            {
                
                first->mergeTrees(second);
                first->setValue(first->getValue()+(factor * second->getUnionVal()));
                second->setValue(second->getValue() - (first->getValue()));
                first->setUnionVal(first->getUnionVal()+second->getUnionVal()*factor);
                return 1;

            }
            else
            {
                
                second->mergeTrees(first);
                first->setValue(first->getValue()+second->getUnionVal()*factor-second->getValue());
                second->setUnionVal(first->getUnionVal()+second->getUnionVal()*factor);
                return 2;
            }
        }

};

#endif
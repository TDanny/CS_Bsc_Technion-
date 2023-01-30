#ifndef UNODE_H_
#define UNODE_H_

#include <iostream>

class Unode {
    private:
        int id;
        int sub_nodes;
        Company* data;
        double value;
        Unode* parent;
        bool root_flag;
        double union_value;
    public:
        Unode(int id): id(id), sub_nodes(1),  data(nullptr), value(id), parent(nullptr), root_flag(true), union_value(id) {
            try{
                data = new Company(id);
            }
            catch(std::bad_alloc& e)
            {
                throw;
            }
        }
        ~Unode() {
            if(data!=nullptr)
                delete data;
            data=nullptr;
        } 

        int getID() { return id; }
        int getSubNodes() { return sub_nodes; }
        Company* getData() { return data; }
        Company getDataV() { return *data; }
        double getValue() { return value; }
        Unode* getParent() { return parent; }
        bool is_root() { return root_flag; }
        double getUnionVal() { return union_value; }

        void setID(int id_) { id=id_; }
        void setSubNodes(int n) { sub_nodes=n; }
        void increaseSubNodes(int inc) { sub_nodes+=inc; }
        void decreaseSubNodes(int dec) { sub_nodes-=dec; }
        void setData(Company* data_) { data=data_; } 
        void setDataV(Company data_) { *data=data_; }
        void setValue(double value_) {value = value_;}
        void increaseValue(double value_) { value+=value_; }
        void setParent(Unode* parent_) { parent=parent_; }
        void setRootFlag(bool flag) { root_flag=flag; }
        void setUnionVal(double val) { union_value=val; }

        bool is_bigger(Unode* s_root) { return (this->sub_nodes >= s_root->getSubNodes()); }
        void mergeTrees(Unode* s_root)
        {
            s_root->setRootFlag(false);
            this->increaseSubNodes(s_root->getSubNodes());
            s_root->setParent(this);
            this->root_flag=true;
        }

};

#endif
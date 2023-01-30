
#ifndef RANKED_TREE_H_
#define RANKED_TREE_H_

#include <iostream>
#include "library2.h"

using namespace std;

#define LEFT 1
#define RIGHT 2
#define ROOT 3

template<typename KeyType, typename DataType>
class Tree;
 
template<typename KeyType, typename DataType>
class Tnode {
    private:
        KeyType *key;
        DataType *data;
        Tnode *left;
        Tnode *right;
        Tnode *parent;
        int height;
        long long tree_nodes;
        long long sum_grades;
        long long t_grade;
    public:
        Tnode(KeyType* key, DataType* data, long long sum_grades): key(key),data(data), left(nullptr), 
                                            right(nullptr), parent(nullptr), height(0), tree_nodes(1), sum_grades(sum_grades), t_grade(sum_grades) {}
 
        ~Tnode() {
            if(key!=nullptr)
                delete key;
            key=nullptr;
            if(data!=nullptr)
                delete data;
            data=nullptr;
        } 
 
        KeyType* getKey() { return key; }
        DataType* getData() { return data; }
        KeyType getKeyV() { return *key; }
        DataType getDataV() { return *data; }
        Tnode* getLeft() { return left; }
        Tnode* getRight() { return right; }
        Tnode* getParent() { return parent; }
        int getHeight() { return height; }
        long long getTreeNodes() { return tree_nodes; }
        long long getSumGrades() { return sum_grades; }
        long long getTGrade() { return t_grade; }

        void setKey(KeyType* key_) { key=key_; } 
        void setData(DataType* data_) { data=data_; } 
        void setDataV(DataType data_) { *data=data_; }
        void setLeft(Tnode* left_) { 
            left=left_;
        }
        void setRight(Tnode* right_) { 
            right=right_; 
        }
        void setParent(Tnode* parent_) { 
            parent=parent_;
        }
        void setHeight(int height_) { height=height_; }
        void setTreeNodes(long long tree_nodes_) { tree_nodes=tree_nodes_; }
        void setSumGrades(long long sum_grades_) { sum_grades=sum_grades_; }
        void setTGrade(long long grade) { t_grade=grade; }
        void increaseHeight() { height++; }
        void decreaseHeight() { height--; }
        void increaseTreeNodes() { tree_nodes++; }
        void decreaseTreeNodes() { tree_nodes--; }
        void increaseSumGrades(long long cur) { sum_grades+=cur; }
        void specialSetGrades(Tnode<KeyType,DataType>* node, long long grade)
        {
            while(node)
            {
                node->increaseSumGrades(grade);
                node=node->getParent();
            }
        }
        void decreaseSumGrades(long long cur) { sum_grades-=cur; }
        void calcGrade() {
            int grade=this->getTGrade();
            if(this->getLeft())
                grade+=this->getLeft()->getSumGrades();
            if(this->getRight())
                grade+=this->getRight()->getSumGrades();
            this->sum_grades=grade;
        }

};
 
 
template<typename KeyType, typename DataType>
class Tree {
    private:
        Tnode<KeyType, DataType>* root;
        long long counter;
        
        void insertN(Tnode<KeyType,DataType>* tnode, Tnode<KeyType, DataType>* new_node);
        void deleteN(Tnode<KeyType,DataType>* tnode, Tnode<KeyType,DataType>* parent ,KeyType key, int father, long long grade);
        Tnode<KeyType, DataType>* search(Tnode<KeyType,DataType>* tnode, KeyType key);
        void inOrder(Tnode<KeyType,DataType>* node, Tnode<KeyType,DataType> ***nodes, int* i, int len);
        void specialInOrder(Tnode<KeyType,DataType>* node, long long* num, long long* sum, int len);

    public:
        Tree() : root(nullptr), counter(0) {}
        ~Tree();

        Tnode<KeyType,DataType>* getRoot() { return root; }
        long long getCounter() { return counter; }
        long long getRank(Tnode<KeyType,DataType>* node, KeyType key);
        long long getDegree(Tnode<KeyType, DataType>* node);

        void setRoot(Tnode<KeyType,DataType>* _root) { root = _root; }
        void setCounter(long long _counter) { counter= _counter; }
        
        int max(int a, int b) { return a>b ? a:b; }

        int getBFactor(Tnode<KeyType,DataType>* node) {
            if(!node)
                return 0;
            int left = adjustHeight(node->getLeft()), right = adjustHeight(node->getRight());
            if(node->getLeft())
                left++;
            if(node->getRight())
                right++;
            return left-right;     
        }

        void deleteAllNodes(Tnode<KeyType,DataType>* node);
        int adjustHeight(Tnode<KeyType,DataType>* node);
        int treeHeight(Tnode<KeyType,DataType>* node);
        Tnode<KeyType, DataType>* searchAssist(KeyType key);
        void rightRotate(Tnode<KeyType,DataType>* tnode);
        void leftRotate(Tnode<KeyType,DataType>* tnode);
        void balance(Tnode<KeyType,DataType>* tnode);
        StatusType insert(KeyType key, DataType data, long long grade);
        StatusType remove(KeyType key, long long grade);

        StatusType inOrderAssist(Tnode<KeyType,DataType> ***nodes, int len);
        StatusType specialInOrderAssist(long long* sum, int len);

        Tnode<KeyType,DataType>* arrayToTree(Tnode<KeyType,DataType> **nodes, 
                int start, int end, StatusType *result);
        Tree<KeyType, DataType>* mergeTree(Tree<KeyType,DataType>* t, StatusType* result);

};

template<typename KeyType, typename DataType>
Tree<KeyType, DataType>::~Tree() {
    if(root!=nullptr)
        deleteAllNodes(root);
}
 
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::deleteAllNodes(Tnode<KeyType,DataType>* node) {
    if(node!=nullptr) {
        if(node->getLeft())
        {
            deleteAllNodes(node->getLeft());

        }
        if(node->getRight())
        {
            deleteAllNodes(node->getRight());
        }
        delete node;
        node=nullptr;
    }
}

// RETURNS THE AMOUNT OF SUB NODES + 1 in RECURSION
template<typename KeyType, typename DataType>
long long Tree<KeyType, DataType>::getRank(Tnode<KeyType,DataType>* node, KeyType key)
{
    if(node==nullptr)
        return 0;
    if(node->getKeyV()==key)
    {
        if(node->getLeft())
            return 1+(node->getLeft())->getTreeNodes();
        return 1;
    }
    if(node->getKeyV() > key)
    {
        if(node->getLeft())
            return 1+(node->getLeft())->getTreeNodes()+getRank(node->getRight(),key);
        return 1+getRank(node->getRight(),key);
    }
    return getRank(node->getLeft(), key);
}

// returns THE NUMBER OF SUB NODES + 1 in O(1)
template<typename KeyType, typename DataType>
long long Tree<KeyType, DataType>::getDegree(Tnode<KeyType, DataType>* node)
{
    if(!node)
        return 0;
    int degree=1;
    if(node->getLeft())
        degree+=(node->getLeft())->getTreeNodes();
    if(node->getRight())
        degree+=(node->getRight())->getTreeNodes();
    return degree;
}

// O(1)
template<typename KeyType, typename DataType>
int Tree<KeyType, DataType>::adjustHeight(Tnode<KeyType,DataType>* node)
{
    int h=0;
    if(!node)
        return 0;
    int left=0, right=0;
    if(node->getLeft())
        left=1+(node->getLeft())->getHeight();
    if(node->getRight())
        right=1+(node->getRight())->getHeight();
    h=max(left,right);
    return h;
    
}
// O(N)
template<typename KeyType, typename DataType>
int Tree<KeyType, DataType>::treeHeight(Tnode<KeyType,DataType>* node)
{
    int h=0;
    if(!node) {
        return 0;
    }
    int left = treeHeight(node->getLeft());
    int right = treeHeight(node->getRight());
    if(node->getLeft())
        left++;
    if(node->getRight())
        right++;
    h=max(left,right);
    node->setHeight(h);
    return h;
}

template<typename KeyType, typename DataType>
Tnode<KeyType, DataType>* Tree<KeyType, DataType>::search(Tnode<KeyType,DataType>* tnode, KeyType key)
{
    if(!tnode)
        return tnode;
    if(tnode->getKeyV() == key)
        return tnode;
    if(tnode->getKeyV()<key)
        return search(tnode->getRight(),key);
    return search(tnode->getLeft(),key);
}

template<typename KeyType, typename DataType>
Tnode<KeyType, DataType>* Tree<KeyType, DataType>::searchAssist(KeyType key)
{
    return search(root, key);
}

 
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::rightRotate(Tnode<KeyType,DataType>* tnode)
{
    // B = TNODE, A = LEFTSON, AR = RIGHT TREE
    bool left=true;
    Tnode<KeyType,DataType> *parent=tnode->getParent();
    if(parent !=nullptr && parent->getLeft() != tnode)
        left=false;
    Tnode<KeyType,DataType> *leftson = tnode->getLeft(); 
    Tnode<KeyType,DataType> *righttree = leftson -> getRight(); 
    tnode->setLeft(righttree);
    leftson->setRight(tnode);
    tnode->setHeight(adjustHeight(tnode));
    leftson->setHeight(adjustHeight(leftson));
    tnode->setTreeNodes(getDegree(tnode));
    leftson->setTreeNodes(getDegree(leftson));
    tnode->setSumGrades(tnode->getTGrade());
    if(tnode->getRight())
        tnode->increaseSumGrades(tnode->getRight()->getSumGrades());
    if(righttree)
    {
        righttree->setParent(tnode);
        tnode->increaseSumGrades(righttree->getSumGrades());
    }
    leftson->setSumGrades(leftson->getTGrade()+tnode->getSumGrades());
    if(leftson->getLeft())
        leftson->increaseSumGrades(leftson->getLeft()->getSumGrades());
    if(parent!=nullptr)
    {
        int tmpG=0;
        if(left) {
            parent->setLeft(leftson);
            if(parent->getRight())
                tmpG=parent->getRight()->getSumGrades();
        }
        else {
            parent->setRight(leftson);
            if(parent->getLeft())
                tmpG=parent->getLeft()->getSumGrades();
        }
        leftson->setParent(parent);
        parent->setHeight(adjustHeight(parent));
        parent->setTreeNodes(getDegree(parent));
        parent->setSumGrades(parent->getTGrade()+leftson->getSumGrades()+tmpG);
    }
    else
    {
        root = leftson;
        leftson->setParent(nullptr);
    }
    tnode->setParent(leftson);
}
 
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::leftRotate(Tnode<KeyType,DataType>* tnode)
{
    bool left=true;
    Tnode<KeyType,DataType> *parent=tnode->getParent();
    if(parent !=nullptr && parent->getLeft() != tnode)
        left=false;
    Tnode<KeyType,DataType> *rightson = tnode->getRight(); 
    Tnode<KeyType,DataType> *lefttree = rightson -> getLeft(); 
    tnode->setRight(lefttree);
    rightson->setLeft(tnode);
    tnode->setHeight(adjustHeight(tnode));
    rightson->setHeight(adjustHeight(rightson));
    tnode->setTreeNodes(getDegree(tnode));
    rightson->setTreeNodes(getDegree(rightson));
    tnode->setSumGrades(tnode->getTGrade());
    if(tnode->getLeft())
        tnode->increaseSumGrades(tnode->getLeft()->getSumGrades());
    if(lefttree)
    {
        lefttree->setParent(tnode);
        tnode->increaseSumGrades(lefttree->getSumGrades());
    }
    rightson->setSumGrades(rightson->getTGrade()+tnode->getSumGrades());
    if(rightson->getRight())
        rightson->increaseSumGrades(rightson->getRight()->getSumGrades());
    if(parent!=nullptr)
    {
        int tmpG=0;
        if(left) {
            parent->setLeft(rightson);
            if(parent->getRight())
                tmpG=parent->getRight()->getSumGrades();
        }
        else {
            parent->setRight(rightson);
            if(parent->getLeft()!=nullptr)
                tmpG=parent->getLeft()->getSumGrades();

        }
        rightson->setParent(parent);
        parent->setHeight(adjustHeight(parent));
        parent->setTreeNodes(getDegree(parent));
        parent->setSumGrades(parent->getTGrade()+rightson->getSumGrades()+tmpG);
    }
    else
    {
        root = rightson; 
        rightson->setParent(nullptr);
    }
    tnode->setParent(rightson);
}

template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::balance(Tnode<KeyType,DataType>* tnode)
{
    int bFactor = getBFactor(tnode);
    if(bFactor > 1)
    {
        if(getBFactor(tnode->getLeft()) < 0) {
            leftRotate(tnode->getLeft());
        }
        rightRotate(tnode);
    }
    else if(bFactor < -1) 
    {
        if(getBFactor(tnode->getRight()) > 0) {
            rightRotate(tnode->getRight());
        }
        leftRotate(tnode);
    }
}
 
template<typename KeyType, typename DataType> // need to create new node and send it
void Tree<KeyType, DataType>::insertN(Tnode<KeyType,DataType>* tnode, Tnode<KeyType, DataType>* new_node)
{
    if(tnode->getKeyV() < new_node->getKeyV()) {
        if(!tnode->getRight()) {
            tnode->setRight(new_node);
            new_node->setParent(tnode);
            tnode->increaseTreeNodes();
            tnode->increaseSumGrades(new_node->getTGrade());
        }
        else {
            tnode->increaseSumGrades(new_node->getTGrade());
            insertN(tnode->getRight(), new_node);
        }
 
    }
    else {
        if(!tnode->getLeft()) {
            tnode->setLeft(new_node);
            new_node->setParent(tnode);
            tnode->increaseTreeNodes();
            tnode->increaseSumGrades(new_node->getTGrade());
        }
        else {
            tnode->increaseSumGrades(new_node->getTGrade());
            insertN(tnode->getLeft(), new_node);
        }
 
    }
    tnode->setHeight(adjustHeight(tnode));
    tnode->setTreeNodes(getDegree(tnode));
    balance(tnode);
}
 
template<typename KeyType, typename DataType> 
StatusType Tree<KeyType, DataType>::insert(KeyType key, DataType data, long long grade)
{
    if(root!=nullptr) {
        if(search(root, key)) // if already exists
        return FAILURE;
    }
    KeyType *key_ = new KeyType(key);
    if(key_==nullptr)
        return ALLOCATION_ERROR;
    DataType *data_ = new DataType(data);
    if(data_==nullptr)
    {
        delete key_;
        return ALLOCATION_ERROR;
    }
    Tnode<KeyType, DataType> *new_node= new Tnode<KeyType, DataType>(key_,data_, grade);
    if(!new_node)
    {
        delete key_;
        delete data_;
        return ALLOCATION_ERROR;
    }
    setCounter(getCounter()+1);
    if(!root) {
        root=new_node;
    }
    else {
        insertN(root, new_node);
    }
    return SUCCESS;
}
 
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::deleteN(Tnode<KeyType,DataType>* tnode, Tnode<KeyType,DataType>* parent ,KeyType key,int father, long long grade)
{
    if(tnode==nullptr)
        return;
    else if(tnode->getKeyV() < key) // in the right tree
    {
        tnode->decreaseSumGrades(grade);
        deleteN(tnode->getRight(),tnode, key,RIGHT,grade);
    }
    else if(tnode->getKeyV() > key) // in the left tree
    {
        tnode->decreaseSumGrades(grade);
        deleteN(tnode->getLeft(),tnode, key,LEFT,grade);
    }
    else // we found the node
    {
        if(tnode->getLeft()==nullptr && tnode->getRight() == nullptr) // leaf
        {
            if(parent!=nullptr)
            {
                if(father==LEFT) {
 
                    parent->setLeft(nullptr);
                }
                else {
                    parent->setRight(nullptr);
                }
                delete tnode;
                tnode=nullptr;
                parent->setHeight(adjustHeight(parent));
                parent->setTreeNodes(getDegree(parent));
                parent->calcGrade();
                balance(parent);
            }
            else // root
            {
                delete tnode;
                root=nullptr;
                tnode=nullptr;
            }
            return;
        }
        else if(tnode->getLeft() == nullptr && tnode->getRight() != nullptr) // has only RIGHT son
        {
            Tnode<KeyType,DataType> *tmp = tnode->getRight();
            tnode->setParent(nullptr);
            tnode->setRight(nullptr);
            if(parent==nullptr) // it's the root
            {
                root=tmp;
                tmp->setParent(nullptr);
            }
            else
            {
                tmp->setParent(parent);
                if(father==RIGHT)
                    parent->setRight(tmp);
                else
                    parent->setLeft(tmp);
                parent->setHeight(adjustHeight(parent));
                parent->setTreeNodes(getDegree(parent));
                parent->calcGrade();
                balance(parent);
            }
            delete tnode;
            tnode=nullptr;
            return;
        }
        else if(tnode->getLeft()!=nullptr && tnode->getRight()==nullptr) // has only LEFT son
        {
            Tnode<KeyType,DataType> *tmp = tnode->getLeft();
            tnode->setParent(nullptr);
            tnode->setLeft(nullptr);
            if(parent==nullptr) // it's the root
            {
                root=tmp;
                tmp->setParent(nullptr);
            }
            else
            {
                tmp->setParent(parent);
                if(father==RIGHT)
                    parent->setRight(tmp);
                else
                    parent->setLeft(tmp);
                parent->setHeight(adjustHeight(parent));
                parent->setTreeNodes(getDegree(parent));
                parent->calcGrade();
                balance(parent);
            }
            delete tnode;
            tnode=nullptr;
            return;
        }
        else // has two sons
        {
            Tnode<KeyType,DataType> *tmpParent=tnode;
            Tnode<KeyType,DataType> *tmp = tnode->getRight();
            int tnodeGrade=tnode->getTGrade(), tnodeNodes=tnode->getTreeNodes();
            // we gonna swap, then we get tNodeGrade 
            while(tmp->getLeft()!=nullptr)
            {
                tmpParent=tmp;
                tmp=tmp->getLeft();
            }
            int new_grade=tmp->getTGrade();
            tmp=tnode->getRight();
            while(tmp->getLeft()!=nullptr)
            {
                tmp->setSumGrades(tmp->getSumGrades()-new_grade+tnodeGrade);
                tmp=tmp->getLeft();
            }
            if(tmpParent==tnode) {
                tmpParent->setRight(tnode);
            }
            else {
                tmpParent->setLeft(tnode);
            }
            tmp->setSumGrades(tmp->getSumGrades()-new_grade+tnodeGrade);
            tnode->setParent(tmpParent);
            // the node has only RIGHT SON at most
            Tnode<KeyType,DataType>* tmpRight = tmp->getRight();
            int tmpH = tmp->getHeight();
            if(parent==nullptr) // it's the root
            {
                root=tmp;
                tmp->setParent(nullptr);
            }
            else // has parent
            {
                if(father == RIGHT) // his right son
                {
                    parent->setRight(tmp);
                
                }
                else // his left son
                {
                    parent->setLeft(tmp);
                }
                tmp->setParent(parent);

            }
            tmp->setLeft(tnode->getLeft());
            tnode->getLeft()->setParent(tmp);
            tmp->setRight(tnode->getRight());
            tnode->getRight()->setParent(tmp);
            tmp->setHeight(tnode->getHeight());
            tnode->setRight(tmpRight);
            tnode->setTreeNodes(1);
            tnode->setSumGrades(tnodeGrade);
            if(tmpRight)
            {
                tmpRight->setParent(tnode);
                tnode->increaseTreeNodes();
                tnode->increaseSumGrades(tmpRight->getTGrade());
            }
            tnode->setLeft(nullptr);
            tnode->setHeight(tmpH);
            tmp->setTreeNodes(tnodeNodes);
            tmp->setSumGrades(new_grade+tmp->getRight()->getSumGrades());
            if(tmp->getLeft())
                tmp->increaseSumGrades(tmp->getLeft()->getSumGrades());
            deleteN(tmp->getRight(),tmp,key,RIGHT,grade);
            tmp->setHeight(adjustHeight(tmp));
            tmp->setTreeNodes(getDegree(tmp));
            tmp->setSumGrades(new_grade);
            if(tmp->getLeft())
                tmp->increaseSumGrades(tmp->getLeft()->getSumGrades());
            if(tmp->getRight())
                tmp->increaseSumGrades(tmp->getRight()->getSumGrades());
            balance(tmp);
            return;
        }
    }
    tnode->setHeight(adjustHeight(tnode));
    tnode->setTreeNodes(getDegree(tnode));
    balance(tnode);
}

template<typename KeyType, typename DataType> 
StatusType Tree<KeyType, DataType>::remove(KeyType key, long long grade)
{
    if(!search(root, key))
        return FAILURE;
    deleteN(root, nullptr, key, ROOT,grade);
    setCounter(getCounter()-1);
    return SUCCESS;
}
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::inOrder(Tnode<KeyType,DataType>* node, Tnode<KeyType,DataType> ***nodes, int* i, int len) {
    if(node && (*i)<len) {
        inOrder(node->getLeft(),nodes,i,len);
        if(*i<len)
        {
            **nodes=node;
            (*nodes)++;
            (*i)++;
        }
        inOrder(node->getRight(),nodes,i,len);
    }
}
template<typename KeyType, typename DataType>
StatusType Tree<KeyType, DataType>::inOrderAssist(Tnode<KeyType,DataType> ***nodes, int len) {
    Tnode<KeyType,DataType> **tmpN = *nodes;
    int *i=new int(0);
    if(i==nullptr)
        return ALLOCATION_ERROR;
    inOrder(root, nodes, i, len);
    *nodes=tmpN;
    delete i;
    return SUCCESS;
}

// LOG N
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::specialInOrder(Tnode<KeyType,DataType>* node, long long* num, long long* sum, int len)
{
    if(node==nullptr || *num==len)
        return;
    if(node->getRight())
    {
        if(*num+node->getRight()->getTreeNodes()>=len)
        {
            specialInOrder(node->getRight(),num,sum,len);
        }
        else 
        {
            *sum+=node->getRight()->getSumGrades()+node->getTGrade();
            *num+=node->getRight()->getTreeNodes()+1;
            if(*num==len)
                return;
            specialInOrder(node->getLeft(),num,sum,len);
        }
    }
    else{
        *sum+=node->getTGrade();
        (*num)++;
        if((*num)==len)
            return;
        specialInOrder(node->getLeft(),num,sum,len);
    }
    return;
}

template<typename KeyType, typename DataType>
StatusType Tree<KeyType, DataType>::specialInOrderAssist(long long* sum, int len)
{
    long long *num=new long long(0);
    if(num==nullptr)
        return ALLOCATION_ERROR;
    *sum=0;
    specialInOrder(root, num, sum,len);
    delete num;
    return SUCCESS;
}
template<typename KeyType, typename DataType>
Tnode<KeyType,DataType>* Tree<KeyType,DataType>::arrayToTree(Tnode<KeyType,DataType> **nodes, 
                int start, int end, StatusType *result)
{
    if(start>end) {
        return nullptr;
    }
    int mid = (start+end)/2;
    KeyType *key = new KeyType(nodes[mid]->getKeyV());
    if(key==nullptr)
    {
        *result=ALLOCATION_ERROR;
        return nullptr;
    }
    DataType *data= new DataType(nodes[mid]->getDataV());
    if(data==nullptr)
    {
        delete data;
        *result=ALLOCATION_ERROR;
        return nullptr;
    }
    Tnode<KeyType,DataType>* node = new Tnode<KeyType,DataType>(key,data,(nodes[mid]->getData())->getGrade());
    if(!node)
    {
        delete key;
        delete data;
        *result = ALLOCATION_ERROR;
        return nullptr;        
    }
    node->setLeft(arrayToTree(nodes,start,mid-1,result));
    if(*result != SUCCESS)
    {
        return nullptr;
    }
    if(node->getLeft())
        (node->getLeft())->setParent(node);
    node->setRight(arrayToTree(nodes,mid+1,end,result));
    if(*result != SUCCESS)
    {
        return nullptr;
    }
    if(node->getRight())
        (node->getRight())->setParent(node);
    node->setHeight(adjustHeight(node));
    node->setTreeNodes(getDegree(node->getLeft())+getDegree(node->getRight())+1);
    node->setSumGrades(node->getTGrade());
    if(node->getLeft())
        node->increaseSumGrades(node->getLeft()->getSumGrades());
    if(node->getRight())
        node->increaseSumGrades(node->getRight()->getSumGrades());
    balance(node);    
    return node;
}

template<typename KeyType, typename DataType>
Tree<KeyType, DataType>* Tree<KeyType,DataType>::mergeTree(Tree<KeyType,DataType>* t, StatusType* result)
{
    int treeLen1 = getCounter(), treeLen2 = t->getCounter();
    Tnode<KeyType,DataType> **tree1 = new Tnode<KeyType,DataType>*[treeLen1];
    if(tree1==nullptr)
    {
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    Tnode<KeyType,DataType> **tree2 = new Tnode<KeyType,DataType>*[treeLen2];
    if(tree2==nullptr)
    {
        delete[] tree1;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    Tnode<KeyType,DataType> **mergedTree = new Tnode<KeyType,DataType>*[treeLen1+treeLen2];
    if(mergedTree==nullptr)
    {
        delete[] tree1;
        delete[] tree2;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    Tnode<KeyType,DataType> ***ptr1 = new Tnode<KeyType,DataType>**();
    if(ptr1==nullptr)
    {
        delete[] tree1;
        delete[] tree2;
        delete[] mergedTree;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    Tnode<KeyType,DataType> ***ptr2 = new Tnode<KeyType,DataType>**();
    if(ptr2==nullptr)
    {
        delete[] tree1;
        delete[] tree2;
        delete[] mergedTree;
        delete ptr1;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    *ptr1=tree1;
    *ptr2=tree2;
    *result = inOrderAssist(ptr1, treeLen1);
    if(*result!=SUCCESS)
    {
        delete[] tree1;
        delete[] tree2;
        delete[] mergedTree;
        delete ptr1;
        delete ptr2;
        return nullptr;
    }
    *result = t->inOrderAssist(ptr2, treeLen2);
    if(*result!=SUCCESS)
    {
        delete[] tree1;
        delete[] tree2;
        delete[] mergedTree;
        delete ptr1;
        delete ptr2;
        return nullptr;
    }
    int t1=0,t2=0;
    int mt=0;
    while(t1<treeLen1 && t2<treeLen2)
    {
        if((tree1[t1])->getKeyV()<(tree2[t2])->getKeyV()) {
            mergedTree[mt]=tree1[t1];
            t1++;
        }
        else {
            mergedTree[mt]=tree2[t2];
            t2++;
        }
        mt++;
    }
    while(t1<treeLen1)
    {
        mergedTree[mt]=tree1[t1];
        t1++;
        mt++;
    }
    while(t2<treeLen2)
    {
        mergedTree[mt]=tree2[t2];
        t2++;
        mt++;  
    }
    Tnode<KeyType,DataType>* new_root;
    new_root = arrayToTree(mergedTree,0, treeLen1+treeLen2-1, result);
    delete[] tree1;
    delete[] tree2;
    delete[] mergedTree;
    delete ptr1;
    delete ptr2;
    if(*result == SUCCESS)
    {
        setCounter(treeLen1+treeLen2);
        Tree<KeyType,DataType>* new_t = new Tree<KeyType,DataType>();
        if(new_t==nullptr) {
            *result=ALLOCATION_ERROR;
            return nullptr;
        }
        new_t->setRoot(new_root);
        new_t->setCounter(getCounter());
        new_t->treeHeight(new_t->getRoot());
        return new_t;     
    }
    return nullptr;
}

#endif
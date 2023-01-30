
#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <iostream>
#include "library1.h"

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
    public:
        Tnode(KeyType* key, DataType* data): key(key),data(data), left(nullptr), right(nullptr), parent(nullptr), height(0) {}
 
        ~Tnode() {
            if(key!=nullptr)
                delete key;
            if(data!=nullptr)
                delete data;
        } 
 
        KeyType* getKey() { return key; }
        DataType* getData() { return data; }
        KeyType getKeyV() { return *key; }
        DataType getDataV() { return *data; }
        Tnode* getLeft() { return left; }
        Tnode* getRight() { return right; }
        Tnode* getParent() { return parent; }
        int getHeight() { return height; }

        void setKey(KeyType* key_) { key=key_; } 
        void setData(DataType* data_) { data=data_; } 
        void setDataV(DataType data_) { *data=data_; }
        void setLeft(Tnode* left_) { left=left_; }
        void setRight(Tnode* right_) { right=right_; }
        void setParent(Tnode* parent_) { parent=parent_; }
        void setHeight(int height_) { height=height_; }

        void increaseHeight() { height++; }
        void decreaseHeight() { height--; }
};
 
 
template<typename KeyType, typename DataType>
class Tree {
    private:
        Tnode<KeyType, DataType>* root;
        int counter;
        KeyType *max_node;
        
        void insertN(Tnode<KeyType,DataType>* tnode, Tnode<KeyType, DataType>* new_node);
        void deleteN(Tnode<KeyType,DataType>* tnode, Tnode<KeyType,DataType>* parent ,KeyType key, int father);
        Tnode<KeyType, DataType>* search(Tnode<KeyType,DataType>* tnode, KeyType key);
        void inOrder(Tnode<KeyType,DataType>* node, KeyType ***k, DataType ***d, int* i, int len);
        int countCondNodes(Tnode<KeyType, DataType>* tnode, KeyType minKey, KeyType maxKey);
        void specialCondNodes(Tnode<KeyType, DataType>* tnode, DataType*** keys, 
                                                            KeyType minKey, KeyType maxKey,int *i, int len);
    public:
        Tree() : root(nullptr), counter(0) {
            max_node=new KeyType();
        }
        ~Tree();

        Tnode<KeyType,DataType>* getRoot() { return root; }
        int getCounter() { return counter; }
        KeyType* getMaxNode() { return max_node; }
        void setRoot(Tnode<KeyType,DataType>* _root) { root = _root; }
        void setCounter(int _counter) { counter= _counter; }
        int max(int a, int b) {
            return a>b ? a:b;
        }
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
        void findMaxNode(Tnode<KeyType,DataType>* node);
        Tnode<KeyType, DataType>* searchAssist(KeyType key);
        void rightRotate(Tnode<KeyType,DataType>* tnode);
        void leftRotate(Tnode<KeyType,DataType>* tnode);
        void balance(Tnode<KeyType,DataType>* tnode);
        StatusType insert(KeyType key, DataType data);
        StatusType remove(KeyType key);

        StatusType inOrderAssist(KeyType ***k, DataType ***d, int len);

        void print();
        void printN(Tnode<KeyType,DataType>* node, string indent, bool last);
        void printNode(Tnode<KeyType,DataType>* node);

        Tnode<KeyType,DataType>* arrayToTree(KeyType **mKeys, DataType **mData, 
                int start, int end, StatusType *result);
        Tree<KeyType, DataType>* mergeTree(Tree<KeyType,DataType>* t, StatusType* result);
        int countCondNodesAss(KeyType minKey, KeyType maxKey);
        StatusType specialCondNodeAss(DataType*** keys, KeyType minKey, KeyType maxKey, int len);
};

template<typename KeyType, typename DataType>
Tree<KeyType, DataType>::~Tree() {
    if(max_node)
        delete max_node;
    if(root!=nullptr)
        deleteAllNodes(root);
}
 
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::deleteAllNodes(Tnode<KeyType,DataType>* node) {
    if(node!=nullptr) {
        if(node->getLeft())
            deleteAllNodes(node->getLeft());
        if(node->getRight())
            deleteAllNodes(node->getRight());
        delete node;
    }
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
// O(LOG N)
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::findMaxNode(Tnode<KeyType,DataType>* node)
{
    if(!node)
        return;
    if(node->getRight()==nullptr)
    {
        *max_node=node->getKeyV();
        return;
    }
    findMaxNode(node->getRight());
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
    bool left=true;
    Tnode<KeyType,DataType> *parent=tnode->getParent();
    if(parent !=nullptr && parent->getLeft() != tnode)
        left=false;
    Tnode<KeyType,DataType> *leftson = tnode->getLeft(); 
    Tnode<KeyType,DataType> *righttree = leftson -> getRight(); 
    tnode->setLeft(righttree); 
    leftson -> setRight(tnode);
    tnode->setHeight(adjustHeight(tnode));
    leftson->setHeight(adjustHeight(leftson));
    if(righttree)
        righttree->setParent(tnode);
    if(parent!=nullptr)
    {
        if(left) {
            parent->setLeft(leftson);
        }
        else {
            parent->setRight(leftson);
        }
        leftson->setParent(parent);
        parent->setHeight(adjustHeight(parent));
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
    rightson -> setLeft(tnode);
    tnode->setHeight(adjustHeight(tnode));
    rightson->setHeight(adjustHeight(rightson));
    if(lefttree)
        lefttree->setParent(tnode);
    if(parent!=nullptr)
    {
        if(left)
            parent->setLeft(rightson);
        else
            parent->setRight(rightson);
        rightson->setParent(parent);
        parent->setHeight(adjustHeight(parent));
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
        }
        else
            insertN(tnode->getRight(), new_node);
 
    }
    else {
        if(!tnode->getLeft()) {
            tnode->setLeft(new_node);
            new_node->setParent(tnode);
        }
        else {
            insertN(tnode->getLeft(), new_node);
        }
 
    }
    tnode->setHeight(adjustHeight(tnode));
    balance(tnode);
}
 
template<typename KeyType, typename DataType> 
StatusType Tree<KeyType, DataType>::insert(KeyType key, DataType data)
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
        return ALLOCATION_ERROR;
    Tnode<KeyType, DataType> *new_node= new Tnode<KeyType, DataType>(key_,data_);
    if(!new_node)
        return ALLOCATION_ERROR;
    setCounter(getCounter()+1);
    if(!root) {
        root=new_node;
        *max_node=new_node->getKeyV();
    }
    else {
        insertN(root, new_node);
        findMaxNode(root);

    }
    return SUCCESS;
}
 
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::deleteN(Tnode<KeyType,DataType>* tnode, Tnode<KeyType,DataType>* parent ,KeyType key,int father)
{
    if(tnode==nullptr)
        return;
    else if(tnode->getKeyV() < key) // in the right tree
        deleteN(tnode->getRight(),tnode, key,RIGHT);
    else if(tnode->getKeyV() > key) // in the left tree
        deleteN(tnode->getLeft(),tnode, key,LEFT);
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
                adjustHeight(parent);
                balance(parent);
                return;
            }
            else // root
            {
                root=nullptr;
                delete tnode;
                return;
            }
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
                parent->decreaseHeight();
                balance(parent);
            }
            delete tnode;
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
                parent->decreaseHeight();
                balance(parent);
            }
            delete tnode;
            return;
        }
        else // has two sons
        {
            Tnode<KeyType,DataType> *tmpParent=tnode;
            Tnode<KeyType,DataType> *tmp = tnode->getRight();
            while(tmp->getLeft()!=nullptr)
            {
                tmpParent=tmp;
                tmp=tmp->getLeft();
            }
            if(tmpParent==tnode)
                tmpParent->setRight(tnode);
            else
                tmpParent->setLeft(tnode);
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
            if(tmpRight)
                tmpRight->setParent(tnode);
            tnode->setLeft(nullptr);
            tnode->setHeight(tmpH);
            deleteN(tmp->getRight(),tmp,key,RIGHT);
            adjustHeight(tmp);
            balance(tmp);
            return;
        }
    }
    adjustHeight(tnode);
    balance(tnode);
}

template<typename KeyType, typename DataType> 
StatusType Tree<KeyType, DataType>::remove(KeyType key)
{
    if(!search(root, key))
        return FAILURE;
    deleteN(root, nullptr, key, ROOT);
    setCounter(getCounter()-1);
    if(*max_node==key)
    {
        findMaxNode(root);
    }
    return SUCCESS;
}
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::inOrder(Tnode<KeyType,DataType>* node, KeyType ***k, DataType ***d, int *i, int len) {
    if(node && (*i)<len) {
        inOrder(node->getLeft(),k,d,i,len);
        if(*i<len)
        {
            **k=node->getKey();
            (*k)++;
            (*i)++;
            if(d!=nullptr) {
                **d = node->getData();
                (*d)++;
            }
        }
        inOrder(node->getRight(),k,d,i,len);
    }
}
template<typename KeyType, typename DataType>
StatusType Tree<KeyType, DataType>::inOrderAssist(KeyType ***k, DataType ***d, int len) {
    KeyType **tmpK = *k;
    int *i=new int(0);
    if(i==nullptr)
        return ALLOCATION_ERROR;
    if(d)
    {
        DataType **tmpD = *d;
        inOrder(root, k, d, i, len);
        *d=tmpD;
    }
    else
        inOrder(root, k, d, i, len);
    *k=tmpK;
    delete i;
    return SUCCESS;
}
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::print() {
    printN(root, "", true);
    cout << endl;
}
template<typename KeyType, typename DataType>
void Tree<KeyType, DataType>::printN(Tnode<KeyType,DataType>* node, string indent, bool last) {
  if (node != nullptr) {
    cout << indent;
    if (last) {
      cout << "R----";
      indent += "   ";
    } else {
      cout << "L----";
      indent += "|  ";
    }
    cout << node->getKeyV() << endl;
    printN(node->getLeft(), indent, false);
    printN(node->getRight(), indent, true);
  }
}
template<typename KeyType, typename DataType> 
void Tree<KeyType, DataType>::printNode(Tnode<KeyType,DataType>* node)
{
    if(node) {
        cout << "NODE: " << endl;
        cout << "VAL: " << node->getKeyV() << endl;
        if(node->getRight())
            cout << "RIGHT: " << node->getRight()->getKeyV() << endl;
        if(node->getLeft())
            cout << "LEFT: " <<node->getLeft()->getKeyV() << endl;
        if(node->getParent())
            cout << "PARENT: " <<node->getParent()->getKeyV() << endl;
        cout << endl;
    }
}
template<typename KeyType, typename DataType>
Tnode<KeyType,DataType>* Tree<KeyType,DataType>::arrayToTree(KeyType **mKeys, DataType **mData, 
                int start, int end, StatusType *result)
{
    if(start>end) {
        return nullptr;
    }
    int mid = (start+end)/2;
    KeyType *key = new KeyType(*mKeys[mid]);
    if(key==nullptr)
    {
        *result=ALLOCATION_ERROR;
        return nullptr;
    }
    DataType *data= new DataType(*mData[mid]);
    if(data==nullptr)
    {
        *result=ALLOCATION_ERROR;
        return nullptr;
    }
    Tnode<KeyType,DataType>* node = new Tnode<KeyType,DataType>(key,data);
    if(!node)
    {
        *result = ALLOCATION_ERROR;
        return nullptr;        
    }
    node->setLeft(arrayToTree(mKeys,mData,start,mid-1,result));
    if(*result != SUCCESS)
    {
        return nullptr;
    }
    if(node->getLeft())
        (node->getLeft())->setParent(node);
    node->setRight(arrayToTree(mKeys,mData,mid+1,end,result));
    if(*result != SUCCESS)
    {
        return nullptr;
    }
    if(node->getRight())
        (node->getRight())->setParent(node);
    adjustHeight(node);
    balance(node);    
    return node;
}

template<typename KeyType, typename DataType>
Tree<KeyType, DataType>* Tree<KeyType,DataType>::mergeTree(Tree<KeyType,DataType>* t, StatusType* result)
{
    int treeLen1 = getCounter(), treeLen2 = t->getCounter();
    KeyType **tree1K = new KeyType*[treeLen1];
    if(tree1K==nullptr)
    {
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    KeyType **tree2K = new KeyType*[treeLen2];
    if(tree2K==nullptr)
    {
        delete[] tree1K;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    DataType **tree1D = new DataType*[treeLen1];
    if(tree1D==nullptr)
    {
        delete[] tree1K;
        delete[] tree2K;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    DataType **tree2D = new DataType*[treeLen2];
    if(tree2D==nullptr)
    {
        delete[] tree1K;
        delete[] tree2K;
        delete[] tree1D;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    KeyType **mergedTreeK = new KeyType*[treeLen1+treeLen2];
    if(mergedTreeK==nullptr)
    {
        delete[] tree1K;
        delete[] tree2K;
        delete[] tree1D;
        delete[] tree2D;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    DataType **mergedTreeD = new DataType*[treeLen1+treeLen2];
    if(mergedTreeD==nullptr)
    {
        delete[] tree1K;
        delete[] tree2K;
        delete[] tree1D;
        delete[] tree2D;
        delete[] mergedTreeK;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    KeyType ***ptr1K = new KeyType**();
    if(ptr1K==nullptr)
    {
        delete[] tree1K;
        delete[] tree2K;
        delete[] tree1D;
        delete[] tree2D;
        delete[] mergedTreeK;
        delete[] mergedTreeD;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    KeyType ***ptr2K = new KeyType**();
    if(ptr2K==nullptr)
    {
        delete[] tree1K;
        delete[] tree2K;
        delete[] tree1D;
        delete[] tree2D;
        delete[] mergedTreeK;
        delete[] mergedTreeD;
        delete ptr1K;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    DataType ***ptr1D = new DataType**();
    if(ptr1D==nullptr)
    {
        delete[] tree1K;
        delete[] tree2K;
        delete[] tree1D;
        delete[] tree2D;
        delete[] mergedTreeK;
        delete[] mergedTreeD;
        delete ptr1K;
        delete ptr2K;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    DataType ***ptr2D = new DataType**();
    if(ptr2D==nullptr)
    {
        delete[] tree1K;
        delete[] tree1D;
        delete[] tree2K;
        delete[] tree2D;
        delete[] mergedTreeK;
        delete[] mergedTreeD;
        delete ptr1K;
        delete ptr2K;
        delete ptr1D;
        *result = ALLOCATION_ERROR;
        return nullptr;
    }
    *ptr1K=tree1K;
    *ptr2K=tree2K;
    *ptr1D=tree1D;
    *ptr2D=tree2D;
    *result = inOrderAssist(ptr1K,ptr1D, treeLen1);
    if(*result!=SUCCESS)
    {
        delete[] tree1K;
        delete[] tree1D;
        delete[] tree2K;
        delete[] tree2D;
        delete[] mergedTreeK;
        delete[] mergedTreeD;
        delete ptr1K;
        delete ptr2K;
        delete ptr1D;
        delete ptr2D;
        return nullptr;
    }
    *result = t->inOrderAssist(ptr2K,ptr2D, treeLen2);
    if(*result!=SUCCESS)
    {
        delete[] tree1K;
        delete[] tree1D;
        delete[] tree2K;
        delete[] tree2D;
        delete[] mergedTreeK;
        delete[] mergedTreeD;
        delete ptr1K;
        delete ptr2K;
        delete ptr1D;
        delete ptr2D;
        return nullptr;
    }
    int t1=0,t2=0;
    int mt=0;
    while(t1<treeLen1 && t2<treeLen2)
    {
        if(*tree1K[t1]<*tree2K[t2]) {
            mergedTreeK[mt]=tree1K[t1];
            mergedTreeD[mt]=tree1D[t1];
            t1++;
        }
        else if(*tree1K[t1]>*tree2K[t2]){
            mergedTreeK[mt]=tree2K[t2];
            mergedTreeD[mt]=tree2D[t2];
            t2++;
        }
        mt++;
    }
    while(t1<treeLen1)
    {
        mergedTreeK[mt]=tree1K[t1];
        mergedTreeD[mt]=tree1D[t1];
        t1++;
        mt++;
    }
    while(t2<treeLen2)
    {
        mergedTreeK[mt]=tree2K[t2];
        mergedTreeD[mt]=tree2D[t2];
        t2++;
        mt++;  
    }
    Tnode<KeyType,DataType>* new_root;
    new_root = arrayToTree(mergedTreeK,mergedTreeD,0, treeLen1+treeLen2-1, result);
    delete[] tree1K;
    delete[] tree1D;
    delete[] tree2K;
    delete[] tree2D;
    delete[] mergedTreeK;
    delete[] mergedTreeD;
    delete ptr1K;
    delete ptr2K;
    delete ptr1D;
    delete ptr2D;
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
        new_t->findMaxNode(new_root);
        new_t->treeHeight(new_t->getRoot());
        return new_t;     
    }
    return nullptr;
}
template<typename KeyType, typename DataType>
int Tree<KeyType,DataType>::countCondNodes(Tnode<KeyType, DataType>* tnode, KeyType minKey, KeyType maxKey)
{
    if(!tnode)
        return 0;
    if(tnode->getKeyV() == minKey && tnode->getKeyV() == maxKey)
        return 1;
    if(tnode->getKeyV() < minKey) // smaller, not allowed
        return countCondNodes(tnode->getRight(), minKey, maxKey);
    if(tnode->getKeyV() > maxKey) // bigger, not allowed
        return countCondNodes(tnode->getLeft(), minKey, maxKey);
    // between the values or equal to one of them
    if(tnode->getLeft() && tnode->getRight()) // has two sons
        return 1+countCondNodes(tnode->getLeft(),minKey,maxKey)+
                countCondNodes(tnode->getRight(),minKey,maxKey);
    if(tnode->getLeft()) // only left son
        return 1+countCondNodes(tnode->getLeft(),minKey,maxKey);
    return 1+countCondNodes(tnode->getRight(),minKey,maxKey); // only right son
}
template<typename KeyType, typename DataType>
int Tree<KeyType,DataType>::countCondNodesAss(KeyType minKey, KeyType maxKey)
{
    return countCondNodes(root,minKey,maxKey);
}
template<typename KeyType, typename DataType>
void Tree<KeyType,DataType>::specialCondNodes(Tnode<KeyType, DataType>* tnode, DataType*** data, 
                                                            KeyType minKey, KeyType maxKey, int *i,int len)
{
    if(!tnode || *i>=len)
        return;
    if(tnode->getKeyV() == minKey && tnode->getKeyV() == maxKey)
    {
        **data=tnode->getData();
        (*data)++;
        (*i)++;
        return;
    }
    if(tnode->getKeyV() < minKey) // smaller, not allowed
    {
        specialCondNodes(tnode->getRight(), data, minKey, maxKey,i,len);
        return;
    }
    if(tnode->getKeyV() > maxKey) // bigger, not allowed
    {
        specialCondNodes(tnode->getLeft(), data, minKey, maxKey,i,len);
        return;
    }
    // between the values
    if(*i>=len)
        return;
    **data=tnode->getData();
    (*data)++;
    (*i)++;
    if(tnode->getRight())
    {
        specialCondNodes(tnode->getRight(), data, minKey, maxKey,i,len);
    }
    if(tnode->getLeft()) 
    {
        specialCondNodes(tnode->getLeft(), data, minKey, maxKey,i,len);
    }   
}
template<typename KeyType, typename DataType>
StatusType Tree<KeyType,DataType>::specialCondNodeAss(DataType*** data, KeyType minKey, KeyType maxKey, int len)
{
    DataType **tmp = *data;
    int *i = new int(0);
    if(i==nullptr)
        return ALLOCATION_ERROR;
    specialCondNodes(root, data, minKey, maxKey,i,len);
    *data=tmp;
    delete i;
    return SUCCESS;
}
#endif
#ifndef COMPANY_H_
#define COMPANY_H_

#include <iostream>
#include "employee.h"
#include "avl.h"
using namespace std;

class Company {
    private:
    int id;
    int value;
    Tree<Employee, Employee>* salarySorted; // avl tree sorted by salary
    Tree<int, Employee>* idSorted; // avl tree sorted by id

    public:
    Company(int id, int value): id(id), value(value), salarySorted(nullptr), idSorted(nullptr) {}
    Company(): id(0),value(0),salarySorted(nullptr), idSorted(nullptr) {}
    Company(const Company& comp)
    {
        id=comp.id;
        value=comp.value;
        salarySorted=nullptr;
        idSorted=nullptr;
    }
    ~Company();

    int getId() { return id; }
    int getValue() { return value; }
    Tree<Employee,Employee>* getSalaryTree() { return salarySorted; }
    Tree<int,Employee>* getIdTree() { return idSorted; }

    void setId(int id_) { id=id_; }
    void setValue(int value_) { value=value_; } 
    void setSalaryTree(Tree<Employee, Employee>* salarySorted_) { salarySorted=salarySorted_; }
    void setIdTree(Tree<int, Employee>* idSorted_) { idSorted=idSorted_; }
    
    StatusType mergeCompanies(Company *target, int value);

    Company& operator=(const Company& comp)
    {
        if(&comp==this)
            return *this;
        id=comp.id;
        value=comp.value;
        salarySorted=comp.salarySorted;
        idSorted=comp.idSorted;
        return *this;
    }
};


Company::~Company()
{
    delete salarySorted;
    delete idSorted;
}
StatusType Company::mergeCompanies(Company *target, int value)
{
    setValue(value);
    StatusType result = SUCCESS;
    Tree<Employee,Employee>* tmpT1 = salarySorted->mergeTree(target->getSalaryTree(), &result);
    if(result!=SUCCESS)
        return result;
    delete salarySorted;
    salarySorted = tmpT1;
    Tree<int,Employee> *tmpT2 = idSorted->mergeTree(target->getIdTree(), &result);
    if(result!=SUCCESS)
        return result;
    delete idSorted;
    idSorted=tmpT2;
    return result;
}

#endif
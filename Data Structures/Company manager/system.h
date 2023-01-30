#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <iostream>
#include "company.h"
using namespace std;

class System {
    private:
    Tree<int, int>* companies; // COMPANIES: sorted by COMPANY ID, has ID and VALUE
    Tree<int, Company>* settledCompanies; // COMPANIES: sorted by COMPANY ID
    Tree<Employee, Employee>* salarySortedEmp; // EMPLOYEES: sorted by EMPLOYEE SALARY
    Tree<int, Employee>* idSortedEmp; // EMPLOYEES: sorted by EMPLOYEE ID
    public:
    System() : companies(nullptr), settledCompanies(nullptr), salarySortedEmp(nullptr), idSortedEmp(nullptr) {}
    ~System();

    Tree<int, int>* getCompanies() { return companies; }

    Tree<int, Company>* getSettledCompanies() { return settledCompanies; }
    Tree<Employee, Employee>* getSalarySortedEmp() { return salarySortedEmp; }
    Tree<int, Employee>* getIdSortedEmp() { return idSortedEmp; }

    void setCompanies(Tree<int, int>* companies_) { companies = companies_; }
    void setSettledCompanies(Tree<int, Company>* settledCompanies_) { settledCompanies = settledCompanies_; }
    void setSalarySortedEmp(Tree<Employee, Employee>* salarySortedEmp_) { salarySortedEmp = salarySortedEmp_; }
    void setIdSortedEmp(Tree<int, Employee>* idSortedEmp_) { idSortedEmp = idSortedEmp_; }
    
    void deleteEmpsCIDs(Tnode<int,Employee>* node);
};

System::~System()
{
    if(companies)
        delete companies;
    if(settledCompanies)
        delete settledCompanies;
    if(salarySortedEmp)
        delete salarySortedEmp;
    if(idSortedEmp) {
        deleteEmpsCIDs(idSortedEmp->getRoot());
        delete idSortedEmp;
    }
}

void System::deleteEmpsCIDs(Tnode<int,Employee>* node)
{
    if(!node)
        return;
    (node->getData())->deleteCompId();
    deleteEmpsCIDs(node->getLeft());
    deleteEmpsCIDs(node->getRight());
}

#endif
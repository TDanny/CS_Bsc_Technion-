#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <iostream>
#include "company.h"
#include "uf.h"

using namespace std;

class System {
    private:
        int size;
        int numEmpsWithSalZ;
        int sumEmpsWithSalZ;
        UnionFind* companies; // COMPANIES: sorted by COMPANY ID, has ID and VALUE
        hashTable<int,Employee>* AllEmployees; // EMPLOYEES: ALL EMPLOYEES 
        Tree<Employee, Employee>* salarySortedEmpNoZ; // EMPLOYEES: sorted by EMPLOYEE SALARY WITHOUT 0
    public:
        System(int k) : size(k), numEmpsWithSalZ(0),sumEmpsWithSalZ(0), companies(nullptr), 
                    AllEmployees(nullptr), salarySortedEmpNoZ(nullptr) {}
        ~System();

        void UFcompanies(int k) {
            try{
                companies = new UnionFind(k);
            }
            catch(std::bad_alloc& e) {
                throw;
            }
        } 

        int getSize() { return size; }
        int getEmpsNum() { return numEmpsWithSalZ; }
        int getSumGrades() { return sumEmpsWithSalZ; }
        UnionFind* getCompanies() {return companies;}
        hashTable<int,Employee>* getAllEmployees() { return AllEmployees; }
        Tree<Employee, Employee>* getSalarySorted() { return salarySortedEmpNoZ; }

        void setSize(int k) { size=k; }
        void increaseEmps() { 
            numEmpsWithSalZ++;
        }
        void decreaseEmps() {
            numEmpsWithSalZ--; 
            if(numEmpsWithSalZ<0)
                numEmpsWithSalZ=0;
        }
        void setEmps(int emps) { numEmpsWithSalZ=emps; }
        void increaseGrades(int grades) { sumEmpsWithSalZ+=grades; }
        void decreaseGrades(int grades) { 
            sumEmpsWithSalZ-=grades;
            if(sumEmpsWithSalZ<0)
                sumEmpsWithSalZ=0;
        }
        void setGrades(int grades) { sumEmpsWithSalZ=grades; }
        void setCompanies(UnionFind* companies_) { companies = companies_; }
        void setAllEmployees(hashTable<int,Employee>* AllEmployees_) { AllEmployees = AllEmployees_; }
        void setSalarySorted(Tree<Employee, Employee>* salarySortedEmpNoZ_) { salarySortedEmpNoZ = salarySortedEmpNoZ_; }
        
        void deleteEmpsCIDs(hashTable<int,Employee>* ht);
};

System::~System()
{
    if(companies)
        delete companies;
    if(getAllEmployees()) {
        deleteEmpsCIDs(AllEmployees);
        delete AllEmployees;

    }
    if(salarySortedEmpNoZ)
    {
        delete salarySortedEmpNoZ;
    }
    companies=nullptr;
    AllEmployees=nullptr;
    salarySortedEmpNoZ=nullptr;
}

void System::deleteEmpsCIDs(hashTable<int,Employee>* ht)
{
    if(ht==nullptr)
        return;
    Ht_item<int,Employee>** arr = ht->getArr();
    Ht_item<int,Employee>* tmp;
    if(arr==nullptr)
        return;
    for(int i=0;i<ht->getSize();i++)
    {
        if(arr[i]!=nullptr)
        {
            tmp=arr[i];
            while(tmp!=nullptr)
            {
                (tmp->getData())->deleteCompId();
                tmp=tmp->getNext();
            }
        }
    }
}
#endif
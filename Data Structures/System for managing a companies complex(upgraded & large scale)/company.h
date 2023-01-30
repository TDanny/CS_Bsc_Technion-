#ifndef COMPANY_H_
#define COMPANY_H_

#include <iostream>
#include "ranked.h"
#include "employee.h"
#include "hashTable.h"

using namespace std;

class Company {
    private:
    int id;
    double value;
    long long numEmpsWithSalZ;
    long long sumEmpsWithSalZ;
    Tree<Employee,Employee>* salarySortedTreeNoZ;
    hashTable<int,Employee>* compEmployees;
    public:
        Company(int id): id(id), value(id),  numEmpsWithSalZ(0),sumEmpsWithSalZ(0), salarySortedTreeNoZ(nullptr),
                            compEmployees(nullptr) {}
        ~Company() {
            if(salarySortedTreeNoZ!=nullptr) {
                delete salarySortedTreeNoZ;
                salarySortedTreeNoZ=nullptr;
            }
            if(compEmployees!=nullptr) {
                delete compEmployees;
                compEmployees=nullptr;
            }
        }

        int getId() { return id; }
        double getVal() { return value; }
        long long getEmpsNum() { return numEmpsWithSalZ; }
        long long getSumGrades() { return sumEmpsWithSalZ; }
        Tree<Employee,Employee>* getSalarySorted() { return salarySortedTreeNoZ; }
        hashTable<int,Employee>* getCompEmps() { return compEmployees; }


        void setId(int id_) { id=id_; }
        void setVal(double val_) { value=val_; }
        void increaseEmps() { numEmpsWithSalZ++; }
        void decreaseEmps() { 
            numEmpsWithSalZ--; 
            if(numEmpsWithSalZ<0)
                numEmpsWithSalZ=0;
        }
        void setEmps(long long emps) { numEmpsWithSalZ=emps; }
        void increaseGrades(long long grades) { sumEmpsWithSalZ+=grades; }
        void decreaseGrades(long long grades) { 
            sumEmpsWithSalZ-=grades;
            if(sumEmpsWithSalZ<0)
                sumEmpsWithSalZ=0;
        }
        void setGrades(long long grades) { sumEmpsWithSalZ=grades; }
        void setSalarySorted(Tree<Employee,Employee>* t) { salarySortedTreeNoZ=t; }
        void setCompEmps(hashTable<int,Employee>* ht) { compEmployees=ht; }
};


#endif
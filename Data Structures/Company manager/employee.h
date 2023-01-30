#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <iostream>
using namespace std;

class Employee {
    private:
    int id;
    int salary;
    int grade;
    int *companyID;
    public:
    Employee(int id, int salary, int grade, int* companyID): id(id), salary(salary), grade(grade), companyID(companyID) {}
    Employee(const Employee& e)
    {
        id=e.id;
        salary=e.salary;
        grade=e.grade;
        companyID=e.companyID;
    }
    Employee(): id(0), salary(0), grade(0), companyID(nullptr) {}
    ~Employee() {}

    int getSalary() { return salary; }
    int getGrade() { return grade; }
    int getId() { return id; }
    int getIds() const { return id; }
    int getCompanyID() { return *companyID; }
    int getCompanyIDs() const { return *companyID; }
    int* getCompanyAdd() { return companyID; }

    void setSalary(int salary_) { salary=salary_; }
    void setGrade(int grade_) { grade=grade_; }
    void setId(int id_) { id=id_; }
    void setCompanyID (int id_) { *companyID = id_; }
    void setCompAdd (int* add) { companyID=add; }
    void increaseSalary(int increase) { salary+=increase; }
    
    void deleteCompId() { 
        if(companyID) 
            delete companyID; 
    }

    Employee& operator=(const Employee& e)
    {
        id=e.id;
        salary=e.salary;
        grade=e.grade;
        companyID=e.companyID;
        return *this;
    }
    friend bool operator<(const Employee& e1, const Employee& e2)
    {
        if(e1.salary<e2.salary)
            return true;
        else if(e1.salary==e2.salary && e1.id > e2.id)
            return true;
        return false;
    }
    friend bool operator==(const Employee& e1, const Employee& e2)
    {
        if(e1.id==e2.id)
            return true;
        return false;
    }
    friend bool operator>(const Employee& e1, const Employee& e2)
    {
        if(e1.salary > e2.salary)
            return true;
        else if(e1.salary == e2.salary && e1.id < e2.id)
            return true;
        return false;
    }

};

std::ostream& operator<< (std::ostream &out, const Employee& data) {
    out << "EMP:" << data.getIds() << ", COMPANY: " << data.getCompanyIDs();
    return out;
}
#endif
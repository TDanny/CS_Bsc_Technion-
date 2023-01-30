#include <iostream>
#include "system.h"

using namespace std;

// HELPER FUNCTIONS

void AverageBumpEmpsLower(Tnode<Employee,Employee>* node, int lower, long long *counter,long long *sum);
void AverageBumpEmpsHigher(Tnode<Employee,Employee>* node, int higher, long long *counter,long long *sum);
void adjustCompanyID(hashTable<int,Employee>* ht, int target, int new_comp);

// IMPLMENTATIONS

// BOTH OF THESE IN LOG N
void AverageBumpEmpsLower(Tnode<Employee,Employee>* node, int lower, long long *counter,long long *sum)
{
    if(node==nullptr)
        return;
    int salary = (node->getData())->getSalary();
    if(salary>=lower) // if the root of the current subtree is above lower, than obviously his right subtree is above lower
    {
        AverageBumpEmpsLower(node->getLeft(), lower, counter, sum);
    }
    else
    {
        *sum+=node->getSumGrades();
        *counter+=node->getTreeNodes();
        if(node->getRight())
        {
            *sum-=node->getRight()->getSumGrades();
            *counter-=node->getRight()->getTreeNodes();
            AverageBumpEmpsLower(node->getRight(), lower, counter, sum);
        }
    }
}
// symmetric
void AverageBumpEmpsHigher(Tnode<Employee,Employee>* node, int higher, long long *counter,long long *sum)
{
    if(node==nullptr)
        return;
    int salary = (node->getData())->getSalary();
    if(salary<=higher) // if the root of the current subtree is below higher, than obviously his left subtree is below higher
    {
        AverageBumpEmpsHigher(node->getRight(), higher, counter, sum);
    }
    else
    {
        *sum+=node->getSumGrades();
        *counter+=node->getTreeNodes();
        if(node->getLeft())
        {
            *sum-=node->getLeft()->getSumGrades();
            *counter-=node->getLeft()->getTreeNodes();
            AverageBumpEmpsHigher(node->getLeft(), higher, counter, sum);
        }
    }
}

void adjustCompanyID(hashTable<int,Employee>* ht, int target, int new_comp)
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
                if((tmp->getData())->getCompanyID()==target)
                    (tmp->getData())->setCompanyID(new_comp);
                tmp=tmp->getNext();
            }
        }
    }
}

// MAIN FUNCTIONS


void* Init(int k)
{
    if(k<1)
        return nullptr;
    System *DS=new System(k);
    if(DS==nullptr)
        return nullptr;
    try
    {
        DS->UFcompanies(k);
    }
    catch(const std::exception& e)
    {
        return nullptr;
    }
    return (void*)DS;
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade)
{
    if(DS==nullptr || employeeID<=0 || companyID<=0 || grade<0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(system->getSize() < companyID)
        return INVALID_INPUT;
    if(system->getAllEmployees() == nullptr)
    {
        hashTable<int, Employee> *ht;
        try {
            ht = new hashTable<int, Employee>();
        }
        catch(const std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
        system->setAllEmployees(ht);
    }
    Company* comp = (system->getCompanies())->find(companyID);
    int* compID = new int(comp->getId());
    if(compID==nullptr)
        return ALLOCATION_ERROR;
    Employee* e = new Employee(employeeID, 0, grade, compID);
    if(e==nullptr)
    {
        delete compID;
        return ALLOCATION_ERROR;
    }
    StatusType res = (system->getAllEmployees())->insert(employeeID, *e);
    if(res!=SUCCESS)
    {
        delete compID;
        delete e;
        return res;
    }
    if(comp->getCompEmps() == nullptr)
    {
        hashTable<int, Employee> *ht;
        try {
            ht = new hashTable<int, Employee>();
        }
        catch(const std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
        comp->setCompEmps(ht);
    }
    res = (comp->getCompEmps())->insert(employeeID, *e);
    if(res!=SUCCESS)
    {
        delete compID;
        delete e;
        return res;
    }
    system->increaseEmps();
    system->increaseGrades(grade);
    comp->increaseEmps();
    comp->increaseGrades(grade);
    delete e;
    return res;
}


StatusType RemoveEmployee(void *DS, int employeeID)
{
    if(DS==nullptr || employeeID<=0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    hashTable<int,Employee>* ht = system->getAllEmployees();
    if(ht==nullptr)
        return FAILURE;
    Employee* e = ht->find(employeeID);
    if(e==nullptr)
    {
        return FAILURE;
    }
    StatusType res = SUCCESS;
    Unode **comps = (system->getCompanies())->getArr();
    Company* comp = comps[e->getCompanyID()-1]->getData();
    if(e->getSalary()>0)
    {
        res=system->getSalarySorted()->remove(*e,e->getGrade());
        if(res!=SUCCESS)
            return res;
        res=comp->getSalarySorted()->remove(*e,e->getGrade());
        if(res!=SUCCESS)
            return res;
    }
    else
    {
        system->decreaseEmps();
        system->decreaseGrades(e->getGrade());
        comp->decreaseEmps();
        comp->decreaseGrades(e->getGrade());
    }
    res=comp->getCompEmps()->remove(employeeID);
    if(res!=SUCCESS)
        return res;
    e->deleteCompId();
    return ht->remove(employeeID);
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor)
{
    if(!DS || companyID1<=0 || companyID2<=0 || companyID1==companyID2 || factor<=0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    int k = (system->getCompanies())->getSize();
    if(companyID1 > k || companyID2 > k) 
        return INVALID_INPUT;
    Unode* company1 = (system->getCompanies())->find_assist(companyID1);  
    Unode* company2 = (system->getCompanies())->find_assist(companyID2);
    Company* acquirCmp;
    Company* targetCmp;
    int return_val=(system->getCompanies())->Union(companyID1,companyID2,factor);
    if(return_val==3)
        return INVALID_INPUT;
    else if(return_val==2)
    {
        acquirCmp = company2->getData();
        targetCmp = company1->getData();
    }
    else 
    {
        acquirCmp = company1->getData();
        targetCmp = company2->getData();   
    }
    Tree<Employee,Employee>* t_acq = acquirCmp->getSalarySorted();
    Tree<Employee,Employee>* t_trg = targetCmp->getSalarySorted();
    hashTable<int,Employee>* htAcqComp = acquirCmp->getCompEmps();
    hashTable<int,Employee>* htTrgComp = targetCmp->getCompEmps();
    if(t_trg!=nullptr)
    {
        if(t_acq==nullptr)
        {
            acquirCmp->setSalarySorted(t_trg);
        }
        else
        {
            StatusType mergeRes = SUCCESS;
            Tree<Employee,Employee>* tmpT = t_acq->mergeTree(t_trg, &mergeRes);
            if(mergeRes!=SUCCESS)
                return mergeRes;
            delete t_acq;
            delete t_trg;
            t_acq=nullptr;
            t_trg=nullptr;
            acquirCmp->setSalarySorted(tmpT);
        }
        targetCmp->setSalarySorted(nullptr);
    }
    if(htTrgComp == nullptr || htTrgComp->getCounter()==0)
        return SUCCESS;
    if(htAcqComp==nullptr)
    {
        try {
            htAcqComp = new hashTable<int, Employee>();
        }
        catch(const std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
    }
    StatusType res = htAcqComp->mergeHTs(htTrgComp);
    if(res!=SUCCESS)
        return res;
    delete htTrgComp;
    targetCmp->setCompEmps(nullptr);
    acquirCmp->setCompEmps(htAcqComp);
    adjustCompanyID(htAcqComp, targetCmp->getId(),acquirCmp->getId());
    acquirCmp->setEmps(acquirCmp->getEmpsNum()+targetCmp->getEmpsNum());
    acquirCmp->increaseGrades(targetCmp->getSumGrades());
    return SUCCESS;
}

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease)
{
    if(DS==nullptr || employeeID<=0 || salaryIncrease<=0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    hashTable<int,Employee>* ht = system->getAllEmployees();
    if(ht==nullptr)
        return FAILURE;
    Employee* ht_e = ht->find(employeeID);
    if(ht_e==nullptr)
        return FAILURE;
    Employee *e = new Employee(ht_e->getId(), ht_e->getSalary()+salaryIncrease, ht_e->getGrade(), ht_e->getCompanyAdd());
    if(e==nullptr)
        return ALLOCATION_ERROR;
    StatusType res = SUCCESS;
    Unode **comps = (system->getCompanies())->getArr();
    Company* comp = comps[e->getCompanyID()-1]->getData();
    if(ht_e->getSalary()==0)
    {
        Tree<Employee, Employee>* salaryTree = system->getSalarySorted();
        if(salaryTree==nullptr)
        {
            try {
                salaryTree=new Tree<Employee,Employee>();
            }
            catch(const std::bad_alloc& e)
            {
                return ALLOCATION_ERROR;
            }
            system->setSalarySorted(salaryTree);
        }
        res = salaryTree->insert(*e, *e, e->getGrade());
        if(res!=SUCCESS)
            return res;
        salaryTree = comp->getSalarySorted();
        if(salaryTree==nullptr)
        {
            try {
                salaryTree=new Tree<Employee,Employee>();
            }
            catch(const std::bad_alloc& e)
            {
                return ALLOCATION_ERROR;
            }
            comp->setSalarySorted(salaryTree);
        }
        res = salaryTree->insert(*e,*e, e->getGrade());
        if(res!=SUCCESS)
            return res;
        system->decreaseEmps();
        system->decreaseGrades(e->getGrade());
        comp->decreaseEmps();
        comp->decreaseGrades(e->getGrade());
    }
    else
    {
        res=system->getSalarySorted()->remove(*ht_e, ht_e->getGrade());
        if(res!=SUCCESS)
            return res;
        res=system->getSalarySorted()->insert(*e, *e, e->getGrade());
        if(res!=SUCCESS)
            return res;
        if(system->getCompanies() == nullptr)
            return FAILURE;
        res=comp->getSalarySorted()->remove(*ht_e, ht_e->getGrade());
        if(res!=SUCCESS)
            return res;
        res=comp->getSalarySorted()->insert(*e, *e, e->getGrade());
        if(res!=SUCCESS)
            return res;

    }
    ht_e->setSalary(e->getSalary());
    ht_e=comp->getCompEmps()->find(employeeID);
    ht_e->setSalary(e->getSalary());
    delete e;
    return SUCCESS;
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade)
{
    if(DS==nullptr || employeeID<=0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    hashTable<int,Employee>* ht = system->getAllEmployees();
    if(ht==nullptr)
        return FAILURE;
    Employee* e = ht->find(employeeID);
    if(e==nullptr)
        return FAILURE;
    if(bumpGrade>0) 
    {
        int cur_grade=e->getGrade();
        e->setGrade(cur_grade+bumpGrade);
        if(e->getSalary()==0)
        {
            system->increaseGrades(bumpGrade);
        }
        Unode **comps = (system->getCompanies())->getArr();
        Company* comp = comps[(e->getCompanyID())-1]->getData();
        
        if(comp==nullptr)
            return FAILURE;
        hashTable<int,Employee>* compHt = comp->getCompEmps();
        if(compHt==nullptr)
            return FAILURE;
        e= compHt->find(employeeID);
        if(e==nullptr)
            return FAILURE;
        e->setGrade(cur_grade+bumpGrade);
        if(e->getSalary()==0)
        {
            comp->increaseGrades(bumpGrade); 
        }
        if(e->getSalary()>0)
        {
            Tnode<Employee,Employee>* enode;
            enode=system->getSalarySorted()->searchAssist(*e);
            if(enode==nullptr)
                return FAILURE;
            enode->setTGrade(cur_grade+bumpGrade);
            enode->getData()->setGrade(cur_grade+bumpGrade);
            enode->getKey()->setGrade(cur_grade+bumpGrade);
            enode->specialSetGrades(enode,bumpGrade);
            if(system->getCompanies() == nullptr)
                return FAILURE;
            enode=comp->getSalarySorted()->searchAssist(*e);
            if(enode==nullptr)
                return FAILURE;
            enode->setTGrade(cur_grade+bumpGrade);
            enode->getData()->setGrade(cur_grade+bumpGrade);
            enode->getKey()->setGrade(cur_grade+bumpGrade);
            enode->specialSetGrades(enode,bumpGrade);
        }
    }
    return SUCCESS;
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m)
{
    if(DS==nullptr || companyID<0 || m<=0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(system->getSize() < companyID)
        return INVALID_INPUT;
    Tree<Employee,Employee> *t;
    if(companyID==0)
    {
        t = system->getSalarySorted();
    }
    else
    {
        Company *comp = (system->getCompanies())->find(companyID);
        // compID is less than K then we know comp exists
        t = comp->getSalarySorted();
    }
    if(t==nullptr || t->getCounter() < m)
        return FAILURE;
    long long *sum=new long long(0);
    if(sum==nullptr)
        return ALLOCATION_ERROR;
    // log n to go to the rightest
    StatusType res = t->specialInOrderAssist(sum,m);
    if(res!=SUCCESS)
        return res;
    printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", *sum);
    delete sum;
    return SUCCESS;
}


StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary)
{
    if(DS==nullptr || companyID < 0 || lowerSalary < 0 || higherSalary < 0 || lowerSalary > higherSalary)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(system->getSize() < companyID)
        return INVALID_INPUT;
    Tree<Employee,Employee> *t;
    long long sum_grades=0, num_emps=0;
    if(companyID==0)
    {
        t = system->getSalarySorted();
        sum_grades= lowerSalary==0 ? system->getSumGrades() : 0;
        num_emps=lowerSalary==0 ? system->getEmpsNum() : 0;
    }
    else
    {
        Company *comp = (system->getCompanies())->find(companyID);
        // compID is less than K then we know comp exists
        t = comp->getSalarySorted();
        sum_grades= lowerSalary==0 ? comp->getSumGrades() : 0;
        num_emps=lowerSalary==0 ? comp->getEmpsNum() : 0;
    }
    if(t==nullptr || t->getCounter() == 0)
    {
        if(num_emps==0)
            return FAILURE;
        double average = (double)sum_grades/num_emps;
        printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", average);
        return SUCCESS;
    }
    //t->print();
    long long *sum=new long long(0);
    if(sum==nullptr)
        return ALLOCATION_ERROR;
    long long *num = new long long(0);
    if(num==nullptr)
        return ALLOCATION_ERROR;
    AverageBumpEmpsHigher(t->getRoot(), higherSalary, num, sum);
    AverageBumpEmpsLower(t->getRoot(), lowerSalary, num, sum);
    sum_grades += (t->getRoot())->getSumGrades()-(*sum);
    num_emps += (t->getRoot())->getTreeNodes()-(*num);
    delete sum;
    delete num;
    if(num_emps==0)
        return FAILURE;
    double average = (double)sum_grades/num_emps;
    printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", average);
    return SUCCESS;
}

StatusType CompanyValue(void *DS, int companyID)
{
    if(!DS || companyID<=0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    int k = (system->getCompanies())->getSize();
    if(companyID > k) 
        return INVALID_INPUT;
    Unode** array = (system->getCompanies())->getArr();
    double* standing= new double(0);
    if(standing==nullptr)
        return ALLOCATION_ERROR;
    Unode* iter = array[companyID-1];
    while(iter!=nullptr)
    {
        *standing += iter->getValue();
        iter = iter->getParent();
    }
    printf("CompanyValue: %.1f\n", *standing);
    delete standing;
    return SUCCESS;
}

void Quit(void** DS)
{
    if(!DS || *DS==nullptr)
        return;
    System *system=(System*) *DS;
    delete system;
    *DS=nullptr;
    return;
}

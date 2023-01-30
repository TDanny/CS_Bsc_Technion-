#include <iostream>
#include "system.h"
using namespace std;


// HELPER FUNCTIONS:

// FUNCTION THAT ADJUSTS THE COMP ID OF EVERY EMPLOYEE IN THE TREE
void adjustCompanyID(Tnode<int,Employee>* node, int target, int new_comp);


// IMPLEMEMTATIONS:

void adjustCompanyID(Tnode<int,Employee>* node, int target, int new_comp)
{
    if(!node)
        return;
    if((node->getData())->getCompanyID()==target)
        (node->getData())->setCompanyID(new_comp);
    adjustCompanyID(node->getLeft(), target, new_comp);
    adjustCompanyID(node->getRight(), target, new_comp);
}

void *Init()
{
    System *DS=new System();
    return (void*)DS;
}

StatusType AddCompany(void *DS, int CompanyID, int Value)
{
    if(!DS || CompanyID <= 0 || Value <= 0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(!system->getCompanies())
    {
        Tree<int,int>* comps = new Tree<int,int>;
        if(!comps)
            return ALLOCATION_ERROR;
        (system->setCompanies(comps));
        Tree<int,Company>* setComps = new Tree<int,Company>;
        if(!setComps)
        {
            delete comps;
            return ALLOCATION_ERROR;
        }
        (system->setSettledCompanies(setComps));
    }
    StatusType result = (system->getCompanies())->insert(CompanyID,Value);
    return result;
}

StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Salary, int Grade)
{
    if(!DS || CompanyID <= 0 || EmployeeID <= 0 || Salary <= 0 || Grade < 0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(!system->getCompanies())
        return FAILURE;
    if(!system->getIdSortedEmp())
    {
        Tree<int,Employee>* t1 = new Tree<int,Employee>();
        if(!t1)
            return ALLOCATION_ERROR;
        (system->setIdSortedEmp(t1));
        Tree<Employee,Employee>* t2 = new Tree<Employee,Employee>();
        if(!t2)
        {
            delete t1;
            return ALLOCATION_ERROR;
        }
        (system->setSalarySortedEmp(t2));       
    }
    if((system->getIdSortedEmp())->searchAssist(EmployeeID)) // employee exists
        return FAILURE;
    StatusType result = SUCCESS;
    if(!((system->getSettledCompanies())->searchAssist(CompanyID))) 
    {
        // company isnt in settled, so we got to add it
        Tnode<int,int>* companyNode1 = ((system->getCompanies())->searchAssist(CompanyID));
        if(!companyNode1)
            return FAILURE;
        Company *tmpCompany = new Company(CompanyID,*(companyNode1->getData()));
        if(tmpCompany == nullptr)
            return ALLOCATION_ERROR;
        Tree<int,Employee>* idT = new Tree<int,Employee>();
        if(!idT)
        {
            delete tmpCompany;
            return ALLOCATION_ERROR;
        }
        Tree<Employee,Employee>* sT = new Tree<Employee,Employee>();
        if(!sT)
        {
            delete tmpCompany;
            delete idT;
            return ALLOCATION_ERROR;
        }
        result = (system->getSettledCompanies())->insert(CompanyID, *tmpCompany);
        if(result!=SUCCESS)
            return result;
        delete tmpCompany;
        (((system->getSettledCompanies())->searchAssist(CompanyID))->getData())->setIdTree(idT);
        (((system->getSettledCompanies())->searchAssist(CompanyID))->getData())->setSalaryTree(sT);
    }
    Tnode<int,Company>* companyNode = ((system->getSettledCompanies())->searchAssist(CompanyID));
    Company *company = companyNode->getData();
    int* compID = new int(CompanyID);
    if(compID==nullptr)
        return ALLOCATION_ERROR;
    Employee *e = new Employee(EmployeeID,Salary,Grade, compID);
    if(!e)
    {
        delete compID;
        return ALLOCATION_ERROR;
    }
    // add to company trees
    Tree<int, Employee> *t = company->getIdTree();
    result = t->insert(EmployeeID, *e);
    if(result!=SUCCESS)
    {
        delete compID;
        delete e;
        return result;
    }
    Tree<Employee, Employee> *t1 = company->getSalaryTree();
    result = t1->insert(*e, *e);
    if(result!=SUCCESS)
    {
        delete compID;
        delete e;
        return result;
    }    
    // add to system trees
    t = system->getIdSortedEmp();
    result = t->insert(EmployeeID, *e);
    if(result!=SUCCESS)
    {
        delete compID;
        delete e;
        return result;
    }
    t1 = system->getSalarySortedEmp();
    result = t1->insert(*e, *e);
    if(result!=SUCCESS)
        delete compID;
    delete e;  
    return result;
}

StatusType RemoveCompany(void *DS, int CompanyID)
{
    if(!DS || CompanyID<=0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(!system->getCompanies())
        return FAILURE;
    if(!(system->getCompanies())->searchAssist(CompanyID))//checks if the company exist
        return FAILURE;
    if((system->getSettledCompanies())->searchAssist(CompanyID)) // if has workers
        return FAILURE;
    return (system->getCompanies())->remove(CompanyID);
}
// we looking into active companies tree - MAX OF N COMPANIES THERE
StatusType RemoveEmployee(void *DS, int EmployeeID)
{
    if(!DS || EmployeeID <= 0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(!system->getIdSortedEmp())
        return FAILURE;
    if(!(system->getIdSortedEmp())->searchAssist(EmployeeID)) // employee exists
        return FAILURE;
    // first remove from id tree of system
    StatusType result = SUCCESS;
    Tree<int, Employee> *t = system->getIdSortedEmp();
    Employee *emp = new Employee((t->searchAssist(EmployeeID))->getDataV());
    if(emp==nullptr)
        return ALLOCATION_ERROR;
    result = t->remove(EmployeeID);
    if(result!=SUCCESS)
    {
        delete emp;
        return result;
    }
    // now remove from salary tree
    Tree<Employee, Employee> *t1=system->getSalarySortedEmp();
    result = t1->remove(*emp);
    if(result!=SUCCESS)
    {
        delete emp;
        return result;
    }
    // now we will find the company in active companies and we will remove from it
    Company *comp = (system->getSettledCompanies())->searchAssist(emp->getCompanyID())->getData();
    t=comp->getIdTree();
    result = t->remove(emp->getId());
    if(result!=SUCCESS)
    {
        delete emp;
        return result;
    }    
    t1=comp->getSalaryTree();
    result = t1->remove(*emp);
    if(result!=SUCCESS)
    {
        delete emp;
        return result;
    }
    if(t->getCounter() == 0) {
        result = (system->getSettledCompanies())->remove(emp->getCompanyID());
        if(result!=SUCCESS)
        {
            delete emp;
            return result;
        }
    }
    emp->deleteCompId();
    delete emp;
    return result;
}

StatusType GetCompanyInfo(void *DS, int CompanyID, int *Value, int *NumEmployees)
{
    if(!DS || CompanyID <= 0 || !Value || !NumEmployees)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(!system->getCompanies())
        return FAILURE;
    if(!(system->getCompanies())->searchAssist(CompanyID))
        return FAILURE;
    *Value = ((system->getCompanies())->searchAssist(CompanyID))->getDataV();
    Tnode<int,Company>* tmp = (system->getSettledCompanies()->searchAssist(CompanyID));
    if(!tmp)
        *NumEmployees=0;
    else
    {
        // first get the company, then the id tree, then the amount of employees
        *NumEmployees=((tmp->getData())->getIdTree())->getCounter();
    }
    return SUCCESS;
}
StatusType GetEmployeeInfo(void *DS, int EmployeeID, int *EmployerID, int *Salary, int *Grade)
{
    if(!DS || !EmployerID || !Salary || !Grade || EmployeeID<=0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(!system->getIdSortedEmp())
        return FAILURE;
    Tnode<int,Employee>* Employee = (system->getIdSortedEmp())->searchAssist(EmployeeID);
    if(!Employee) // employee doesnt exists
        return FAILURE;
    *EmployerID = (Employee->getData())->getCompanyID();
    *Salary = (Employee->getData())->getSalary();
    *Grade = (Employee->getData())->getGrade();
    return SUCCESS;
}
StatusType IncreaseCompanyValue(void *DS, int CompanyID, int ValueIncrease) 
{
    if(!DS || CompanyID <= 0 || ValueIncrease <= 0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(!system->getCompanies())
        return FAILURE;
    Tnode<int,int>* tmp = (system->getCompanies())->searchAssist(CompanyID);
    if(!tmp)  //checks if company exist
        return FAILURE;
    int new_value= ValueIncrease + (tmp->getDataV());
    tmp->setDataV(new_value);
    Tnode<int, Company>* comp = (system->getSettledCompanies())->searchAssist(CompanyID);
    if(comp)
    {
        (comp->getData())->setValue(new_value);
    } 
    return SUCCESS;
}

// settled companies has max of N nodes - as much as employees in the system
StatusType PromoteEmployee(void *DS, int EmployeeID, int SalaryIncrease, int BumpGrade)
{
    if(!DS || EmployeeID <= 0 || SalaryIncrease <= 0)
        return INVALID_INPUT;
    System *system = (System*) DS;
    if(!system->getIdSortedEmp())
        return FAILURE;
    Tnode<int,Employee>* emp = (system->getIdSortedEmp())->searchAssist(EmployeeID);
    if(!emp) // employee doesnt exists
        return FAILURE;
    int grade = (emp->getData())->getGrade();
    Employee* tmpEmp = new Employee(emp->getDataV());
    if(!tmpEmp)
        return ALLOCATION_ERROR;
    if(BumpGrade > 0)
        grade++;
    // update ID sorted TREE
    (emp->getData())->setGrade(grade);
    (emp->getData())->increaseSalary(SalaryIncrease);
    // update SALARY sorted TREE
    StatusType result = (system->getSalarySortedEmp())->remove(*tmpEmp);
    if(result!=SUCCESS)
    {
        delete tmpEmp;
        return result;
    }
    result = (system->getSalarySortedEmp())->insert(emp->getDataV(), emp->getDataV());
    if(result!=SUCCESS)
    {
        delete tmpEmp;
        return result;
    }

    // update his company TREES
    Tnode<int, Company>* comp = (system->getSettledCompanies())->searchAssist(((emp->getData())->getCompanyID()));
    // update SALARY sorted TREE
    Tree<Employee,Employee>* t  = (comp->getData())->getSalaryTree();
    result = t->remove(*tmpEmp);
    if(result!=SUCCESS)
    {
        delete tmpEmp;
        return result;
    }
    result = t->insert(emp->getDataV(), emp->getDataV());
    if(result!=SUCCESS)
    {
        delete tmpEmp;
        return result;
    }
    // update ID sorted TREE
    Tree<int,Employee>* t1 = (comp->getData())->getIdTree();
    (t1->searchAssist(EmployeeID)->getData())->setGrade(grade);
    (t1->searchAssist(EmployeeID)->getData())->increaseSalary(SalaryIncrease);
    delete tmpEmp;
    return result;
}
StatusType HireEmployee(void *DS, int EmployeeID, int NewCompanyID)
{
    if(!DS || EmployeeID<=0 || NewCompanyID<=0)
        return INVALID_INPUT;
    System *system=(System*)DS;
    if(!system->getCompanies())
        return FAILURE;
    if(!system->getIdSortedEmp())
        return FAILURE;
    Tnode<int,Employee>* employee = (system->getIdSortedEmp())->searchAssist(EmployeeID);
    Tnode<int,int>* new_company = (system->getCompanies())->searchAssist(NewCompanyID);
    if(!employee || !new_company) // employee doesnt exists or new company doesnt exist
        return FAILURE;
    if((employee->getData())->getCompanyID()==NewCompanyID) //check if employee already in the new company
        return FAILURE;
    int salary = (employee->getData())->getSalary();
    int grade = (employee->getData())->getGrade();
    StatusType result = SUCCESS;
    result = RemoveEmployee(DS , EmployeeID);
    if(result != SUCCESS)
        return result;
    result = AddEmployee(DS, EmployeeID , NewCompanyID , salary,grade);
    return result;
}
StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor)
{
    if(!DS || AcquirerID <= 0 || TargetID <= 0 || TargetID == AcquirerID || Factor < 1.00)
        return INVALID_INPUT;
    System *system=(System*)DS;
    if(!system->getCompanies())
        return FAILURE;
    Tnode<int,int>* AComp = (system->getCompanies())->searchAssist(AcquirerID);
    Tnode<int,int>* TComp = (system->getCompanies())->searchAssist(TargetID);
    if(!AComp || !TComp)
        return FAILURE;
    if(AComp->getDataV() < 10*(TComp->getDataV()))
        return FAILURE;
    int new_value =(int)((AComp->getDataV() + TComp->getDataV())*Factor);
    StatusType result = SUCCESS;
    Tnode<int,Company>* AcqComp = (system->getSettledCompanies())->searchAssist(AcquirerID);
    Tnode<int, Company>* TgtComp = (system->getSettledCompanies())->searchAssist(TargetID);
    AComp->setDataV(new_value);
    // now we have to adjust in settledCompanies
    if(TgtComp == nullptr) // target comp has no workers
    {
        if(AcqComp!=nullptr)
        {
            (AcqComp->getData())->setValue(new_value);
        }
    }
    else if(AcqComp==nullptr) // acquirer comp has no workers
    {
        Company *acq = new Company(AcquirerID, new_value);
        if(!acq)
            return ALLOCATION_ERROR;
        Tree<int,Employee>* idT = new Tree<int,Employee>;
        if(!idT)
        {
            delete acq;
            return ALLOCATION_ERROR;
        }
        Tree<Employee,Employee>* sT = new Tree<Employee,Employee>;
        if(!sT)
        {
            delete acq;
            delete idT;
            return ALLOCATION_ERROR;            
        }
        result = ((system->getSettledCompanies())->insert(AcquirerID,*acq));
        if(result!=SUCCESS)
        {
            delete acq;
            delete idT;
            delete sT;
            return result;            
        }
        Company *acqC = ((((system->getSettledCompanies())->searchAssist(AcquirerID))->getData()));
        acqC->setIdTree(idT);
        acqC->setSalaryTree(sT);
        result = acqC->mergeCompanies((TgtComp->getData()), new_value);
        if(result!=SUCCESS)
        {
            delete acq;
            delete idT;
            delete sT;
            return result;            
        }       
        adjustCompanyID((acqC->getIdTree())->getRoot(), TargetID, AcquirerID);
        delete acq; 

        // compID in employee is pointer, so once we changed it for one tree, it will change for all the others
    }
    else
    {
        result = (AcqComp->getData())->mergeCompanies((TgtComp->getData()), new_value);
        if(result!=SUCCESS)
            return result;
        adjustCompanyID(((AcqComp->getData())->getIdTree())->getRoot(), TargetID, AcquirerID);
    }
    if(TgtComp)
        result = (system->getSettledCompanies())->remove(TargetID);
    if(result!=SUCCESS)
        return result;
    return (system->getCompanies())->remove(TargetID);
}
StatusType GetHighestEarner(void *DS, int CompanyID, int *EmployeeID)
{
    if( !DS || CompanyID==0 || !EmployeeID)
        return INVALID_INPUT;
    System *system=(System*)DS;
    if(!system->getCompanies())
        return FAILURE;
    if(!system->getIdSortedEmp() || (system->getIdSortedEmp())->getCounter() == 0)
        return FAILURE;
    if(CompanyID>0)
    {
        Tnode<int,int>* company_exist = (system->getCompanies())->searchAssist(CompanyID);
        Tnode<int,Company>* company = (system->getSettledCompanies())->searchAssist(CompanyID);
        if(!company_exist || !company)
            return FAILURE;
        *EmployeeID = (((company->getData())->getSalaryTree())->getMaxNode())->getId();
        return SUCCESS;
    }
    else // CompanyID<0
    {
        Tree<Employee,Employee>* all_workers = system->getSalarySortedEmp();
        if(!all_workers)
            return FAILURE;
        *EmployeeID = (all_workers->getMaxNode())->getId();
    }
    return SUCCESS;
}
// our inorder - from low to high, when salaries are equal then from high to low
// reverse inorder - from high to low, when salaries are equal then from low to high
// = goes down and IDs goes up
// then we got to put it in reverse inorder
StatusType GetAllEmployeesBySalary(void *DS, int CompanyID, int **Employees, int *NumOfEmployees)
{
    if(!DS || !Employees || !NumOfEmployees || CompanyID==0)
        return INVALID_INPUT;
    System *system=(System*)DS;
    if(!system->getCompanies())
        return FAILURE;
    if(!system->getIdSortedEmp() || (system->getIdSortedEmp())->getCounter() == 0)
        return FAILURE;
    int len=0;
    Employee **arr;
    StatusType result = SUCCESS;
    if(CompanyID > 0)
    {
        Tnode<int,int>* company_exist = (system->getCompanies())->searchAssist(CompanyID);
        Tnode<int,Company>* company = (system->getSettledCompanies())->searchAssist(CompanyID);
        if(!company_exist || !company)
            return FAILURE;
        Tree<Employee,Employee>* t = (company->getData())->getSalaryTree();
        if(!t)
            return FAILURE;
        len=t->getCounter();
        arr = new Employee*[len];
        if(!arr)
            return ALLOCATION_ERROR;
        result = t->inOrderAssist(&arr, nullptr, len);
        if(result!=SUCCESS)
        {
            delete[] arr;
            return result;
        }
    }
    else // companyid < 0
    {
        Tree<Employee,Employee>* t = system->getSalarySortedEmp();
        if(!t)
            return FAILURE;
        len=t->getCounter();
        arr = new Employee*[len];
        if(!arr)
            return ALLOCATION_ERROR;
        result = t->inOrderAssist(&arr, nullptr, len);
        if(result!=SUCCESS)
        {
            delete[] arr;
            return result;
        }
    }
    int *emps = (int*)malloc(sizeof(int)*len);
    if(!emps)
    {
        delete[] arr;
        return ALLOCATION_ERROR;
    }
    Employee** tmpArr =arr;
    for(int i=0;i<len;i++)
    {
        emps[i]=(*arr)->getId();
        arr++;
    }
    arr=tmpArr;
    delete[] arr;
    int tmp, start=0, end=len-1;
    while(start<end)
    {
        tmp=emps[start];
        emps[start]=emps[end];
        emps[end]=tmp;
        start++;
        end--;
    }
    *Employees = emps;
    *NumOfEmployees = len;
    return SUCCESS;
}
// lowest companies with workers
StatusType GetHighestEarnerInEachCompany(void *DS, int NumOfCompanies, int **Employees)
{
    if(!DS || !Employees || NumOfCompanies < 1)
        return INVALID_INPUT;
    System *system=(System*)DS;
    if(!(system->getSettledCompanies()))
        return FAILURE;
    if((system->getSettledCompanies())->getCounter() < NumOfCompanies)
        return FAILURE;
    Tree<int,Company>* t =  system->getSettledCompanies();
    StatusType result = SUCCESS;
    int **compsIDs=new int*[NumOfCompanies];
    if(!compsIDs)
        return ALLOCATION_ERROR;
    Company **comps = new Company*[NumOfCompanies];
    if(!comps)
    {
        delete[] compsIDs;
        return ALLOCATION_ERROR;
    }
    result = t->inOrderAssist(&compsIDs, &comps,NumOfCompanies);
    if(result!=SUCCESS)
    {
        delete[] compsIDs;
        delete[] comps;
        return result;        
    }
    for(int i=0;i<NumOfCompanies;i++)
        compsIDs[i]=nullptr;
    delete[] compsIDs;
    int *emps =(int*)malloc(sizeof(int)*NumOfCompanies);
    if(!emps)
    {
        delete[] comps;
        return ALLOCATION_ERROR;
    }
    Employee *tmpEmp;
    for(int i=0;i<NumOfCompanies;i++)
    {
        tmpEmp=(comps[i]->getSalaryTree())->getMaxNode();
        emps[i]=tmpEmp->getId();
        comps[i]=nullptr;
    }
    delete[] comps;
    *Employees=emps;
    return SUCCESS;
}
StatusType GetNumEmployeesMatching(void *DS, int CompanyID, int MinEmployeeID, int MaxEmployeeId,
        int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees)
{
    if(!DS || !TotalNumOfEmployees || !NumOfEmployees || CompanyID == 0 || MinEmployeeID < 0
                        || MaxEmployeeId < 0 || MinSalary < 0 || MaxEmployeeId < 0
                        || MinGrade < 0 || MinEmployeeID > MaxEmployeeId)
    {
        return INVALID_INPUT;
    }
    System *system=(System*)DS;
    if(!(system->getCompanies()))
        return FAILURE;
    if(!system->getIdSortedEmp()  || (system->getIdSortedEmp())->getCounter() == 0)
        return FAILURE;
    Employee **emps;
    int numEmployees=0;
    StatusType result = SUCCESS;
    if(CompanyID > 0) {
        Tnode<int,int>* company_exist = (system->getCompanies())->searchAssist(CompanyID);
        Tnode<int,Company>* company = (system->getSettledCompanies())->searchAssist(CompanyID);
        if(!company_exist || !company) // doesnt exist
            return FAILURE;
        // first we find totalNum
        *TotalNumOfEmployees = ((company->getData())->getIdTree())->countCondNodesAss(MinEmployeeID,MaxEmployeeId);
        if(*TotalNumOfEmployees==0)
        {
            *NumOfEmployees=0;
            return SUCCESS;
        }
        emps = new Employee*[*TotalNumOfEmployees];
        if(!emps)
            return ALLOCATION_ERROR;
        result = ((company->getData())->getIdTree())->specialCondNodeAss(&emps, MinEmployeeID,MaxEmployeeId,*TotalNumOfEmployees);
        if(result!=SUCCESS)
        {
            delete[] emps;
            return result;
        }
    }
    else
    {
        Tree<int,Employee>* t = system->getIdSortedEmp();
        if(!t) // no workers in system
            return FAILURE;
        *TotalNumOfEmployees = t->countCondNodesAss(MinEmployeeID,MaxEmployeeId);
        if(*TotalNumOfEmployees==0)
        {
            *NumOfEmployees=0;
            return SUCCESS;
        }
        emps = new Employee*[*TotalNumOfEmployees];
        if(!emps)
            return ALLOCATION_ERROR;
        result = t->specialCondNodeAss(&emps,MinEmployeeID,MaxEmployeeId,*TotalNumOfEmployees);
        if(result!=SUCCESS)
        {
            delete[] emps;
            return result;
        }
    }
    Employee **tmpEmp = emps;
    for(int i=0;i<*TotalNumOfEmployees;i++)
    {
        if((*emps)->getSalary()>=MinSalary && (*emps)->getGrade()>=MinGrade)
            numEmployees++;
        emps++;
    }
    emps=tmpEmp;
    *NumOfEmployees=numEmployees;
    delete[] emps;
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
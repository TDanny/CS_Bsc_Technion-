#include "examDetails.h"

using namespace mtm;
using namespace std;

/**
 * ExamDetails: copy-constructor of exam , copy from old by value
 * 
 * @param old: an exam you want to copy the details from
**/
ExamDetails::ExamDetails(const ExamDetails& old)
{
    allocateAndCopy(*this,old);
}
/**
* ExamDetails: constructor of exam  
* 
* @param course_id: ineger number, non negative, present course id 
* @param month: ineger number, between 1-12, present month number
* @param day: ineger number, between 1-30, present day number
* @param time_exam_starts: double number, non negative, present time in the day exam start
* @param length_of_exam: ineger number, present the lenght of the exam in hours
* @param link_exam: string of link to zoom of the exam, if nothing is send to function, the default is empty string
 **/
ExamDetails::ExamDetails(const int course_id,const int month,const int day,const double time_exam_starts,const int length_of_exam,const string link_exam)
{
    if(course_id<=0)
    {
        throw InvaildArgsException();
    }
    if(month<=0 || month>MONTH_PER_YEAR)
    {
        throw InvalidDateException();
    }
    if(day<1 || day>DAY_PER_MONTH)
    {
        throw InvalidDateException();
    }
    if(length_of_exam<0)
    {
        throw InvalidTimeException();
    }
    if(time_exam_starts>MAX_HOUR || time_exam_starts<0 || std::fmod(time_exam_starts,HALF)!=0)
    {
        throw InvalidTimeException();
    }
    this->course_id = course_id;
    this->month = month;
    this->day = day;
    this->time_exam_starts = time_exam_starts;
    this->length_of_exam = length_of_exam;
    this->link_exam = link_exam;

}
/**
*  ~ExamDetails: distructor of exam  
**/
ExamDetails::~ExamDetails()
{
}
/**
* operator=: copy from "old" to *this exam by value
* 
* @param old: an exam you want to copy the details from
* @return:
*          *this - the exam you copy to, possible to concatenate
**/
ExamDetails& ExamDetails::operator=(const ExamDetails& old)
{
    allocateAndCopy(*this,old);
    return *this;
}
/**
 * getLink: get link from specific exam
 * 
 * @return:
 *          string - present the link of *this exam
 **/
string ExamDetails::getLink() const
{
    return link_exam;
}
/**
 * setLink: set link to specific exam
 * @param link: string that present the link you want to enter to *this exam
 **/
void ExamDetails::setLink(const string link)
{
    if(link_exam == link)
    {
        return;
    }
    string update_old_link = link;
    link_exam = update_old_link;

}
/**
 * allocateAndCopy: get 2 exam and delete the data from frist one and copy all the day by value from second exam  
 * 
 * @param exam1: exam you want to copy to
 * @param exam2: exam you want to copy from
 **/
void ExamDetails::allocateAndCopy(ExamDetails& exam1,const ExamDetails& exam2)
{
    if(&exam1 == &exam2)
    {
        return;
    }
    string new_link = exam2.link_exam;
    
    exam1.link_exam = new_link;
    exam1.course_id = exam2.course_id;
    month = exam2.month;
    day = exam2.day;
    time_exam_starts = exam2.time_exam_starts;
    length_of_exam = exam2.length_of_exam;

    return;
}
/**
 * operator-: return the difference between left exam from right by days
 * 
 * @param exam1: an exam you want to check the difference with
 * @return:
 *          Positive number if left happend after the right exam
 *          Negative number if right happend after the left exam
 **/
int ExamDetails::operator-(const ExamDetails exam1) const
{
    if(this->day == exam1.day)
    {
        return DAY_PER_MONTH*(this->month-exam1.month);
    }
    
    return DAY_PER_MONTH*(this->month - exam1.month)+(this->day - exam1.day);
}
/**
 * operator<: return if left exam happend before right exam
 * 
 * @param exam1: an exam you want to check the difference with
 * @return:
 *          true - if left exam happend before the right exam
 *          false - if right exam happend before the left exam
 **/
bool ExamDetails::operator<(const ExamDetails exam1) const
{
    if((*this - exam1) < 0 )
    {
        return true;
    }

    return false;
}
/**
 * operator<<: print operator that print like instruction on *.pdf
 * 
 * @param os: reference to ostream operator
 * @param exam:exam you want to print
 * @return:
 *          ostream - reference to the print operator
 **/
namespace mtm {
    ostream&  operator<<(ostream& os,const ExamDetails& exam)
    {
        string text1 = "Course Number: ";
        string text2 = "Time: ";
        string text3 = "Duration: ";
        string text4 = "Zoom Link: ";

    
        //In case the exam doesn't start at :00
        if(fmod(exam.time_exam_starts,1)!=0)
        {
            return os << text1 << exam.course_id << std::endl << text2 << exam.day << "." << exam.month << " at " << (int)exam.time_exam_starts \
            << ":30" << std::endl << text3 << exam.length_of_exam << ":00" << std::endl << text4 << exam.link_exam << std::endl ;

        }
        //In case the exam does start at :00
        return os << text1 << exam.course_id << std::endl << text2 << exam.day << "." << exam.month << " at " << (int)exam.time_exam_starts \
            << ":00" << std::endl << text3 << exam.length_of_exam << ":00" << std::endl << text4 << exam.link_exam << std::endl ;

    }
}


/**
 * makeMatamExam: create a specific default exam, like instruction on *.pdf
 * 
 * @return:
 *          ExamDetails - by value like present in *.pdf
 **/
ExamDetails ExamDetails::makeMatamExam()
{
   
    ExamDetails exam(MTM_ID,MTM_MONTH_OF_EXAM,MTM_DAY_OF_EXAM,MTM_TIME_EXAM_START,MTM_DURATION,MTM_LINK_EXAM);
    return exam;
    
}

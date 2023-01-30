#ifndef EXAMDETAILS_H_
#define EXAMDETAILS_H_
#include <iostream>
#include <cmath>
#include <cstring>
#include <stdlib.h>


/* -----------Defines-------------*/
#define HALF 0.5
#define DAY_PER_MONTH 30
#define MONTH_PER_YEAR 12
#define MAX_HOUR 23.5
#define MTM_ID 234124
#define MTM_DAY_OF_EXAM 28
#define MTM_MONTH_OF_EXAM 7
#define MTM_DURATION 3
#define MTM_TIME_EXAM_START 13
#define MTM_LINK_EXAM "https://tinyurl.com/59hzps6m"

/**
 * ExamDetails: class present on exam with structor from *.pdf
 **/

namespace mtm
{
    class ExamDetails 
    {
        private:
        int course_id , month , day , length_of_exam;
        double time_exam_starts;
        std::string link_exam;
        /**
         * allocateAndCopy: get 2 exam and delete the data from frist one and copy all the day by value from second exam  
         * 
         * @param exam1: exam you want to copy to
         * @param exam2: exam you want to copy from
         **/
        void allocateAndCopy(ExamDetails& exam1,const ExamDetails& exam2);
        public:
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
        ExamDetails(const int course_id,const int month,const int day,const double time_exam_starts,const int length_of_exam,const std::string link_exam="");
        /**
         * ExamDetails: copy-constructor of exam , copy from old by value
         * 
         * @param old: an exam you want to copy the details from
         **/
        ExamDetails (const ExamDetails& old);
        /**
         *  ~ExamDetails: distructor of exam  
         **/
        ~ExamDetails();
        
        /**
         * operator=: copy from "old" to *this exam by value
         * 
         * @param old: an exam you want to copy the details from
         * @return:
         *          *this - the exam you copy to, possible to concatenate
         **/
        ExamDetails& operator=(const ExamDetails& old);
        /**
         * operator-: return the difference between left exam from right by days
         * 
         * @param exam1: an exam you want to check the difference with
         * @return:
         *          Positive number if left happend after the right exam
         *          Negative number if right happend after the left exam
         **/
        int operator-(const ExamDetails exam1) const;
        /**
         * operator<: return if left exam happend before right exam
         * 
         * @param exam1: an exam you want to check the difference with
         * @return:
         *          true - if left exam happend before the right exam
         *          false - if right exam happend before the left exam
         **/
        bool operator<(const ExamDetails exam1) const;
        /**
         * operator<<: print operator that print like instruction on *.pdf
         * 
         * @param os: reference to ostream operator
         * @param exam:exam you want to print
         * @return:
         *          ostream - reference to the print operator
         **/
        friend std::ostream &operator<<(std::ostream& os, const ExamDetails& exam);
        /**
         * getLink: get link from specific exam
         * 
         * @return:
         *          string - present the link of *this exam
         **/
        std::string getLink() const;
        /**
         * setLink: set link to specific exam
         * @param link: string that present the link you want to enter to *this exam
         **/
        void setLink(const std::string link);
        /**
         * makeMatamExam: create a specific default exam, like instruction on *.pdf
         * 
         * @return:
         *          ExamDetails - by value like present in *.pdf
         **/
        static ExamDetails makeMatamExam();
        /**
         * InvalidDateException: class present exeption of invalid date, exception throw in term that present in *.pdf
         **/
        class InvalidDateException{};
        /**
         * InvalidTimeException: class present exeption of invalid time, exception throw in term that present in *.pdf
         **/
        class InvalidTimeException{};
         /**
         * InvaildArgsException: class present exeption of invalid args, exception throw in term that present in *.pdf
         **/
        class InvaildArgsException{};
        
    };
}

#endif //_EXAMDETAILS_H

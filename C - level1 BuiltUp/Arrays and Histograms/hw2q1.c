/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFUALT '@'

/*=========================================================================
  Constants and definitions:
==========================================================================*/

/* put your #defines and typedefs here*/
bool checkThreeOpetions(int counter,int sum,bool monotonic_up,bool r_letter_shown);
bool checkTwoOptions(int counter,int sum , bool monotonic_up , bool r_letter_shown);
void oneOptionRobber(int counter,int sum , bool monotonic_up , bool r_letter_shown);
void whoIsTheRobber(int counter , int sum , bool monotonic_up , bool r_letter_shown);
int print_welcome_message();
int print_Walter_White_sent();
int print_Daenerys_sent();
int print_Doron_Kabilio_sent();
int print_no_player();

/*-------------------------------------------------------------------------
The program scans a message from the professor  , analyze the message and
and determine is a robber will send inside the help the others and who it
will be:Daenerys Targaryen, Doron Kabilio or Walter_White.
 -------------------------------------------------------------------------*/
int main()
{
    print_welcome_message();
    char letter , temp = DEFUALT;
    bool monotonic_up = true, r_letter_shown = false ;
    int counter=0 , sum=0;
    while((letter=getchar())!='!')
    {
        if(letter=='r')
        {
            r_letter_shown = true;
        }
        if((temp >= letter) && (counter))
        {
            monotonic_up = false;
        }

        counter++;
        sum+=letter;
        temp = letter;
    }
    whoIsTheRobber(counter , sum , monotonic_up , r_letter_shown);
  return 0;
}


/* Function responsable to start the check of each option,
I did it to prevent big main function.
*/
void whoIsTheRobber(int counter , int sum , bool monotonic_up , bool r_letter_shown)
{
    bool sent=false;
    sent=checkThreeOpetions(counter,sum,monotonic_up,r_letter_shown);
    if(!sent)
        sent=checkTwoOptions(counter,sum,monotonic_up,r_letter_shown);
    if(!sent)
        oneOptionRobber(counter,sum,monotonic_up,r_letter_shown);
}


/*check if there 3 options for potentional robbers.
The function is boolian so the information will help in other function.
*/
bool checkThreeOpetions(int counter,int sum,bool monotonic_up,bool r_letter_shown)
{
    if(monotonic_up&&(sum%5!=0||counter>4)&&counter<6&&r_letter_shown==false)
    {
        print_Daenerys_sent();
        return true;
    }
    return false;
}

//check if there 2 options for potentional robbers.
//The function is boolian so the information will help in other function.
bool checkTwoOptions(int counter,int sum , bool monotonic_up , bool r_letter_shown)
{
    if(monotonic_up && counter<6 && r_letter_shown==false)
    {
        print_Daenerys_sent();
        return true;
    }
    if(monotonic_up && (sum%5!=0 || counter>4))
    {
        print_Daenerys_sent();
        return true;
    }
    if(counter<6 && r_letter_shown==false && (sum%5!=0 || counter>4))
    {
        print_Doron_Kabilio_sent();
        return true;
    }
    return false;
}


/*The function will run only if the 2/3 option are false.
the function will decide if there is the robber that the professor wants
to send inside OR even no robber will send inside.
*/
void oneOptionRobber(int counter,int sum , bool monotonic_up , bool r_letter_shown)
{
    if(monotonic_up)
    {

        print_Daenerys_sent();
        return;
    }

    else if(counter<6 && r_letter_shown==false)
    {
        print_Doron_Kabilio_sent();
        return;
    }

    else if(sum%5!=0 || counter>4)
    {
        print_Walter_White_sent();
        return;
    }

    else
    {
        print_no_player();
        return;
    }
}

/* print Functions */


int print_welcome_message(){
    return printf("La Inspectora Murillo, Please enter El Professor name:\n");
}

int print_Walter_White_sent(){
    return printf("\nEl Professor sent Walter White into the robbery!\n");
}

int print_Daenerys_sent(){
    return printf("\nEl Professor sent Daenerys Targaryen into the robbery!\n");
}

int print_Doron_Kabilio_sent(){
    return printf("\nEl Professor sent Doron Kabilio into the robbery!\n");
}

int print_no_player(){
    return printf("\nNO player has been sent into the robbery!\n");
}

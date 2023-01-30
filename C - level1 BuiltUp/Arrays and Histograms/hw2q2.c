/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



/*=========================================================================
  Constants and definitions:
==========================================================================*/

/* put your #defines and typedefs here*/

#define PRACTITIONERS 13
#define HIGHEST_NUM_VOTE 13
#define LOWEST_NUM_VOTE 1
#define END_OF_VOTES -1
#define DEFUALT 0

void processData(int hist[PRACTITIONERS]);
void print_welcome_message();
void print_welcome_message2();
void print_introduce_histogram();
void printHist(int hist[], int max, char symbol);
void checkWhoIsWinner(int hist[], char symbol);
void printWinnerName(int winner);
/*-------------------------------------------------------------------------
  The program will get the votes for the new Technion president, arrange
  it in histogram , analyze the numbers of votes,
  publish the winner's name and print the vote histogram.
 -------------------------------------------------------------------------*/
int main()
{
    print_welcome_message();
    char symbol;
    scanf("%c",&symbol);
    int histogram[PRACTITIONERS]={0};
    processData(histogram);
    checkWhoIsWinner(histogram , symbol);



  return 0;
}


/*The function will process the votes and arrange it in histogram.
*/
void processData(int hist[PRACTITIONERS])
{
    print_welcome_message2();
    int vote;
    while(vote!=END_OF_VOTES)
    {
        scanf(" %d",&vote);  //CHECK % D
        if(vote==END_OF_VOTES)
        {
            return;
        }
        if(vote<LOWEST_NUM_VOTE || vote>HIGHEST_NUM_VOTE)
        {
            printf("not a legal vote , please vote again!");
            continue;
        }
        hist[vote-1]++;
    }
}


/* Function will check who is the winner by counting who has the most
votes.
*/
void checkWhoIsWinner(int hist[], char symbol)
{
    int max=DEFUALT , winner_index=1;
    for(int i=0 ; i<PRACTITIONERS ; i++)
    {
        if(hist[i]>max)
        {
            winner_index = i+1;
            max = hist[i];
        }
    }
    print_introduce_histogram();
    printHist(hist , max , symbol);
    printWinnerName(winner_index);
}


/*Function will print winner's name.
*/
void printWinnerName(int winner)
{
    switch(winner)
    {
        case 1:
            printf("Adi was elected!\n");
            break;
        case 2:
            printf("Alon was elected!\n");
            break;
        case 3:
            printf("Assaf was elected!\n");
            break;
        case 4:
            printf("Daniella was elected!\n");
            break;
        case 5:
            printf("Danielle was elected!\n");
            break;
        case 6:
            printf("Dmitry was elected!\n");
            break;
        case 7:
            printf("Gasob was elected!\n");
            break;
        case 8:
            printf("Ido was elected!\n");
            break;
        case 9:
            printf("Najeeb was elected!\n");
            break;
        case 10:
            printf("Nir was elected!\n");
            break;
        case 11:
            printf("Omer was elected!\n");
            break;
        case 12:
            printf("Yair was elected!\n");
            break;
        case 13:
            printf("Yara was elected!\n");
            break;
    }
}


/* Function will print the histogram.
*/
void printHist(int hist[], int max, char symbol)
{
    for(int i=max ; i>0 ; i--)
    {
        for(int j=0 ; j<PRACTITIONERS ; j++)
        {
            if(hist[j]==max)
            {
                printf("%c", symbol);
                hist[j]--;
                if(j!=PRACTITIONERS && j<9)
                    printf(" ");
                if(j!=PRACTITIONERS && j>=9)
                    printf("  ");
            }

            else
            {
                if(j!=PRACTITIONERS && j<9)
                    printf("  ");
                else if(j!=PRACTITIONERS && j>=9)
                    printf("   ");
            }
        }
        max--;
        printf("\n");
    }
    printf("1 2 3 4 5 6 7 8 9 10 11 12 13 \n");
}

void print_welcome_message(){
	printf("Please enter a character:\n");
}

void print_welcome_message2(){
	printf("Please enter votes:\n");
}

void print_introduce_histogram(){
	printf("Histogram:\n");
}









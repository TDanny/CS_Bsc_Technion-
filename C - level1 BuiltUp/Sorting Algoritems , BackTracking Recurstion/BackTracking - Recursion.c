#include <stdio.h>
#include <stdbool.h>

#define N 5
#define MINIMUM 60

bool scan_1d_arr(int arr[], int n);
bool scan_2d_bool_arr(bool arr[][N], int nrows);
bool scan_input(int mandates[], bool can_collaborate[][N]);
int count_coalitions(int mandates[], bool can_collaborate[][N], bool chosen[N], int index ,int sum );
bool check_colobration(bool can_collaborate[][N] ,int index , bool chosen[N]);


int main()
{
    int mandates[N]={0};
    bool can_collaborate[N][N] = {{0}};
    bool chosen[N]={0};
    int index=0 , sum=0;
    if (!scan_input(mandates, can_collaborate))
    {
        printf("Wrong input");
        return 0;
    }
    int coalition_num = count_coalitions(mandates, can_collaborate , chosen ,index, sum );
    printf("Number of coalitions: %d\n", coalition_num);
    return 0;
}



int count_coalitions(int mandates[], bool can_collaborate[][N] , bool chosen[N], int index ,int sum )
{
    if(index==N && sum<=MINIMUM)
        return 0;
    else if(index==N && sum>MINIMUM)
        return 1;

    int combinations=0;
    chosen[index]=true;
    if(check_colobration(can_collaborate , index , chosen))
        combinations+= count_coalitions(mandates, can_collaborate , chosen ,index+1, sum+mandates[index]);
    chosen[index]=false;
    combinations+=count_coalitions(mandates, can_collaborate , chosen ,index+1, sum);

    return combinations;
}

bool check_colobration(bool can_collaborate[][N] ,int index , bool chosen[N])
{

    for(int group=0 ;group<index ; group++)
    {
        if(chosen[group]==false)
            continue;
        if(can_collaborate[index][group]==false)
        {
            return false;
        }
    }
    return true;
}

bool scan_1d_arr(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", arr+i)!=1 || arr[i] < 0)
        {
            return false;
        }
    }
    return true;
}

bool scan_2d_bool_arr(bool arr[][N], int nrows)
{
    int x = 0;
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (scanf("%d", &x) != 1)
            {
                return false;
            }
            arr[i][j] = x; // convert to bool
        }
    }
    return true;
}

bool scan_input(int mandates[], bool can_collaborate[][N])
{
    printf("Enter mandates:\n");
    if (!scan_1d_arr(mandates, N))
    {
        return false;
    }
    printf("Enter possible collaborations:\n");
    return scan_2d_bool_arr(can_collaborate, N);
}


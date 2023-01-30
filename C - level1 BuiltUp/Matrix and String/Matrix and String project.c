/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BIGGEST_SCAN 258
#define WHITE 0
#define BLACK 255
#define MAX 16
#define NEIGHBORS 8
#define ITERATIONS 3

/*=========================================================================
  Constants and definitions:
==========================================================================*/

/* put your #defines and typedefs here*/
void print_image(int* image, int n, int m);
void ImageSmoothing(int* center_arr ,int* center_temp,int* image, int row , int column);
int* createArray();
void scanPic(int* single_arr, int* image, int* target);
void print_welcome_message();
void excuteOption(char option,int* image ,int* arr_temp,int row,int column);
void average(int* image , int row , int column, int* arr_temp);
bool checkBoarders(int* pixel , int* image, int row , int column);
void rotate(int* image, int row, int column, int* target, int direction);
void negative(int* image, int row, int column, int* target);
char insertToArrays(int* single_arr , int* image , int* target);
void swap(int *dst, int *src);
void print_target(int* target , int row, int column);
/*-------------------------------------------------------------------------
  The main program. (describe what your program does here)
 -------------------------------------------------------------------------*/
int main()
{
    int* row,column;
    int* one_d_array=malloc(sizeof(int)*print_welcome_message(row,column));
    int* image = createArray(*row,*column);
    int* target = createArray(*row,*column);
    scanPic(one_d_array,image,target);

    free(one_d_array);
    free(image);
    free(target);
  return 0;
}

void scanPic(int* single_arr, int* image, int* target)
{
    int number;
    int* ptr_one_d = single_arr;
    while(scanf("%d",&number)==1)
    {
        *ptr_one_d=number;
        ptr_one_d++;
    }
    scanf("%c",ptr_one_d);
    row = *single_arr;
    column = *(single_arr+1);
    char option = insertToArrays(single_arr,image,target);
    excuteOption(option , image,target,row,column);
}

void print_target(int* target , int row, int column)
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column ; j++)
        {
            if(j!=column-1)
            {
                printf("%d ",target[i*row+j]);
            }
            else
            {
                printf("%d",target[i*row+j]);
                printf("\n");
            }
        }
    }
    printf("\n");
}

char insertToArrays(int* single_arr , int* image , int* target)
{
    int* ptr_single_arr = single_arr;
    int row = *(ptr_single_arr++);
    int column = *(ptr_single_arr++);
    for(int i=0;i<row ; i++)
    {
        for(int j=0;j<column ; j++)
        {
            *(image+(row*i+j))=*ptr_single_arr;
            *(target+(row*i+j))=*ptr_single_arr;
            ptr_single_arr++;
        }
    }
    print_target(image,row,column);
    char option = *ptr_single_arr;
    //printf("option is %c\n",option);
    return option;
}


void swap(int *dst, int *src)
{
    int temp = *dst;
    *dst = *src;
    *src = temp;
}

void excuteOption(char option,int* image ,int* target,int row,int column)
{
    printf("What you would like to do with your photo (L,R,V,N)?\n");
    switch(option)
    {
        case 'L':
            rotate(image,row, column,target,1);
            break;
        case 'R':
            rotate(image , row,column,target,0);
            break;
        case 'V':
            average(image, row , column, target);
            break;
        case 'N':
            negative(image,row,column,target);
            break;
    }
}
void rotate(int* image, int row, int column, int* target, int direction)
{
    /*int i, j;
    if(direction==1){
        printf("The image after left rotation:\n");
        for(j=1 ;j <= row; j++)
        {
            for (i =1; i <= column; i++)
            {
                target[j*(column+2)+i]=image[i*(row+2)+row-j+1];
            }
        }
        print_target(target, row, column);
        return;
    }
    if(direction==0){
        printf("The image after right rotation:\n");
        for(j=1 ;j <= row; j++)
        {
            for (i =1; i <= column; i++)
            {
                target[j*(column+2)+i]=image[(column-i+1)*(row+2)+j];
            }
        }
        print_target(target, row, column);
        return;
    }
}
 */
    if(direction==1)
    {
        for(int i=0 ; i<row ; i++)
        {
            for(int j=0 ; j<column ; j++)
            {
                *(target+(j*column+i)) = *(image+(i*row+j));
            }
        }
        swap(&row,&column);
        for (int i = 0; i < row; i++)
        {
            int start = 0, end = column - 1;
            while (start < end)
            {
                swap(target+(i*row + start),target+(i*row + end));
                start++;
                end--;
            }
        }

        print_target(target,row,column);
    }
    if(direction==0)
    {
        printf("IM HERE\n");
        printf("row = %d ___ column =%d\n",row,column);
        int temp=0;
        for(int i=0 ; i<row ; i++)
        {
            for(int j=0 ; j<column ; j++)
            {
                target[j*row+i]=image[i*row+j];
                //*(target+(j*row+i)) = *(image+(i*row+j));
            }
        }

        swap(&row,&column);
        print_target(target,row,column);
        for (int i = 0; i < row; i++)
        {
            int start = 0, end = column - 1;
            while (start < end)
            {
                swap(target+(i*row + start),target+(i*row + end));
                start++;
                end--;
            }
        }

        for (int i=row-1; i >=0; i--)
        {
            for (int col=0;col<column ;col++)
            {
                *(target+(i*row+col))=*(image+(temp*row+col));
            }
            temp++;
        }

        print_target(target,row,column);
    }
}


void negative(int* image, int row, int column, int* target)
{
    for(int i=0; i<row*column; i++)
    {
        *(target+i) = BLACK-(*(image+i));
    }
    printf("The image after negating:\n");
    print_target(target,row,column);
}

void average(int* image , int row , int column, int* target)
{
    int* pixel = image;
    int* temp_pixel = target;
    for(int i=0; i<row*column; i++)
        ImageSmoothing(pixel+i ,temp_pixel+i, image, row, column);
    printf("The image after averaging:\n");
    print_target(target , row ,column);
}


void ImageSmoothing(int* center_arr ,int* center_temp,int* image, int row , int column)
{
    int sum=0 , counter=0;
    int* target = center_arr-row-1;
    for(int i=0; i<ITERATIONS ; i++)
    {
        for(int j=0 ; j<ITERATIONS ;j++)
        {
            if(!checkBoarders(target+(row*i)+j,image, row,column))
            {
                printf("FASLE = %d\n", *(target+(row*i)+j));
                continue;
            }
            printf("TRUE\n");
            sum+=(*(target+(row*i)+j));
            counter++;
        }
    }
    *center_temp = sum/counter;
    printf("avrage = %d", sum/counter);
}

bool checkBoarders(int* pixel , int* image, int row , int column)
{
    if(*pixel<1 || *pixel>255)
        return false;
    return true;
}




int* createArray(int row, int column)
{
    int* image = (int *)calloc(sizeof(int)*(row+2)*(column+2));
    if(!image)
    {
        exit(0);
    }

    return image;
}

void print_image(int* image, int n, int m)
{
    printf("row = %d ___ column =%d\n",n,m);
    int i,j;
    for(j=0;j < n; j++)
    {
        for (i =0; i < m; i++)
        {
            if (i != m-1)
                printf("%d\t", *( (image+j*m) + i));//image[j][i]
            else
                printf("%d", *( (image+j*m) + i));
        }
        printf("\n");
    }
}
int print_welcome_message(int* row, int* column;)
 {
     printf("Enter the number of rows in your image matrix:\n");
     scanf("%d",row);
     printf("Enter the number of columns in your image matrix:\n");
     scanf("%d",column);
     return (*row+2)*(*column+2);
 }

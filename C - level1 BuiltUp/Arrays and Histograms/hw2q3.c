/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/*=========================================================================
  Constants and definitions:
==========================================================================*/
#define BYTES 256
#define MAX_SIZE 256
/* put your #defines and typedefs here*/

void print_welcome_message1();
void print_invalid_amount();
void print_welcome_message2();
void print_enter_matrix();
void print_encrypted_message();
void print_matrix(int size, int aes[MAX_SIZE][MAX_SIZE]);
void byteSubstitution(int size,int mat[MAX_SIZE][MAX_SIZE] , int sbox[BYTES]);
void shiftRows(int size ,int mat[MAX_SIZE][MAX_SIZE]);
int userNumOfIteration();
int userSizeOfMatrix();
void encryption(int iterations ,int size , int mat[MAX_SIZE][MAX_SIZE], int sbox[BYTES]);
void fillMatrix(int size, int mat[MAX_SIZE][MAX_SIZE]);
/*-------------------------------------------------------------------------
  the program will encrypt a message provided by the user according to AES
  encryption steps. The user's message will be form in matrix fill
  with integers in range of: 0-255.
  The program will print the encrypted message according to user's input.
 -------------------------------------------------------------------------*/
int sbox[BYTES] = { 99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254,
		215, 171, 118, 202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175,
		156, 164, 114, 192, 183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229,
		241, 113, 216, 49, 21, 4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128,
		226, 235, 39, 178, 117, 9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214,
		179, 41, 227, 47, 132, 83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190,
		57, 74, 76, 88, 207, 208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2,
		127, 80, 60, 159, 168, 81, 163, 64, 143, 146, 157, 56, 245, 188, 182,
		218, 33, 16, 255, 243, 210, 205, 12, 19, 236, 95, 151, 68, 23, 196, 167,
		126, 61, 100, 93, 25, 115, 96, 129, 79, 220, 34, 42, 144, 136, 70, 238,
		184, 20, 222, 94, 11, 219, 224, 50, 58, 10, 73, 6, 36, 92, 194, 211,
		172, 98, 145, 149, 228, 121, 231, 200, 55, 109, 141, 213, 78, 169, 108,
		86, 244, 234, 101, 122, 174, 8, 186, 120, 37, 46, 28, 166, 180, 198,
		232, 221, 116, 31, 75, 189, 139, 138, 112, 62, 181, 102, 72, 3, 246, 14,
		97, 53, 87, 185, 134, 193, 29, 158, 225, 248, 152, 17, 105, 217, 142,
		148, 155, 30, 135, 233, 206, 85, 40, 223, 140, 161, 137, 13, 191, 230,
		66, 104, 65, 153, 45, 15, 176, 84, 187, 22 };

int main()
{

    int iterations = userNumOfIteration();
    int mat_size = userSizeOfMatrix();
    print_enter_matrix();
    int matrix[MAX_SIZE][MAX_SIZE];
    fillMatrix(mat_size,matrix);
    encryption(iterations, mat_size , matrix, sbox);
    print_encrypted_message();
    print_matrix(mat_size , matrix);


  return 0;
}


//function change each cell in the matrix with the Sbox's cell-
//that got the index like the value cell in matrix user provided.
void byteSubstitution(int size,int mat[MAX_SIZE][MAX_SIZE] , int sbox[BYTES])
{
    for(int row=0; row<size ; row++)
    {
        for(int column=0 ; column<size ; column++)
        {
           int index = mat[row][column];
           mat[row][column] = sbox[index];
        }
    }
}

// function which shift each row "row" times left.
// @param size = size of each row and column in  the matrix.
// @param matrix its self.
void shiftRows(int size ,int mat[MAX_SIZE][MAX_SIZE] )
{
    int temp[MAX_SIZE];
    for(int i=1 ; i<size ; i++)
    {
        for(int j=0 ; j<size ; j++)
        {
            if(j-i<0)
                temp[size+j-i]=mat[i][j];
                //temp[size-j-1]=mat[i][j];
            else
                temp[j-i]=mat[i][j];

        }
        for(int j=0 ; j<size ; j++)
        {
            mat[i][j] = temp[j];
        }
    }
}





//function will use the sbox array to encrypt the message
void encryption(int iterations ,int size , int mat[MAX_SIZE][MAX_SIZE], int sbox[BYTES])
{
    for(int iter=0 ; iter<iterations ; iter++)
    {
        byteSubstitution(size, mat , sbox);
        shiftRows(size, mat);
    }
}

//function insert user's numbers to the matrix
void fillMatrix(int size, int mat[MAX_SIZE][MAX_SIZE])
{
    for(int row=0; row<size ; row++)
    {
        for(int column=0 ; column<size ; column++)
        {
           scanf("%d",&mat[row][column]);
        }
    }

}

//function checks if users iteration number is valid to the process
int userNumOfIteration()
{
    print_welcome_message1();
    bool num_is_ok = false;
    int num;
    while(!num_is_ok)
    {
        scanf("%d", &num);
        if(num<0)
        {
            print_invalid_amount();
            continue;
        }

        else
            num_is_ok = true;
    }
    return num;

}


//function checks if users matrix size number is valid to the process
int userSizeOfMatrix()
{
    print_welcome_message2();
    int size;
    bool num_is_ok = false;
    while(!num_is_ok)
    {
        scanf("%d", &size);
        if(size<0 || size>MAX_SIZE)
        {
            printf("invalid size, please try again:\n");
            continue;
        }

        else
            num_is_ok = true;
    }
    return size;

}


//print functions:


void print_welcome_message1(){
	printf("Starting the AES Algorithm, please pick amount of iterations:\n");
}

void print_invalid_amount(){
	printf("Invalid amount of iterations!\n");
	printf("Please try again:\n");
}

void print_welcome_message2(){
	printf("Please pick a matrix size:\n");
}

void print_enter_matrix(){
	printf("Please enter the matrix:\n");
}

void print_encrypted_message(){
	printf("The encrypted message is:\n");
}

void print_matrix(int n, int aes[MAX_SIZE][MAX_SIZE]){
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", aes[i][j]);
		}
		printf("\n");
	}
}


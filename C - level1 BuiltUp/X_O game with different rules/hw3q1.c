/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdbool.h>

/*=========================================================================
  Constants and definitions:
==========================================================================*/

#define N 11
#define INDEX 2
#define PLAYER_ONE_INDEX 'X'
#define PLAYER_TWO_INDEX 'O'
#define MAX_MOVES 121
/*-------------------------------------------------------------------------
    Function declaration
-------------------------------------------------------------------------*/
void print_welcome();
int print_enter_board_size();
void print_board(char board[N][N], int n);
void print_player_turn(int player_index);
void print_error();
void print_winner(int player_index);
void print_tie();
void startgame(char board[N][N],int size, int moves[INDEX][MAX_MOVES], int *move_ptr);
bool check_column(char board[N][N],int size);
bool check_row(char board[N][N],int size);
bool check_diagonal_1(char board[N][N],int size);
bool check_diagonal_2(char board[N][N],int size);
bool check_for_win(char board[N][N], int size);
int playerTurn(int *move_ptr);
void scan_check_sendforexcute(char board[N][N], int moves[INDEX][MAX_MOVES],int boardsize, int *move_ptr);
bool Check_if_row_legal(int *move_ptr,int row);
int undo(char board[N][N], int moves[INDEX][MAX_MOVES],int *move_ptr,int row);
int excute_turn(char board[N][N],int row,int column,int moves[INDEX][MAX_MOVES],int* move_ptr);
void FillBoard(char board[N][N],int boardsize);
void check_which_kind_of_win(int *move_ptr,int boardsize);

/*=========================================================================
==========================================================================*/
//function main
//8 lines
int main()
{
    print_welcome();
    char board[N][N];
    int moves[INDEX][MAX_MOVES];
    int counter=0;
    int *move_ptr=&counter;
    int size=print_enter_board_size();
    startgame(board,size,moves,move_ptr);


  return 0;
}

/*-------------------------------------------------------------------------
    Implementation
-------------------------------------------------------------------------*/

//print welcome message
//1 lines
void print_welcome()
{
    printf("*** Welcome to AVOIDANCE TIC-TAC-TOE game ***\n\n");
}

//print message to enter board size and scan the size
//4 lines
int print_enter_board_size()
{
    printf("Please enter board size (1 to %d):\n", N);
    int size=0;
    scanf("%d",&size);
    return size;
}

//the function decide who's turn to play
//5 lines
int playerTurn(int *move_ptr)
{
    if(*move_ptr%2==0){
        int player_index=1;
        return player_index;
    }
    int player_index=2;
    return player_index;
}


//that function will contain all the game process ,order to print each move and to notice who's turn it is.
//7 lines
void startgame(char board[N][N],int size, int moves[INDEX][MAX_MOVES], int *move_ptr)
{
    FillBoard(board,size);
    print_board(board,size);
    while(!(check_for_win(board,size)) && (*move_ptr<(size*size))){
        print_player_turn(playerTurn(move_ptr));
        scan_check_sendforexcute(board,moves,size,move_ptr);
        print_board(board,size);
    }
    check_which_kind_of_win(move_ptr,size);
}


//function scanning checking legalization of the players move
//13 lines
void scan_check_sendforexcute(char board[N][N], int moves[INDEX][MAX_MOVES], int boardsize, int* move_ptr)
{
    int row = 0 , column = 0,done=1;
        while(done==1){
            scanf(" %d",&row);
                if(row <0)
                {
                    if(Check_if_row_legal(move_ptr,row))
                    {
                        undo(board,moves,move_ptr,row);
                        return;
                    }
                    print_error();
                    continue;
                }

            scanf(" %d",&column);
            if(column<1 || column>boardsize||row<1 || row>boardsize|| board[row-1][column-1]!='_'){
                print_error();
            }else
            {
                done = excute_turn(board,row,column,moves,move_ptr);
            }

        }
}

//check if row which scanned is legal
//3 lines
bool Check_if_row_legal(int *move_ptr,int row)
{
    if((row<1 && row%2==0)||((*move_ptr)+row)<0){
        return false;
    }
    return true;
}




//function undo will return the game amount of steps beck according to players request.
//4 lines
int undo(char board[N][N], int moves[INDEX][MAX_MOVES],int *move_ptr,int row)
{

    for(int i=0;i>row;i--){
        board[(moves[0][(*move_ptr)+i-1])][(moves[1][(*move_ptr)+i-1])]='_';
    }
    *move_ptr +=row;
    return row;
}



//function which scan players move , marks at the board and remember it
//8 lines
int excute_turn(char board[N][N],int row,int column,int moves[INDEX][MAX_MOVES],int* move_ptr)
{
        if(*move_ptr%2==0)
            board[row-1][column-1]=PLAYER_ONE_INDEX;
        if(*move_ptr%2==1)
            board[row-1][column-1]=PLAYER_TWO_INDEX;
        moves[0][*move_ptr]=row-1;
        moves[1][*move_ptr]=column-1;
        (*move_ptr)++;
        return 0;

}




//checks if there a full column of the same symbol
//8 lines
bool check_column(char board[N][N],int size)
{
    for (int i = 0; i < size; i++){
        int counter=0;
        for (int j = 1; j < size; j++){
            if(board[i][j-1]!='_'&& board[i][j]!='_' && board[i][j-1]==board[i][j])
                counter++;
            }
        if(counter==size-1 && size!=1)
            return true;
        }
    return false;
}

//check if there a full row of the same symbol
//8 lines
bool check_row(char board[N][N],int size)
{
    for (int j = 0; j < size; j++){
        int counter=0;
        for (int i = 1; i < size; i++){
            if(board[i-1][j]!='_'&& board[i][j]!='_' && board[i-1][j]==board[i][j])
                counter++;
            }
        if(counter==size-1 && size!=1)
            return true;
        }
    return false;
}


//check if there a full down diagonal, of the same symbol
//7 lines
bool check_diagonal_1(char board[N][N],int size)
{
    int counter=0;
    for(int i=1;i<size;i++){
        if(board[i][i]==board[i-1][i-1] && board[i][i]!='_' && board[i-1][i-1]!='_')
            counter++;
    }
    if(counter==size-1 && size!=1)
        return true;
    return false;
}

//check if there a full up diagonal, of the same symbol
//7 lines
bool check_diagonal_2(char board[N][N],int size)
{
    int counter=0;
    for(int i=size-2,j=1;j<size;i--,j++){
        if(board[i][j]==board[i+1][j-1] && board[i][i]!='_' && board[i+1][j-1]!='_')
            counter++;
    }
    if(counter==size-1 && size!=1)
        return true;
    return false;
}


//The function checks if a player won
//9 lines
bool check_for_win(char board[N][N], int size)
{
    if(check_column(board,size))
        return true;
    if (check_row(board,size))
        return true;
    if(check_diagonal_1(board,size))
        return true;
    if(check_diagonal_2(board,size))
        return true;
    return false;
}

//this function will determine if one of the players won or there is a tie
//6 lines
void check_which_kind_of_win(int *move_ptr,int boardsize)
{
    if((*move_ptr)==(boardsize*boardsize) && boardsize!=1){
        print_tie();
        return;
    }
    if((*move_ptr)<MAX_MOVES){
        print_winner(playerTurn(move_ptr));
        return;
    }

}

//Fill all board with '_' chars
//4 lines
void FillBoard(char board[N][N],int boardsize)
{
    for(int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            board[i][j]='_';
        }
    }
    return;
}



//print the board
//7 lines
void print_board(char board[N][N], int n)
{
    printf("\nCurrent board:\n");
    for (int i = 0; i < n; i++)
    {
        printf("|");
        for (int j = 0; j < n; j++)
        {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//print a request for player with index "player_index" to move
//1 lines
void print_player_turn(int player_index)
{
    printf("\nPlayer ** %d **, enter next move:\n", player_index);
}

//print error message
//1 lines
void print_error()
{
    printf("Illegal move!!!, please try again:\n");
}

//print the winner
//1 lines
void print_winner(int player_index)
{
    printf("Player %d Wins! Hooray!\n", player_index);
}

//print message to announce there is a tie (no one wins)
//1 lines
void print_tie()
{
    printf("It's a tie!\n");
}

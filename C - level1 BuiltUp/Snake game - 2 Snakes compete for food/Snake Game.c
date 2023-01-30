/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


/*=========================================================================
  Constants and definitions:
==========================================================================*/

/* put your #defines and typedefs here*/
#define FOOD 'F'
#define WHITE_HEAD_NUMBER 1
#define BLACK_HEAD_NUMBER 2
#define EMPTY   0
#define M 3
#define N 25
#define BOARD_ELEMENT '#'
#define PERCENT 0
#define FIRST_PLAYER PERCENT
#define PERCENT_HEAD '%'
#define SHTRODEL_HEAD '@'
#define SHTRODEL 1
#define ILLEGAL_MOVE 0
#define FOOD_LACK 1
#define SNAKE_NUMBER 2
#define STEP_SIZE 10
#define NUMBER_OF_DIRECTION 4
#define SPACE ' '
#define CONDITION -1
#define COORDINATES 2
#define FIRST_BODY_PART 1
#define SEC_BODY_PART 2
#define HEAD 0
typedef enum {right=6,left=4,up=8,down=2} direction;
typedef enum {w='*', b='+', white_head='%',black_head='@',
                    board_element='#'} draw;




// GLOBAL VARIABLES

typedef struct snake_t{
    int number_of_steps;
    int head_index;
    draw symbol_head;
    draw symbol_body;
}snake;
int board[N][N];
int board_to_print[N][N];
snake snakes[SNAKE_NUMBER];

/*-------------------------------------------------------------------------
*   Function declartion
*--------------------------------------------------------------------------*/
void init_board(int );                                 /*initialize the board */
void init_snakes(int , int[N*N], int[N*N], int);                                 /*initialize snakes*/
void print_introduction_msg();                     /*print welcome message , and introduction message to the game*/
void create_food(int size);                                /*create food in random place */
void print_board(int size);                                /*print the current board for the user*/
void update_board(int size);                                /*print the current board for the user*/
void print_player_message(int );               /*print message when we switch between users*/
bool illegal_snake_move(int , direction, int ,int);   /*return 1 if the move is illegal*/
void print_finish_msg(int ,int );    /*get the player who lose and the reason of losing and print message state*/
void is_snake_die(int ,direction,int,int );             /*return 1 if the current sanke make K step with no eating*/
void make_snake_move(int,direction,int , int max_steps);           /*get direction and make new move*/
int get_next_player(int);                  /*Switch between users */
direction get_snake_move();                        /*get the direction input from the user*/
bool isBoardFul(int );                                  /* if the board is ful return 1 */
int get_next_index(int,int,int);                        /* get current part of the snake and return the next part of the snake*/
void print_introduction_msg();                           /*print welcome message , and introduction message to the game*/
void print_board_size_msg();
void print_steps_without_food();
void print_food_location_msg();
void print_error_input();
void print_invalid_input();
void print_full_board();
void print_insert_direction();
int get_direct_index(direction,int);
void snake_eat_food(int player ,int max_steps , int size);
int askMaxSteps();
int askBoardSize();
void initGame(int board_size , int max_steps);
int askBoradSize();
int checkSnakeLen(int snake_hist[N*N]);
void snakeMoves(int player,direction move,int length_snake[N*N],int size
                ,int max_steps);
bool illegal_move_into_snake(int player , direction move , int size);
void updateSnakeHist(int snake_hist[N*N],direction move,
                     bool eat_food, int size);
void drawMove(int player,int snake_hist[N*N],direction move, bool eat_food
               ,int size);
void startGame(int size ,int length_percent[N*N] ,int length_shtrodel[N*N]
               ,int max_steps);

int main()
{
    print_introduction_msg();
    int board_size = askBoradSize();
    print_steps_without_food();
    int max_steps = askMaxSteps();
    initGame(board_size, max_steps);


  return 0;
}


/*-------------------------------------------------------------------------
*   Function :
*--------------------------------------------------------------------------*/

/*====================================================================
initGame:
    init the game , init snakes , create food, print initial board,
    and start the game
*====================================================================*/
void initGame(int board_size, int max_steps)
{
    init_board(board_size);
    int length_percent[N*N] = {0};
    int length_shtrodel[N*N] = {0};
    init_snakes(board_size,length_percent,length_shtrodel,max_steps);
    create_food(board_size);
    print_board(board_size);
    startGame(board_size , length_percent , length_shtrodel , max_steps);

}
/*====================================================================
 init_snakes:is_snake_die
        inite snake structure .

*====================================================================*/
void init_snakes(int size,int len_percent[N*N],int len_shtrodel[N*N] ,
                 int max_steps)
{
    for(int i=0 ; i<SNAKE_NUMBER ; i++)
    {
        snakes[i].head_index=i*N*(size-1)+1;
        snakes[i].number_of_steps = max_steps;
    }
    len_percent[0] = 1;
    len_percent[1] = 2;
    len_percent[2] = 3;
    len_shtrodel[0] = ((size-1)*N)+1;
    len_shtrodel[1] = ((size-1)*N)+2;
    len_shtrodel[2] = ((size-1)*N)+3;
    return;
}

/*====================================================================
startGame:
        input: board size , 2 histograms of each snake.
        function starts the game and controlling each turn.
*====================================================================*/
void startGame(int size ,int length_percent[N*N] ,int length_shtrodel[N*N]
               ,int max_steps)
{
    int turn=0 , player;
    direction move;
    /*printf("\n STARTING HISTOGRAM % SNAKE:\n");
    for(int counter=0 ; counter<N ; counter++)
    {
        printf("%d|",length_percent[counter]);
    }
    printf("\n");
    printf("\n STARTING HISTOGRAM @ SNAKE:\n");
    for(int counter=0 ; counter<N ; counter++)
    {
        printf("%d|",length_shtrodel[counter]);
    }
    printf("\n");
    */
    while(!isBoardFul(size))
    {
        player = turn%SNAKE_NUMBER;
        //printf("%d",player);
        print_player_message(player);
        move = get_snake_move();
        //printf("IM HERE AFTER GETTING DERCTION\n");
        is_snake_die(player, move,snakes[player].number_of_steps,size);
        //printf("IM HERE AFTER CHECKING IF SNAKE DIES\n");
        snakes[player-1].number_of_steps--;
        if(player == PERCENT)
        {
            //printf("IM HERE % MOVES : player=%d ,dircetion = %d \n", player , move);
            snakeMoves(player, move,length_percent,size , max_steps);
        }
        else
        {
            //printf("IM HERE!!!!!!!!!!\n");
            //printf("IM HERE @ MOVES : player=%d ,dircetion = %d \n", player , move);
            snakeMoves(player , move , length_shtrodel,size,max_steps);
        }
        turn++;

    }
    print_full_board();
}

/*====================================================================
snakeMoves:
        input: player number, direction to move,
        player's snake histogram, board size.
        the function will check the consequences of the player move,
        and execute the move.
*====================================================================*/
void snakeMoves(int player, direction move,int length_snake[N*N],int size
                ,int max_steps)
{
    int head_src = snakes[player].head_index;
    //printf("SNAKES HEAD SOURCE %d\n" , head_src);
    //printf("this is the add to head src = %d",get_direct_index(move, size));
    int head_dest = head_src + get_direct_index(move, size);

    //printf("SNAKES HEAD DEST %d\n" , head_dest);
    bool eat_food = false;
    if(board[head_dest/N][head_dest%N-1]== FOOD)
        eat_food=true;
    if(illegal_snake_move(player, move , size,head_dest))
    {
        print_finish_msg(player , ILLEGAL_MOVE);
        exit(0);
    }
    else
    {
        updateSnakeHist(length_snake,move,eat_food, size);

        drawMove(player , length_snake ,move, eat_food , size);
        print_board(size);
        if(eat_food)
            snake_eat_food(player,max_steps , size);
    }



}


int checkSnakeLen(int snake_hist[N*N])
{
    int length=0;
    while(snake_hist[length]!=0)
    {
        length++;
    }
    return length;
}
/*====================================================================
updateSnakeHist:
        input: player number, direction to move,
        player's snake histogram, board size ,
        if snake will eat food send true else false.

        The function will update snake's histogram which contains
        his head and body location.
*====================================================================*/

void updateSnakeHist(int snake_hist[N*N],direction move,
                     bool eat_food , int size)
{
    int length=checkSnakeLen(snake_hist);
    /*printf("snakes length is %d\n", length);
     if(eat_food)
        printf("eat_food = true\n");
    else
        printf("eat_food = false\n");
    */
    if(eat_food)
    {
        for(int i=length-1 ;i>=0 ; i--)
        {
            snake_hist[i+1] = snake_hist[i];
        }
        snake_hist[0] = snake_hist[0]+ get_direct_index(move, size);
    }
    else
    {
        board[snake_hist[length-1]/N][snake_hist[length-1]%N-1]=SPACE;
        for(int i=length-1 ;i>0 ; i--)
        {
            snake_hist[i]=snake_hist[i-1];
        }
        snake_hist[0] = snake_hist[0]+ get_direct_index(move, size);
    }
    /*printf("\n AFTER UPDATE HIST:\n");
    for(int counter=0 ; counter<N ; counter++)
    {
        printf("%d|",snake_hist[counter]);
    }
    printf("\n");
    */
}


void drawMove(int player,int snake_hist[N*N],direction move, bool eat_food
               ,int size)
{
    int head_src = snakes[player].head_index , i=1;
    int head_dest = head_src + get_direct_index(move, size);
    board[head_dest/N][head_dest%N-1] = board[head_src/N][head_src%N-1];
    snakes[player].head_index=head_dest;
    int snake_body = snake_hist[SEC_BODY_PART];
    if(eat_food)
    {
        board[head_src/N][head_src%N-1] = board[snake_body/N][snake_body%N-1];
        return;
    }
    else
    {
        while(snake_hist[i]!=0)
        {
            board[snake_hist[i]/N][snake_hist[i]%N-1] = board[snake_body/N][snake_body%N-1];
            i++;
        }
        board[snake_hist[i]/N][snake_hist[i]%N-1] = SPACE;
    }
}


bool illegal_move_into_snake(int player , direction move , int size)
{
    int snake_location = snakes[player-1].head_index;
    switch(move)
    {
    case right:
        {
            if(board[snake_location/size][snake_location%size+1]!=SPACE)
                return false;
        }
    case left:
        {
            if(board[snake_location/size][snake_location%size-1]==SPACE)
                return false;
        }
    case up:
        {
            if(board[snake_location/size-1][snake_location%size]==SPACE)
                return false;
        }
    case down:
        {
            if(board[snake_location/size+1][snake_location%size]==SPACE)
                return false;
        }
    }
    return true;
}




/*====================================================================
askBoradSize:
    return size of the board from user.
*====================================================================*/
int askBoradSize()
{
    int size;
    bool size_valid = false;
    print_board_size_msg();
    while(!size_valid)
    {
        scanf("%d",&size);
        if(size>M && size<=N)
            size_valid=true;
        else
            print_invalid_input();

    }
    return size;
}

/*====================================================================
askMaxSteps:
    return max steps snake can do without food,
     of the board from user.
*====================================================================*/
int askMaxSteps()
{
    int max_steps = CONDITION;
    while(max_steps<1)
    {
        scanf("%d",&max_steps);
        if(max_steps<1)
            print_invalid_input();
    }
    return max_steps;
}
/*====================================================================
isBoardFul:
    return true if there is no empty place.otherwise return false
*====================================================================*/
bool isBoardFul(int size)
{
	for(int row=0 ; row<size ; row++)
    {
        for(int column=0 ; column<size ; column++)
        {
            if(board[row][column]==SPACE)
                return false;
        }
    }
    return true;
}


/*====================================================================
create_food:
        scan random number between 0 to N^2 that is empty in the board.
        (we check board[number/N][number%N] is empty)
		hint: You can use do while here. check correctness!

*====================================================================*/
void create_food(int size)
{
    print_food_location_msg();
    int row,column;
    bool valid_coord = false;
    while(!valid_coord)
    {
        scanf("%d%d",&row,&column);
        if(row<0 || column<0 || row>=size || column>=size)
        {
            print_invalid_input();
            continue;
        }
        if(board[row][column]!=' ')
        {
            print_invalid_input();
            continue;
        }
        valid_coord=true;
    }
    board[row][column]=FOOD;
}


/*====================================================================
init_board:
        Init the board,snake and food.
        the snake saved as a sqeuence number.
*====================================================================*/
void init_board(int size)
{
    for(int row=0 ; row<size ; row++)
    {
        for(int column=0 ; column<size ; column++)
        {
            board[row][column] = SPACE;
        }
    }
    board[0][HEAD] = white_head;
    board[0][FIRST_BODY_PART] = w;
    board[0][SEC_BODY_PART] = w;
    board[size-1][HEAD] = black_head;
    board[size-1][FIRST_BODY_PART] = b;
    board[size-1][SEC_BODY_PART] = b;
    return;
}



/*====================================================================
get_snake_move:

        This function get input from user ,validate and return it..
*====================================================================*/
direction get_snake_move()
{
    print_insert_direction();
    int move=0;
    while(!(move%2==0 && move!=0 && move<10))
    {
        scanf("%d", &move);
        if(!(move%2==0 && move!=0 && move<10))
        {
            print_invalid_input();
            move = 0;
        }
    }

    return move;
}
/*====================================================================
is_illegal_snake_move:
        Input: current player, direction.
        Output: return 1 if the next move is illegal, otherwise return 0
*====================================================================*/
bool illegal_snake_move(int player,direction snake_direction,int size
                             , int head_dest)
{
    //int snake_location = snakes[player].head_index;
    //printf("CHECKING : ILLIGEL WALL FUNCTION -SNAKES HEAD SOURCE %d\n" , head_dest);
    switch(snake_direction)
    {
    case right:
        {
        if(board[head_dest/N][head_dest%N-1]==SPACE
           || board[head_dest/N][head_dest%N-1]==FOOD)
                return false;
        }
    case left:
        {
            if(board[head_dest/N][head_dest%N-1]==SPACE
               || board[head_dest/N][head_dest%N-1]==FOOD)
                return false;
        }
    case up:
        {
            if(board[head_dest/N][head_dest%N-1]==SPACE
               || board[head_dest/N][head_dest%N-1]==FOOD)
                return false;
        }
    case down:
        {
            if(board[head_dest/N][head_dest%N-1]==SPACE
               || board[head_dest/N][head_dest%N-1]==FOOD)
                return false;
        }
    }
    return true;

}
/*====================================================================
get_direct_index:
    input: direction
    output: index of the direction '(2-down,4-left,6-right,8-up)
*====================================================================*/
int get_direct_index(direction snake_direction,int size)
{
    switch(snake_direction)
    {
    case down:
        {
            return N;
        }
    case up:
        {
            return (-1)*N;
        }
    case right:
        {
            return 1;
        }
    case left:
        {
            return (-1);
        }
    }
    return 0;
}
/*====================================================================
is_snake_die:
        input: current player, direction
        output: check if the snake did k rounds without eating food
        ASK*********
*====================================================================*/
void is_snake_die(int player,direction snake_direction,int max_steps,int size )
{
    if(snakes[player].number_of_steps==0)
    {
        print_finish_msg(player ,FOOD_LACK);
        exit(0);
    }
    return;
}

/*====================================================================
snake_eat_food:

    Input:player playing, snake's head destination , max steps without
    eating.
    update max moves and updating the board.
*====================================================================*/
void snake_eat_food(int player ,int max_steps , int size)
{
    snakes[player-1].number_of_steps = max_steps;
    create_food(size);
    print_board(size);
}


/*====================================================================
print_introduction_msg:
        print welcome message
*====================================================================*/
void print_introduction_msg()
{
    printf("Welcome to multi-snake  game, The game have 2 player Percent and Shtrodel.\n"
    "The game starts with PERCENT player.\n");
    printf("You have to choose 4 moves :Up,Down,Right and Left "
    "The Percent player is the snake that starts in (0,0)\n\n");
}

void print_board_size_msg(){
    printf("Insert board size between 4 and 25:\n");
}

void print_steps_without_food(){
    printf("Insert the maximum number of steps the snake can do without food:\n");
}

void print_food_location_msg(){
    printf("in order to continue the game please Insert a row then column numbers to place the next food: \n");
}

/*====================================================================
help_print_board:

        this function print static part of the board
*====================================================================*/
void help_print_board(int size)
{
    int i=0;
    printf("%c",BOARD_ELEMENT);
    for(i=0;i<size;i++)
    {
        printf("%c",BOARD_ELEMENT);
    }
    printf("%c\n",BOARD_ELEMENT);
}

/*====================================================================
print_board:

        print the current board
*====================================================================*/
void print_board(int size)
{
    int i=0,j=0;
    help_print_board(size);
    for(i=0;i<size;i++)
    {
        printf("%c",BOARD_ELEMENT);
        for(j=0;j<size;j++)
        {
            printf("%c",board[i][j]);
        }
        printf("%c\n",BOARD_ELEMENT);
    }
    help_print_board(size);
    printf("\n\n");
}

/*====================================================================
print_player_message:

    print message when the changing player
*====================================================================*/
void print_player_message(int player )
{
    printf("******* Turn of %c ***********\n",player==PERCENT?PERCENT_HEAD:SHTRODEL_HEAD);
}

void print_insert_direction(){
    printf("Enter the direction to move your snake: (2-down,4-left,6-right,8-up):\n");
}
/*====================================================================
print_finsih_msg:
        Input: current player, reason of die.
        Output: print finish message
*====================================================================*/
void print_finish_msg(int player ,int finish_reason)
{
    printf("\nThe game finish and the winner is:\n");
    printf("*** %s player ****.\n",player==PERCENT?"SHTRODEL":"PERCENT");
    printf("The reason is %s of the ",finish_reason==ILLEGAL_MOVE? "Illegal move" : "Snake die");
    printf("%s player.\n\n",player==PERCENT?"PERCENT":"SHTRODEL");
}

void print_invalid_input(){
    printf("Please re-Inset valid input:\n");
}

void print_full_board(){
    printf("Full Board. The game over with no Winner.\n");
}
void print_error_input(){
    printf("Error with the input.\n");
}

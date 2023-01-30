#include "Game.h"
#include "Exceptions.h"

using namespace mtm;
using namespace std;

#define CROSS_SOLDIER_SYMBOL 's'
#define CROSS_SNIPER_SYMBOL 'n'
#define CROSS_MEDIC_SYMBOL 'm'
#define POWER_SOLDIER_SYMBOL 'S'
#define POWER_SNIPER_SYMBOL 'N'
#define POWER_MEDIC_SYMBOL 'M'



/** 
 * Game::Game -constructor, create a game and a board to the game. 
 * @param int - number of rows in the board game.
 * @param col - number of columns in the board game.
*/
Game::Game(int row , int col)
{
    if(row<1 || col<1)
    {
        throw IllegalArgument();
    }
    this->row=row;
    this->col=col;
    Board = createBoard(row,col);
}

/** 
 * createBoard - method which create a two-dimensional game board
 * @param row - number of rows to create.
 * @param col - number of column to create.
 *
 * @return:
 * two-dimensional game board
*/
vector<vector<shared_ptr<Character>>> Game::createBoard(int row, int col)
{

    vector<vector<shared_ptr<Character>>> Board;
    for(int i=0; i<row ; i++)
    {
        vector<shared_ptr<Character>> rows(col);
        
        for(int j=0 ; j<col ; j++)
        {
            rows[j] = nullptr;
        }
        Board.push_back(rows);
    }
    
    return Board;
}




/** 
 * distructor - delete Game Class which created
*/
Game::~Game()
{
 this->deleteBoard();
}




/** 
 * deleteBoard - clear each colunm vector contains all the Characters in the board game
*/
void Game::deleteBoard ()
{
    for(int j=row-1 ; j>=0 ; j--)
    {
        Board[j].clear();
    }
}




        
/** 
 * copyAndAllocate - create and add a new Character same to one that "second" that points on.
 * @param first - pointer to a square in the board where we want to create and add the Character second is pointer to. 
 * @param second - pointer to Character which we want to copy by value.
 *
*/
void Game::copyAndAllocate (vector<vector<shared_ptr<Character>>>& first,const vector<vector<shared_ptr<Character>>>& second)
{
   
    for(int i=0; i<row ; i++)
    {
        for(int j=0 ; j<col ; j++)
        {
            if(second[i][j] != nullptr)
            {
                Character* add = (second[i][j])->clone();
                first[i][j] = makeCharacter(add->getType(),add->getTeam(),add->getHealth(),add->getAmmo(),add->getRange(),add->getPower());
                delete add;

            }
        }
    }

}
    



/** 
 * Copy constructor, copying all the details by value of one game to a new game 
 * @param other - pointer to the game we want ti copy all the details from.
*/
Game::Game(const Game& other)
{
    row = other.row;
    col = other.col;
    Board = createBoard(row,col);
    copyAndAllocate(this->Board,other.Board);
}


/** 
 * makeCharacter- creates a new 
 * @param type - type of Character.
 * @param team - hich team the character belongs to.
 * @param health - start amount of health the Character will have .
 * @param ammo - start amount of ammo the Character will have .
 * @param range - the range by squares that the Character can to execute an attack .
 * @param power - the damage that rival oppenent suffered each attack / the amount of health Medic can heal other teamate.
 * 
 * @return:
 * shared_ptr to the new Character according to the demand - Soldier/Medic/Sniper
*/
shared_ptr<Character> Game::makeCharacter(CharacterType type, Team team,units_t health,units_t ammo,units_t range,units_t power)
{
    if(health<1||ammo<0||range<0||power<0)
    {
        throw IllegalArgument();
    }
    if(type==SOLDIER)
    {
        shared_ptr<Character> ptr(new Soldier(health,ammo,range,power,team,type));
        return ptr;
    }
    if(type==MEDIC)
    {
        shared_ptr<Character> ptr(new Medic(health,ammo,range,power,team,type));
        return ptr;
    }
    else //sniper
    {
        shared_ptr<Character> ptr(new Sniper(health,ammo,range,power,team,type));
        return ptr;
    }

}


/**
 * operator=: delete old game details on *this, and copy details from "other" to this by value
 * 
 * @param other - present the game you want to copy the details from
 * @return:
 *          reference to Game object that present copy of "other" game.
 */
Game& Game::operator=(const Game& other)
{
    if(this == &other)
    {
        return *this;
    }
    vector<vector<shared_ptr<Character>>> temp_board = createBoard(other.row,other.col);
    copyAndAllocate(temp_board,other.Board);
    this->deleteBoard();
    this->Board = temp_board;
    return *this;
}


/**
 * addCharacter: function that add new Character to the board, throw exeptions if needed,
 * and update the board - by the term from *.pdf
 * 
 * @param coordinates - present the position user want to place the new character 
 * @param character - all the details of the character user want to add to the board
 */
void Game::addCharacter(const GridPoint coordinates, shared_ptr<Character> character)
{
    if(coordinates.col<0 || coordinates.col >= col ||coordinates.row<0 || coordinates.row>=row)
    {
        throw IllegalCell();
    }
    if(Board.at(coordinates.row).at(coordinates.col)!=NULL)
    {
        throw CellOccupied();
    }
    Board.at(coordinates.row).at(coordinates.col)= character;
    
}


/**
 * move: function that move of character on board, throw exeption if something wrong happend
 *          procceed to move, and update the board - by the term from *.pdf
 * 
 * @param src_coordinates - present the position of character that want be move
 * @param dst_coordinates - present the new position of the character
 */
void Game::move(const GridPoint & src_coordinates, const GridPoint & dst_coordinates)
{
    if(src_coordinates.col<0 || src_coordinates.col >= col ||src_coordinates.row<0 || src_coordinates.row>=row ||
        dst_coordinates.col<0 || dst_coordinates.col >= col ||dst_coordinates.row<0 || dst_coordinates.row>=row)
    {
        throw IllegalCell();
    }
    if(Board.at(src_coordinates.row).at(src_coordinates.col)==nullptr)
    {
        throw CellEmpty();
    }
   
    if(Board.at(src_coordinates.row).at(src_coordinates.col)->getSteps() < GridPoint::distance(src_coordinates,dst_coordinates))
    {
        throw MoveTooFar();
    }
    if(Board.at(dst_coordinates.row).at(dst_coordinates.col)!=nullptr)
    {
        throw CellOccupied();
    }

    Board.at(dst_coordinates.row).at(dst_coordinates.col)=Board.at(src_coordinates.row).at(src_coordinates.col);
    Board.at(src_coordinates.row).at(src_coordinates.col)=nullptr;   
}


/**
 * reload: function that manage the reload in the game - makes the character reload ammo, by the term from *.pdf
 * 
 * @param coordinates - present the position of character that user want to reload with ammo 
 */
void Game::reload(const GridPoint& coordinates)
{
    if(coordinates.col<0 || coordinates.col >= col ||coordinates.row<0 || coordinates.row>=row)
    {
        throw IllegalCell();
    }
    shared_ptr<Character> character = Board.at(coordinates.row).at(coordinates.col);
    if(character==NULL)
    {
        throw CellEmpty();
    }
    character->updateAmmo(character->getReloadAmmo());
}



/**
 * attack: function that manage the attacks in the game, throw exeption if something wrong happend
 *          procceed to attack, and update the board and the charaters that the attack applied on them
 *          by the term from *.pdf
 * 
 * @param src_coordinates - present the position of character that attack
 * @param dst_coordinates - present the position where the attack accourd
 * 
 */
void Game::attack(const GridPoint & src_coordinates, const GridPoint & dst_coordinates)
{
   // printf("here!!\n");
    if(src_coordinates.col<0 || src_coordinates.col >= col ||src_coordinates.row<0 || src_coordinates.row>=row ||
        dst_coordinates.col<0 || dst_coordinates.col >= col ||dst_coordinates.row<0 || dst_coordinates.row>=row)
    {
        throw IllegalCell();
    }
    shared_ptr<Character> character = Board.at(src_coordinates.row).at(src_coordinates.col);
    shared_ptr<Character> enemy = Board.at(dst_coordinates.row).at(dst_coordinates.col);
    if(character==NULL)
    {
        throw CellEmpty();
    }
    if(!character->possibleAttack(src_coordinates,dst_coordinates))
    {
        throw OutOfRange();
    }
    if(character->getType()==SOLDIER||character->getType()==SNIPER)
    {
        if(character->getAmmo()-character->getAmmoPerAttack()<0)
        {
            throw OutOfAmmo();
        }
    }
    else //medic attack
    {
        if(enemy != nullptr && character->getTeam()!=enemy->getTeam()&&character->getAmmo()-character->getAmmoPerAttack()<0)
        {
           throw OutOfAmmo();
        }
    }
    
    character->attackByCharacter(Board,src_coordinates,dst_coordinates);
   
}



/**
 * isOver: give status if the game is ended (by terms in *pdf), and update the winning team in the pointer
 * 
 * @param winningTeam - an pointer to Team type, if not nullptr and the game is over, 
 *                      the value update to the name of the winning team
 * @return : 
 *          true - if any there a team that wins (no more character in the enemy team)
 *          false - if the game is still on
 */
bool Game::isOver(Team* winningTeam)const
{
    bool powerLifters = false;
    bool crossFitters = false;
    for(int i=0; i<row ; i++)
    {
        for(int j=0 ; j<col ; j++)
        {
            if(Board.at(i).at(j)!=nullptr && Board.at(i).at(j)->getTeam()==CROSSFITTERS)
            {
                crossFitters=true;
            }
            if(Board.at(i).at(j)!=nullptr && Board.at(i).at(j)->getTeam()==POWERLIFTERS)
            {
                powerLifters=true;
            }
            if(powerLifters&&crossFitters)
            {
                return false;
            }
        }
    }
    if(powerLifters)
    {
        if(winningTeam!=nullptr)
        {
            *winningTeam = POWERLIFTERS;
        }
        return true;
    }
    if(crossFitters)
    {
        if(winningTeam!=nullptr)
        {
            *winningTeam = CROSSFITTERS;
        }
        return true;
    }
    return false;
}



/**
 * operator<<: print game board like describe in *.pdf (use print operator of Auxileries)
 * 
 * @param os - ostream print function
 * @param game - the game you want to print his board
 * @return : 
 *          reference to ostream object
 */
namespace mtm {
    ostream&  operator<<(ostream& os,const Game& game)
    {
        
       vector<char> board (game.row*game.col,' ');
        char* start = &board[0];
        char* end = &board[game.row*game.col];
        int index=0;
        for(int i=0;i<game.row;i++)
        {
            for(int j=0;j<game.col;j++)
            {
                if(game.Board.at(i).at(j)!=nullptr)
                {

                    if(game.Board.at(i).at(j)->getTeam()==CROSSFITTERS)
                    {
                        if(game.Board.at(i).at(j)->getType()==SOLDIER)
                        {
                            board.at(index)=CROSS_SOLDIER_SYMBOL;
                        }
                        if(game.Board.at(i).at(j)->getType()==SNIPER)
                        {
                            board.at(index)=CROSS_SNIPER_SYMBOL;
                        }
                        if(game.Board.at(i).at(j)->getType()==MEDIC)
                        {
                           board.at(index)=CROSS_MEDIC_SYMBOL;
                        }
                    }
                    else
                    {
                    if(game.Board.at(i).at(j)->getType()==SOLDIER)
                        {
                            board.at(index)=POWER_SOLDIER_SYMBOL;
                        }
                        if(game.Board.at(i).at(j)->getType()==SNIPER)
                        {
                           board.at(index)=POWER_SNIPER_SYMBOL;
                        }
                        if(game.Board.at(i).at(j)->getType()==MEDIC)
                        {
                            board.at(index)=POWER_MEDIC_SYMBOL;
                        }
                    }

                }
               index++;
            }
        }
        std::ostream& check = printGameBoard(os,start,end,game.col);
        return check;
    }
}

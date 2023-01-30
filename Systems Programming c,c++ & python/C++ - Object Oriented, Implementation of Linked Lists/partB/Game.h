#include <vector>
#include <memory>
#include "Character.h"
#include "Soldier.h"
#include "Medic.h"
#include "Sniper.h"
#include "Auxiliaries.h"

namespace mtm{

    /** 
     *  Game is a class that represents each game board that described in the *pdf
     */
    class Game {

        private:
        int row;
        int col;
        std::vector<std::vector<std::shared_ptr<Character>>> Board;


        /** 
          * createBoard - method which create a two-dimensional game board
          * @param row - number of rows to create.
          * @param col - number of column to create.
          *
          * @return:
          * two-dimensional game board
        */
        std::vector<std::vector<std::shared_ptr<Character>>> createBoard(int row, int col);
        
        
        /** 
          * copyAndAllocate - create and add a new Character same to one that "second" that points on.
          * @param first - pointer to a square in the board where we want to create and add the Character second is pointer to. 
          * @param second - pointer to Character which we want to copy by value.
          *
        */
        void copyAndAllocate (std::vector<std::vector<std::shared_ptr<Character>>>& first, const std::vector<std::vector<std::shared_ptr<Character>>>& second);
        
        
        
        /** 
          * deleteBoard - clear each colunm vector contains all the Characters in the board game
        */
        void deleteBoard ();


        public:


        /** 
         * Game::Game -constructor, create a game and a board to the game. 
         * @param int - number of rows in the board game.
         * @param col - number of columns in the board game.
        */
        Game(int row , int col); 


        /** 
         * distructor - delete Game Class which created
        */
        ~Game();

        /** 
         * Copy constructor, copying all the details by value of one game to a new game 
         * @param other - pointer to the game we want ti copy all the details from.
        */
        Game(const Game& other);

        /**
         * operator=: delete old game details on *this, and copy details from "other" to this by value
         * 
         * @param other - present the game you want to copy the details from
         * @return:
         *          reference to Game object that present copy of "other" game.
         */
        static std::shared_ptr<Character> makeCharacter(CharacterType type, Team team,units_t health,units_t ammo,units_t range,units_t power);


        /**
         * addCharacter: function that add new Character to the board, throw exeptions if needed,
         * and update the board - by the term from *.pdf
         * 
         * @param coordinates - present the position user want to place the new character 
         * @param character - all the details of the character user want to add to the board
         */
        void addCharacter(const GridPoint coordinates, std::shared_ptr<Character> character);

        /**
         * operator=: delete old game details on *this, and copy details from "other" to this by value
         * 
         * @param other - present the game you want to copy the details from
         * @return:
         *          reference to Game object that present copy of "other" game.
         */
        Game& operator=(const Game& other) ;


        /**
         * move: function that move of character on board, throw exeption if something wrong happend
         *          procceed to move, and update the board - by the term from *.pdf
         * 
         * @param src_coordinates - present the position of character that want be move
         * @param dst_coordinates - present the new position of the character
         */
        void move(const GridPoint & src_coordinates, const GridPoint & dst_coordinates);


        /**
         * reload: function that manage the reload in the game - makes the character reload ammo, by the term from *.pdf
         * 
         * @param coordinates - present the position of character that user want to reload with ammo 
         */
        void reload(const GridPoint& coordinates);


       /**
         * attack: function that manage the attacks in the game, throw exeption if something wrong happend
         *          procceed to attack, and update the board and the charaters that the attack applied on them
         *          by the term from *.pdf
         * 
         * @param src_coordinates - present the position of character that attack
         * @param dst_coordinates - present the position where the attack accourd
         * 
         */
        void attack(const GridPoint & src_coordinates, const GridPoint & dst_coordinates);


        /**
         * isOver: give status if the game is ended (by terms in *pdf), and update the winning team in the pointer
         * 
         * @param winningTeam - an pointer to Team type, if not nullptr and the game is over, 
         *                      the value update to the name of the winning team
         * @return : 
         *          true - if any there a team that wins (no more character in the enemy team)
         *          false - if the game is still on
         */
        bool isOver(Team* winningTeam) const;

        /**
         * operator<<: print game board like describe in *.pdf (use print operator of Auxileries)
         * 
         * @param os - ostream print function
         * @param game - the game you want to print his board
         * @return : 
         *          reference to ostream object
         */
        friend std::ostream &operator<<(std::ostream& , const Game&);
    };  
}
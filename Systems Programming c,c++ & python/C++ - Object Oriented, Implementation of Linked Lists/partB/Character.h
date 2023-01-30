#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Auxiliaries.h"
#include "vector"
#include <memory>
#include "Exceptions.h"

namespace mtm{

    /**
     * Character is the Master class which the other classes will heritage it beheiver by polymorphism.
    */
    class Character
    {
        private:
        
        protected:
        int health;
        int ammo;
        int range;
        int power;
        Team team;
        CharacterType type;


        /** 
          * Character::Character -constructor - virtual -
          * @param health - health of Character.
          * @param ammo - ammo capacity.
          * @param range - range Character can attack.
          * @param power - damage per Character's attack. 
          * @param team - which team the character belongs to.
          * @param type - type of Character - Character.
        */
        Character(int health , int ammo, int range , int power ,  Team team ,  CharacterType type);
    

        public:

        /**
         * virtual distructor for Character class. 
         */
        virtual ~Character() = 0;


        /** clone - virtual - creates a copy of Character .
         *  @return:
         *  pointer to the new Character's copy. 
         */
        virtual Character*  clone() const= 0;


        /**
         *  getSteps - virtual - checks the number of moves the Character can march.
         *  @return :
         *  int number which represent the number of STEPS Character can move each turn. 
         */
        virtual int getSteps() const=0;


        /**
         *  getReloadAmmo - virtual - checks the number of ammo that Character can reload.
         *  @return :
         *  int number which represent the number of ammo Character can reload per reload.  
         */
        virtual int getReloadAmmo()=0 ;


        /**
         *  updateAmmo - virtual - update to new amount of ammo according to the Character actions.
         *  @param ammo - the amount of ammo the Character used/gets which depends on the sign before the integer.  
         */
        virtual void updateAmmo(int)=0 ;


        /**
         *  updateHealth - virtual - update to new amount of health according to the Character actions.
         *  @param points - the amount of ammo the Character used/gets which depends on the sign before the integer.  
         */
        virtual void updateHealth(int)=0;


        /**
         *  getHealth - virtual - check the health of the Character.
         *  @return:
         *  int number which represent the number of health. 
         */
        virtual int getHealth()const=0;



        /**
         *  getAmmo - virtual - check the number of ammo Character has at th momment.
         *  @return:
         *  int number which represent the number of ammo Character has. 
         */
        virtual int getAmmo()const=0;



        /**
         *  getRange - virtual - check the range Character can attack.
         *  @return:
         *  int number which represent the maximum number of squares Character attack from the square he stands. 
         */
        virtual int getRange()const=0;



        /**
         *  getAmmoPerAttack - virtual - check the number of ammo each Character's attack reduce.
         *  @return:
         *  int number which represent the number of ammo Character uses each attack. 
         */
        virtual int getAmmoPerAttack()const=0;



        /**
         *  getPower - virtual - check the damage Character does in each attack.
         *  @return:
         *  int number which represent the damage Character does to rival each attack. 
         */
        virtual int getPower()const=0;



        /**
         *  getTeam - virtual - check which team the Character belong.
         *  @return:
         *  enum - Team the Character belongs to. 
         */
        virtual Team getTeam()const=0;


        /**
         *  getType - virtual - check which type of Character is.
         *  @return:
         *  enum - CharacterType. 
         */
        virtual CharacterType getType()const=0;


        /** 
          * possibleAttack- virtual -method which check if Character can attack particular square by *pdf instruction.
          * @param src_coordinates - position of Character.
          * @param dst_coordinates - target.
          *
          * @return:
          *  true - attack is possible by all conditions.
          *  false - attack isn't possible by all cconditions.
        */
        virtual bool possibleAttack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)const=0;



        /**
         *  attackByCharacter- virtual -preceed the attack by Character attacks by *pdf instruction.
         * @param first - game Board.
         * @param src_coordinates - position of Medic.
         * @param dst_coordinates - target.
        */
        virtual void attackByCharacter(std::vector<std::vector<std::shared_ptr<Character>>>& first,const GridPoint& src_coordinates, const GridPoint& dst_coordinates)=0;
    };
}


#endif



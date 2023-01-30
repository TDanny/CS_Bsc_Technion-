#ifndef SNIPER_H_
#define SNIPER_H_
#include "Character.h"
    
namespace mtm{    
    
    /**
     * Sniper class is a polymorphism of Character which characteristics by *pdf.
     */
    class Sniper : public Character
    {
        private:
        int strike_attack;

        public:

        /** Copy constructor - copying from other to this by value.
         * @param other - pointer to the Sniper we want to copy by value from. 
         */
        Sniper(const Sniper& other);


        /** clone - creates a copy of Sniper Character.
         *  @return:
         *  pointer to the new Sniper's copy. 
         */
        Character* clone() const override;


        /** 
          * Sniper::Sniper -constructor 
          * @param health - health of Sniper.
          * @param ammo - ammo capacity.
          * @param range - range Sniper can attack.
          * @param power - damage per Sniper's attack. 
          * @param team - which team the character belongs to.
          * @param type - type of Character - Sniper.
        */
        Sniper(const int health ,const int ammo,const int range ,const int power , const Team team ,const CharacterType type);
        
        
        /**
         *  distructor - delete Sniper class. 
         */
        ~Sniper() override = default;


        /**
         *  getSteps - checks the number of moves the Sniper can march.
         *  @return :
         *  int number which represent the number of STEPS Sniper can move each turn. 
         */
        int getSteps() const override; 


         /**
         *  getReloadAmmo - checks the number of ammo that Sniper can reload.
         *  @return :
         *  int number which represent the number of ammo Sniper can reload per reload.  
         */
        int getReloadAmmo()override ;


         /**
         *  updateAmmo - update to new amount of ammo according to the Sniper actions.
         *  @param ammo - the amount of ammo the Sniper used/gets which depends on the sign before the integer.  
         */
        void updateAmmo(int) override;


        /**
         *  updateHealth - update to new amount of health according to the Sniper actions.
         *  @param points - the amount of ammo the Sniper used/gets which depends on the sign before the integer.  
         */
        void updateHealth(int)override;


        /**
         *  getHealth - check the health of the Sniper.
         *  @return:
         *  int number which represent the number of health. 
         */
        int getHealth()const override;


        /**
         *  getAmmoPerAttack - check the number of ammo each Sniper's attack reduce.
         *  @return:
         *  int number which represent the number of ammo Sniper uses each attack. 
         */
        int getAmmoPerAttack()const override;


        /**
         *  getAmmo - check the number of ammo Sniper has at th momment.
         *  @return:
         *  int number which represent the number of ammo Sniper has. 
         */
        int getAmmo()const override;


        /**
         *  getRange - check the range Sniper can attack.
         *  @return:
         *  int number which represent the maximum number of squares Sniper attack from the square he stands. 
         */
        int getRange()const override;


        /**
         *  getPower - check the damage Sniper does in each attack.
         *  @return:
         *  int number which represent the damage Sniper does to rival each attack. 
         */
        int getPower()const override;



         /**
         *  getTeam - check which team the Sniper belong.
         *  @return:
         *  enum - Team the Sniper belongs to. 
         */
        Team getTeam()const override;


        /**
         *  getType - check which type of Character is.
         *  @return:
         *  enum - CharacterType- SNIPER. 
         */
        CharacterType getType()const override;


        /** 
          * possibleAttack:method which check if Sniper can attack particular square by *pdf instruction.
          * @param src_coordinates - position of Sniper.
          * @param dst_coordinates - target.
          *
          * @return:
          *  true - attack is possible by all conditions.
          *  false - attack isn't possible by all cconditions.
        */
        bool possibleAttack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)const override;


        /**
         * attackOccurred: method which counts the number Sniper attacked and double his power in every third attack, if the counter%3 isn't An
         *  integer with no remainder then the power updated to be as a default.
         */
        void attackOccurred();



        /**
         *  attackByCharacter: preceed the attack by Sniper attacks by *pdf instruction.
         * @param first - game Board.
         * @param src_coordinates - position of Sniper.
         * @param dst_coordinates - target.
        */
        void attackByCharacter(std::vector<std::vector<std::shared_ptr<Character>>>& board \
        ,const GridPoint& src_coordinates, const GridPoint& dst_coordinates)override;

    };

    
}

#endif

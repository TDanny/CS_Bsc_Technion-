#ifndef SOLDIER_H_
#define SOLDIER_H_
#include "Character.h"
    
namespace mtm{    
    
    /**
     * Soldier class is a polymorphism of Character which בharacteristics by *pdf.
     */
    class Soldier : public Character
    {
        public:

        /** Copy constructor - copying from other to this by value.
         * @param other - pointer to the Soldier we want to copy by value from. 
         */
        Soldier(const Soldier& other);


        /** clone - creates a copy of Soldier Character.
         *  @return:
         *  pointer to the new Soldier's copy. 
         */
        Character *clone() const override;


        /** 
          * Soldier::Soldier -constructor 
          * @param health - health of soldier.
          * @param ammo - ammo capacity.
          * @param range - range Soldier can attack.
          * @param power - damage per Soldier's attack. 
          * @param team - which team the character belongs to.
          * @param type - type of Character - Soldier.
        */
        Soldier( int health , int ammo, int range , int power ,  Team team , CharacterType type);
        
        
        /**
         *  distructor - delete Soldier class. 
         */
        ~Soldier() override = default;


        /**
         *  getSteps - checks the number of moves the Soldier can march.
         *  @return :
         *  int number which represent the number of STEPS soldier can move each turn. 
         */
        int getSteps() const override; 


        /**
         *  getReloadAmmo - checks the number of ammo that Soldier can reload.
         *  @return :
         *  int number which represent the number of ammo soldier can reload per reload.  
         */
        int getReloadAmmo()override ; 


        /**
         *  updateAmmo - update to new amount of ammo according to the Soldier actions.
         *  @param ammo - the amount of ammo the soldier used/gets which depends on the sign before the integer.  
         */
        void updateAmmo(int ammo) override; 


        /**
         *  updateHealth - update to new amount of health according to the Soldier actions.
         *  @param points - the amount of ammo the soldier used/gets which depends on the sign before the integer.  
         */
        void updateHealth(int points)override;


        /**
         *  getHealth - check the health of the Soldier.
         *  @return:
         *  int number which represent the number of health. 
         */
        int getHealth()const override;


        /**
         *  getAmmoPerAttack - check the number of ammo each Soldier's attack reduce.
         *  @return:
         *  int number which represent the number of ammo Soldier uses each attack. 
         */
        int getAmmoPerAttack()const override;


        /**
         *  getAmmo - check the number of ammo Soldier has at th momment.
         *  @return:
         *  int number which represent the number of ammo Soldier has. 
         */
        int getAmmo()const override;


        /**
         *  getRange - check the range Soldier can attack.
         *  @return:
         *  int number which represent the maximum number of squares Soldier attack from the square he stands. 
         */
        int getRange()const override;


        /**
         *  getPower - check the damage Soldier does in each attack.
         *  @return:
         *  int number which represent the damage Soldier does to rival each attack. 
         */
        int getPower()const override;


        /**
         *  getTeam - check which team the Soldier belong.
         *  @return:
         *  enum - Team the Soldier belongs to. 
         */
        Team getTeam()const override;


        /**
         *  getType - check which type of Character is.
         *  @return:
         *  enum - CharacterType- SOLDIER. 
         */
        CharacterType getType()const override;

         /** 
          * possibleAttack:method which check if Soldier can attack particular square by *pdf instruction.
          * @param src_coordinates - position of Soldier.
          * @param dst_coordinates - target.
          *
          * @return:
          *  true - attack is possible by all conditions.
          *  false - attack isn't possible by all cconditions.
        */
        bool possibleAttack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)const override;


        /**
         *  attackByCharacter: preceed the attack by Soldier attacks by *pdf instruction.
         * @param first - game Board.
         * @param src_coordinates - position of Soldier.
         * @param dst_coordinates - target.
        */
        void attackByCharacter(std::vector<std::vector<std::shared_ptr<Character>>>& first,const GridPoint& src_coordinates, const GridPoint& dst_coordinates) override;
        
        
        /**
         *  damagedByAttack: check if rival character suffered damage according to its position relative to the place of attack.
         * @param attack_coordinates - position of the strike attack.
         * @param character_coordinates - target.
         *
         * @return:
         * true - rival character suffered damage.
         * false - rival character isn't suffered damage.
        */
        bool damagedByAttack(const GridPoint& attack_coordinates, const GridPoint& character_coordinates)const ;

    };

    

}

#endif //CHARACTER_H_

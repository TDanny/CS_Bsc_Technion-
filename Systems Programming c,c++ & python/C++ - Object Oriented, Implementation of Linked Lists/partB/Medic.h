#ifndef MEDIC_H_
#define MEDIC_H_


#include "Character.h"
    
namespace mtm{    
    
    /**
     * Medic class is a polymorphism of Character which characteristics by *pdf.
     */
    class Medic : public Character
    {
        public:


        /** Copy constructor - copying from other to this by value.
         * @param other - pointer to the Medic we want to copy by value from. 
         */
        Medic(const Medic& other);


        /** clone - creates a copy of Medic Character.
         *  @return:
         *  pointer to the new Medic's copy. 
         */
        Character* clone() const override;


        /** 
          * Medic::Medic -constructor 
          * @param health - health of Medic.
          * @param ammo - ammo capacity.
          * @param range - range Medic can attack.
          * @param power - damage per Medic's attack. 
          * @param team - which team the character belongs to.
          * @param type - type of Character - Medic.
        */
        Medic(const int health ,const int ammo,const int range ,const int power , const Team team ,const CharacterType type);
        
        
        /**
         *  distructor - delete Medic class. 
         */
        ~Medic() override = default;


         /**
         *  getSteps - checks the number of moves the Medic can march.
         *  @return :
         *  int number which represent the number of STEPS Medic can move each turn. 
         */
        int getSteps() const override; 


         /**
         *  getReloadAmmo - checks the number of ammo that Medic can reload.
         *  @return :
         *  int number which represent the number of ammo Medic can reload per reload.  
         */
        int getReloadAmmo()override ; 


        /**
         *  updateAmmo - update to new amount of ammo according to the Medic actions.
         *  @param ammo - the amount of ammo the Medic used/gets which depends on the sign before the integer.  
         */
        void updateAmmo(int) override; 



        /**
         *  updateHealth - update to new amount of health according to the Medic actions.
         *  @param points - the amount of ammo the Medic used/gets which depends on the sign before the integer.  
         */
        void updateHealth(int)override; 


        /**
         *  getHealth - check the health of the Medic.
         *  @return:
         *  int number which represent the number of health. 
         */
        int getHealth()const override;



        /**
         *  getAmmoPerAttack - check the number of ammo each Medic's attack reduce.
         *  @return:
         *  int number which represent the number of ammo Medic uses each attack. 
         */
        int getAmmoPerAttack()const override;



        /**
         *  getAmmo - check the number of ammo Medic has at th momment.
         *  @return:
         *  int number which represent the number of ammo Medic has. 
         */
        int getAmmo()const override;



        /**
         *  getRange - check the range Medic can attack.
         *  @return:
         *  int number which represent the maximum number of squares Medic attack from the square he stands. 
         */
        int getRange()const override;


        /**
         *  getPower - check the damage Medic does in each attack.
         *  @return:
         *  int number which represent the damage Medic does to rival each attack. 
         */
        int getPower()const override;



        /**
         *  getTeam - check which team the Medic belong.
         *  @return:
         *  enum - Team the Medic belongs to. 
         */
        Team getTeam()const override;


        /**
         *  getType - check which type of Character is.
         *  @return:
         *  enum - CharacterType- MEDIC. 
         */
        CharacterType getType()const override;


        /** 
          * possibleAttack:method which check if Medic can attack particular square by *pdf instruction.
          * @param src_coordinates - position of Medic.
          * @param dst_coordinates - target.
          *
          * @return:
          *  true - attack is possible by all conditions.
          *  false - attack isn't possible by all cconditions.
        */
        bool possibleAttack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)const override; 
        
        
        
        /**
         *  attackByCharacter: preceed the attack by Medic attacks by *pdf instruction.
         * @param first - game Board.
         * @param src_coordinates - position of Medic.
         * @param dst_coordinates - target.
        */
        void attackByCharacter(std::vector<std::vector<std::shared_ptr<Character>>>& board,const GridPoint& src_coordinates, const GridPoint& dst_coordinates) override;

    };

}

#endif
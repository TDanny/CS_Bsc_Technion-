
#include "Medic.h"

#define STEPS 5
#define AMMO_TO_RELOAD 5
#define AMMO_USED 1
#define POWER_SCALE 3

using namespace mtm ;
using namespace std ;



/** 
 * Medic::Medic -constructor 
 * @param health - health of Medic.
 * @param ammo - ammo capacity.
 * @param range - range Medic can attack.
 * @param power - damage per Medic's attack. 
 * @param team - which team the character belongs to.
 * @param type - type of Character - Medic.
*/
Medic::Medic(const int health ,const int ammo,const int range ,const int power , const Team team ,const CharacterType type):
Character(health ,ammo ,range ,power, team , type)
{
}


/**
 *  getSteps - checks the number of moves the Medic can march.
 *  @return :
 *  int number which represent the number of STEPS Medic can move each turn. 
 */
int Medic::getSteps() const
{
    return STEPS;
}



/**
 *  getReloadAmmo - checks the number of ammo that Medic can reload.
 *  @return :
 *  int number which represent the number of ammo Medic can reload per reload.  
 */
int Medic::getReloadAmmo()
{
    return AMMO_TO_RELOAD;
}


/**
 *  updateAmmo - update to new amount of ammo according to the Medic actions.
 *  @param ammo - the amount of ammo the Medic used/gets which depends on the sign before the integer.  
 */
void Medic::updateAmmo(int ammo) 
{
    this->ammo+=ammo;
}



/**
 *  updateHealth - update to new amount of health according to the Medic actions.
 *  @param points - the amount of ammo the Medic used/gets which depends on the sign before the integer.  
 */
void Medic::updateHealth(int points) 
{
    this->health+=points;
}



/**
 *  getHealth - check the health of the Medic.
 *  @return:
 *  int number which represent the number of health. 
 */
int Medic::getHealth() const
{
    return this->health;
}


/**
 *  getAmmo - check the number of ammo Medic has at th momment.
 *  @return:
 *  int number which represent the number of ammo Medic has. 
 */
int Medic::getAmmo()const
{
    return this->ammo;
}
/**
 *  getRange - check the range Medic can attack.
 *  @return:
 *  int number which represent the maximum number of squares Medic attack from the square he stands. 
 */
int Medic::getRange()const
{
    return this->range;
}



/**
 *  getPower - check the damage Medic does in each attack.
 *  @return:
 *  int number which represent the damage Medic does to rival each attack. 
 */
int Medic::getPower()const
{
    return this->power;
}



/**
 *  getTeam - check which team the Medic belong.
 *  @return:
 *  enum - Team the Medic belongs to. 
 */
Team Medic::getTeam()const
{
    return this->team;
}



/**
 *  getType - check which type of Character is.
 *  @return:
 *  enum - CharacterType- MEDIC. 
 */
CharacterType Medic::getType()const
{
    return this->type;
}



/** 
 * possibleAttack:method which check if Medic can attack particular square by *pdf instruction.
 * @param src_coordinates - position of Medic.
 * @param dst_coordinates - target.
 *
 * @return:
 *  true - attack is possible by all conditions.
 *  false - attack isn't possible by all cconditions.
*/
bool Medic::possibleAttack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)const
{
    int distance = GridPoint::distance(src_coordinates,dst_coordinates);
    
    return(distance<=range);
}


/** Copy constructor - copying from other to this by value.
 * @param other - pointer to the Medic we want to copy by value from. 
 */
Medic::Medic(const Medic& other):
Character(other.health ,other.ammo ,other.range ,other.power, other.team , other.type)
{

}


/** clone - creates a copy of Medic Character.
 *  @return:
 *  pointer to the new Medic's copy. 
 */
Character* Medic::clone() const 
{
    return new Medic(*this);
}


/**
 *  getAmmoPerAttack - check the number of ammo each Medic's attack reduce.
 *  @return:
 *  int number which represent the number of ammo Medic uses each attack. 
 */
int Medic::getAmmoPerAttack()const
{
    return AMMO_USED;
}


/**
 *  attackByCharacter: preceed the attack by Medic attacks by *pdf instruction.
 * @param first - game Board.
 * @param src_coordinates - position of Medic.
 * @param dst_coordinates - target.
*/
void Medic::attackByCharacter(vector<vector<shared_ptr<Character>>>& board , const GridPoint& src_coordinates, const GridPoint& dst_coordinates )
{
    //printf("medic 1 \n");
    shared_ptr<Character> target = board.at(dst_coordinates.row).at(dst_coordinates.col);
    if(GridPoint::distance(src_coordinates,dst_coordinates)==0 || target==nullptr)
    {
        throw IllegalTarget();
    }
    // printf("medic 2 \n");
    //printf("medic 2 \n");
    if(target->getTeam() != this->getTeam())
    {
        this->updateAmmo(-AMMO_USED);
        target->updateHealth(-power);

    }
    //  printf("medic 3 \n");
    if(target->getTeam() == this->getTeam())
    {
        target->updateHealth(+power);
    }
    if(target->getHealth()<=0)
    {
        board.at(dst_coordinates.row).at(dst_coordinates.col) = nullptr;
        target=nullptr;
    }
}
#include "Sniper.h"


#define STEPS 4  
#define AMMO_TO_RELOAD 2
#define AMMO_USED 1
#define POWER_SCALE 2
#define STRIKE_COMBO 3
#define STRIKE_DAMEGE 2

using namespace mtm;
using namespace std;



/** 
 * Sniper::Sniper -constructor 
 * @param health - health of Sniper.
 * @param ammo - ammo capacity.
 * @param range - range Sniper can attack.
 * @param power - damage per Sniper's attack. 
 * @param team - which team the character belongs to.
 * @param type - type of Character - Sniper.
*/
Sniper::Sniper(const int health ,const int ammo,const int range ,const int power , const Team team ,const CharacterType type):
Character(health ,ammo ,range ,power, team , type)
{
    strike_attack=0;
}


/**
 *  getSteps - checks the number of moves the Sniper can march.
 *  @return :
 *  int number which represent the number of STEPS Sniper can move each turn. 
 */
int Sniper::getSteps() const
{
    return STEPS;
}


/**
 *  getReloadAmmo - checks the number of ammo that Sniper can reload.
 *  @return :
 *  int number which represent the number of ammo Sniper can reload per reload.  
 */
int Sniper::getReloadAmmo()
{
    return AMMO_TO_RELOAD;
}



/**
 *  updateAmmo - update to new amount of ammo according to the Sniper actions.
 *  @param ammo - the amount of ammo the Sniper used/gets which depends on the sign before the integer.  
 */
void Sniper::updateAmmo(int ammo) 
{
    this->ammo+=ammo;
}




/**
 *  updateHealth - update to new amount of health according to the Sniper actions.
 *  @param points - the amount of ammo the Sniper used/gets which depends on the sign before the integer.  
 */
void Sniper::updateHealth(int points) 
{
    this->health+=points;
}



/**
 *  getHealth - check the health of the Sniper.
 *  @return:
 *  int number which represent the number of health. 
 */
int Sniper::getHealth() const
{
    return this->health;
}


/**
 *  getAmmo - check the number of ammo Sniper has at th momment.
 *  @return:
 *  int number which represent the number of ammo Sniper has. 
 */
int Sniper::getAmmo()const
{
    return this->ammo;
}



/**
 *  getRange - check the range Sniper can attack.
 *  @return:
 *  int number which represent the maximum number of squares Sniper attack from the square he stands. 
 */
int Sniper::getRange()const
{
    return this->range;
}


/**
 *  getPower - check the damage Sniper does in each attack.
 *  @return:
 *  int number which represent the damage Sniper does to rival each attack. 
 */
int Sniper::getPower()const
{
    return this->power;
}



/**
 *  getTeam - check which team the Sniper belong.
 *  @return:
 *  enum - Team the Sniper belongs to. 
 */
Team Sniper::getTeam()const
{
    return this->team;
}



/**
 *  getType - check which type of Character is.
 *  @return:
 *  enum - CharacterType- SNIPER. 
 */
CharacterType Sniper::getType()const
{
    return this->type;
}


    
/** 
 * possibleAttack:method which check if Sniper can attack particular square by *pdf instruction.
 * @param src_coordinates - position of Sniper.
 * @param dst_coordinates - target.
 *
 * @return:
 *  true - attack is possible by all conditions.
 *  false - attack isn't possible by all cconditions.
*/
bool Sniper::possibleAttack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)const
{
    int distance = src_coordinates.distance(src_coordinates,dst_coordinates);
    int minimum_distance = ceil((double)range/(double)POWER_SCALE);
    return (distance<=range && distance>=minimum_distance);
}



/**
 * attackOccurred: method which counts the number Sniper attacked and double his power in every third attack, if the counter%3 isn't An
 *  integer with no remainder then the power updated to be as a default.
 */
void Sniper::attackOccurred()
{
    strike_attack++;
    if(strike_attack%STRIKE_COMBO==0)
    {
        power=power*STRIKE_DAMEGE;
    }
    if(strike_attack%STRIKE_COMBO==1 && strike_attack!=1)
    {
        power=power/STRIKE_DAMEGE;
    }
}



/** Copy constructor - copying from other to this by value.
 * @param other - pointer to the Sniper we want to copy by value from. 
 */
Sniper::Sniper(const Sniper& other):
Character(other.health ,other.ammo ,other.range ,other.power, other.team , other.type)
{

}



/** clone - creates a copy of Sniper Character.
 *  @return:
 *  pointer to the new Sniper's copy. 
 */
Character* Sniper::clone() const 
{
    return new Sniper(*this);
}


/**
 *  getAmmoPerAttack - check the number of ammo each Sniper's attack reduce.
 *  @return:
 *  int number which represent the number of ammo Sniper uses each attack. 
 */
int Sniper::getAmmoPerAttack()const
{
    return AMMO_USED;
}



/**
 *  attackByCharacter: preceed the attack by Sniper attacks by *pdf instruction.
 * @param first - game Board.
 * @param src_coordinates - position of Sniper.
 * @param dst_coordinates - target.
*/
void Sniper::attackByCharacter(vector<vector<shared_ptr<Character>>>& board,const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
{
    shared_ptr<Character> target = board.at(dst_coordinates.row).at(dst_coordinates.col);
    if(target==nullptr || target->getTeam() == team )
    {
        throw IllegalTarget();
    }
    this->attackOccurred();
    this->updateAmmo(-AMMO_USED);
    target->updateHealth(-this->getPower());
    if(target->getHealth()<=0)
    {
        board.at(dst_coordinates.row).at(dst_coordinates.col) = nullptr;
        target=nullptr;
    }
}

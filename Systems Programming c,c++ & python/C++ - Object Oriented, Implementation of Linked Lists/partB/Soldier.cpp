#include "Soldier.h"
#define STEPS 3
#define AMMO_TO_RELOAD 3
#define AMMO_USED 1
#define POWER_SCALE 3
#define DAMAGE_SCALE 2

using namespace mtm;
using namespace std;

/** 
     * Soldier::Soldier :constructor 
     * @param health - health of soldier.
     * @param ammo - ammo capacity.
     * @param range - range Soldier can attack.
     * @param power - damage per Soldier's attack. 
     * @param team - which team the character belongs to.
     * @param type - type of Character - Soldier.
*/
Soldier::Soldier( int health , int ammo, int range , int power ,  Team team , CharacterType type):
Character(health ,ammo ,range ,power, team , type)
{
}


/**
 *  getSteps - checks the number of moves the Soldier can march.
 *  @return :
 *  int number which represent the number of STEPS soldier can move each turn. 
 */
int Soldier::getSteps() const
{
    return STEPS;
}


/**
 *  getReloadAmmo - checks the number of ammo that Soldier can reload.
 *  @return :
 *  int number which represent the number of ammo soldier can reload per reload.  
 */
int Soldier::getReloadAmmo()
{
    return AMMO_TO_RELOAD;
}


/**
 *  updateAmmo - update to new amount of ammo according to the Soldier actions.
 *  @param ammo - the amount of ammo the soldier used/gets which depends on the sign before the integer.  
 */
void Soldier::updateAmmo(int ammo) 
{
    this->ammo+=ammo;
}


/**
 *  updateHealth - update to new amount of health according to the Soldier actions.
 *  @param points - the amount of health the soldier reduce/get depends on the sign before the integer.  
 */
void Soldier::updateHealth(int points) 
{
    this->health+=points;
}


/**
 *  getHealth - check the health of the Soldier.
 *  @return:
 *  int number which represent the number of health. 
 */
int Soldier::getHealth() const
{
    return this->health;
}



/**
 *  getAmmo - check the number of ammo Soldier has at th momment.
 *  @return:
 *  int number which represent the number of ammo Soldier has. 
 */
int Soldier::getAmmo()const
{
    return this->ammo;
}


/**
 *  getRange - check the range Soldier can attack.
 *  @return:
 *  int number which represent the maximum number of squares Soldier attack from the square he stands. 
 */
int Soldier::getRange()const
{
    return this->range;
}


/**
 *  getPower - check the damage Soldier does in each attack.
 *  @return:
 *  int number which represent the damage Soldier does to rival each attack. 
 */
int Soldier::getPower()const
{
    return this->power;
}


/**
 *  getTeam - check which team the Soldier belong.
 *  @return:
 *  enum - Team the Soldier belongs to. 
 */
Team Soldier::getTeam()const
{
    return this->team;
}


/**
 *  getType - check which type of Character is.
 *  @return:
 *  enum - CharacterType- SOLDIER. 
 */
CharacterType Soldier::getType()const
{
    return this->type;
}


/** 
 * possibleAttack:method which check if Soldier can attack particular square by *pdf instruction.
 * @param src_coordinates - position of Soldier.
 * @param dst_coordinates - target.
 *
 * @return:
 *  true - attack is possible by all conditions.
 *  false - attack isn't possible by all cconditions.
*/
bool Soldier::possibleAttack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)const
{

        return src_coordinates.distance(src_coordinates,dst_coordinates)<=range;
}



/**
 * damagedByAttack: check if rival character suffered damage according to its position relative to the place of attack.
 * @param attack_coordinates - position of the strike attack.
 * @param character_coordinates - target.
 *
 * @return:
 * true - rival character suffered damage.
 * false - rival character isn't suffered damage.
*/
bool Soldier::damagedByAttack(const GridPoint& attack_coordinates, const GridPoint& character_coordinates)const
{
    int new_range = ceil((double)range/(double)POWER_SCALE);
    return attack_coordinates.distance(attack_coordinates,character_coordinates)<=new_range;
}


/** Copy constructor - copying from other to this by value.
 * @param other - pointer to the Soldier we want to copy by value from. 
 */
Soldier::Soldier(const Soldier& other):
    Character(other.health ,other.ammo ,other.range ,other.power, other.team , other.type)

{

}


/** clone - creates a copy of Soldier Character.
 *  @return:
 *  pointer to the new Soldier's copy. 
 */
Character* Soldier::clone() const 
{
    return new Soldier(*this);
}

/**
 *  getAmmoPerAttack - check the number of ammo each Soldier's attack reduce.
 *  @return:
 *  int number which represent the number of ammo Soldier uses each attack. 
 */
int Soldier::getAmmoPerAttack()const
{
    return AMMO_USED;
}



/**
 *  attackByCharacter: preceed the attack by Soldier attacks by *pdf instruction.
 * @param first - game Board.
 * @param src_coordinates - position of Soldier.
 * @param dst_coordinates - target.
*/
void Soldier::attackByCharacter(vector<vector<shared_ptr<Character>>>& board,const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
{
if(src_coordinates.row != dst_coordinates.row && src_coordinates.col != dst_coordinates.col)
{
    throw IllegalTarget();
}

int area_damage = ceil((double)power/(double)DAMAGE_SCALE);
for(int i=0; i<(int)board.size() ; i++)
    {
    for(int j=0 ; j<(int)board[i].size() ; j++)
    {
        if(board.at(i).at(j)!= nullptr && board.at(i).at(j)->getTeam()!=team) // not empty cell and not a teammate
        {
            GridPoint enemy(i,j);
            if(this->damagedByAttack(dst_coordinates,enemy))
            {
                if(enemy==dst_coordinates) // strike attack
                {
                    board.at(i).at(j)->updateHealth(-power);
                }
                else //area attack
                {
                    board.at(i).at(j)->updateHealth(-area_damage);
                }
                if(board.at(i).at(j)->getHealth()<=0)
                {
                    board.at(i).at(j) =nullptr;
                }
            }
        }
    }
}
this->updateAmmo(-AMMO_USED);
}

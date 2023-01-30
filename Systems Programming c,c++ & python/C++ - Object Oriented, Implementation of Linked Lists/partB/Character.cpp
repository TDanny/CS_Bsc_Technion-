#include "Character.h"

using namespace mtm;

/** 
 * Character::Character -constructor - virtual -
 * @param health - health of Character.
 * @param ammo - ammo capacity.
 * @param range - range Character can attack.
 * @param power - damage per Character's attack. 
 * @param team - which team the character belongs to.
 * @param type - type of Character - Character.
*/
Character::Character (int health , int ammo, int range , int power ,  Team team ,  CharacterType type):
health(health), ammo(ammo), range(range),power(power),team(team),type(type)
{}

Character::~Character()
{
    
}
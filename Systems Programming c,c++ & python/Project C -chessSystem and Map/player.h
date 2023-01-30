#ifndef PLAYER_H_
#define PLAYER_H_
 
#ifndef FILE_H_
#include <stdlib.h>
#include "chessSystem.h"
#include "./mtm_map/map.h"
#endif //FILE_H_

/**
* Player Card Container
*
* The following functions are available:
*   copyPlayerId		-  copy player ID
*   comparePlayerId		- compare id between players
*   copyPlayerData		- Copy Player Data
*   freePlayerId		- Free Player Id
*   freePlayerData	- Free Player Data
*/

/** Type for representing a Player Card that plays in chess tournaments */
typedef struct player_t{
    int id_player;
    int wins;
    int loses;
    int draws;
    int total_game_time;
}*Player;

//----enmus and defines----//


//----functions----//

/**
 * copyPlayerId: copying an player's Id as a key
 *
 * @param player_id - player Id that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty ID;
 *     MapKeyElement - that present the id. 
 */
MapKeyElement copyPlayerId (MapKeyElement player_id);

/**
 * comparePlayerId: comparing Players ID
 *
 * @param first_id - player's id of player #1
 * @param second_id - player's id of player #2
 * @return
 *     0 - if player id's is equal
 *     -1 - if first player's id is greater than second's player id
 *     1 - if second player's id is greater than first's player id
 */
int comparePlayerId (MapKeyElement first_id,MapKeyElement second_id);

/**
 * copyPlayerData: copying a player data as MapDataElement
 *e
 * @param player - player Data that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty Data;
 *     MapDataElement - that present the player's data. 
 */
MapDataElement copyPlayerData (MapDataElement player);


/**
* freePlayerId: freeing Player Id Key.
* @param player_id
* 	Target player id to remove all element from.
*/
void freePlayerId (MapKeyElement player_id);


/**
* freePlayerData: freeing Player data key.
* @param player
* 	Target player data to remove all element from.
*/
void freePlayerData (MapDataElement player);

#endif //PLAYER_H_
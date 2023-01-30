#ifndef CHESSGAME_H_
#define CHESSGAME_H_

#ifndef FILE_H_
#include "chessSystem.h"
#include "./mtm_map/map.h"
#include <stdlib.h>
#endif //FILE_H_


/**
* Chess game Card Container
*
* The following functions are available:
*   copyGameId		-  copy Games ID
*   compareGamesId		- compare id between Games
*   copyGameData		- Copy Game Data
*   freeGameId		- Free Game Id
*   freeGameData	- Free Game Data
*/

//----enmus and defines----//

//-----struct-----//
/** Type for representing a game in chess system */
typedef struct chess_game_t{
    int first_player_id;
    int second_player_id;
    Winner game_result;
    int game_time;
    int game_id;
    bool first_player_deleted;
    bool second_player_deleted;
}*ChessGame;

//----functions----//

/**
 * copyGameId: copying an games's Id as a key
 *
 * @param game_id - games Id that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty ID;
 *     MapKeyElement - that present the id. 
 */
MapKeyElement copyGameId (MapKeyElement game_id);

/**
 * compareGamesId: comparing Games ID
 *
 * @param first_id - game's id of game #1
 * @param second_id - game's id of game #2
 * @return
 *     0 - if id's is equal
 *     -1 - if first  id is greater than second's  id
 *     1 - if second  id is greater than first's  id
 */
int  compareGamesId (MapKeyElement first_id,MapKeyElement second_id);
/**
 * copyGameData: copying a game data as MapDataElement
 *e
 * @param game - game Data that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty Data;
 *     MapDataElement - that present the game's data. 
 */
MapDataElement copyGameData (MapDataElement game);


/**
* freeGameId: freeing game Id Key.
* @param game_id
* 	Target game id to remove all element from.
*/
void freeGameId (MapKeyElement game_id);

/**
* freeGameData: freeing game data key.
* @param game
* 	Target game data to remove all element from.
*/
void freeGameData (MapDataElement game);

#endif //CHESSGAME_H_
#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_

#ifndef FILE_H_
#include <stdlib.h>
#include <string.h>
#include "chessSystem.h"
#include "./mtm_map/map.h"
#endif //FILE_H_


/**
* Chess Tournament Card Container
*
* The following functions are available:
*   copyTournamentId		-  copy Games ID
*   compareTournamentsId		- compare id between Games
*   copyTournamentData		- Copy Game Data
*   freeTournamentId		- Free Game Id
*   freeTournamentData	- Free Game Data
*    
*/

//----enmus and defines----//


#define BOTTOM_CAPITAL_LETTERS 'A'
#define TOP_CAPITAL_LETTERS 'Z'
#define BOTTOM_SMALL_LETTERS 'a'
#define TOP_SMALL_LETTERS 'z'
#define SPACE_ASCII ' '


/** Type for representing a tournement in chess system contain all the games in the tournemnts*/
typedef struct tournament_t{
    char* tournament_location;
    int max_games_per_player;
    Map games;
    int winner_id;
    bool in_progress; // defual is true
    int tournament_id;
    int number_of_players;
}*Tournament;


//----enmus----//

//----functions----//

/**
 * copyTouranmentId: copying an tournament's Id as a key
 *
 * @param tournament_id - tournament Id that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty ID;
 *     MapKeyElement - that present the id. 
 */
MapKeyElement copyTouranmentId (MapKeyElement tournament_id);


/**
 * compareTournamentsId: comparing Tournaments ID
 *
 * @param first_id - Tournament's id of game #1
 * @param second_id - Tournament's id of game #2
 * @return
 *     0 - if id's is equal
 *     -1 - if first  id is greater than second's  id
 *     1 - if second  id is greater than first's  id
 */
int compareTournamentsId (MapKeyElement first_id,MapKeyElement second_id);

/**
 * copyTournamentData: copying a Tournament data as MapDataElement
 *e
 * @param tournament - Tournament Data that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty Data;
 *     MapDataElement - that present the tournament's data. 
 */
MapDataElement copyTournamentData (MapDataElement tournament);


/**
* freeTournamentId: freeing tourament Id Key.
* @param tourament_id
* 	Target tourament id to remove all element from.
*/
void freeTournamentId (MapKeyElement tourament_id);

/**
* freeTournamentData: freeing tournament data key.
* @param tournament
* 	Target tournament data to remove all element from.
*/
void freeTournamentData (MapDataElement tournament);

/**
* tournamentLocationValid: freeing tournament data key.
* @param tournament_location
* 	    the tournament id. Must be non-negative, and unique.
* @return 
*    Ture - start with a capital letter (A -Z) followed by small letters (a -z) and spaces (' ').
*    False - Everything else, include empty
*/
bool tournamentLocationValid (const char* tournament_location);


#endif //TOURNAMENT_H_
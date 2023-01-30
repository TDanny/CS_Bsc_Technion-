#include "chessGame.h"
/**
 * copyGameId: copying an games's Id as a key
 *
 * @param game_id - games Id that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty ID;
 *     MapKeyElement - that present the id. 
 */
MapKeyElement copyGameId (MapKeyElement game_id)
{
    int* new_game_id = malloc(sizeof(*new_game_id));
    if(!new_game_id)
    {
        return NULL;
    }
    *new_game_id = *(int*)game_id;
    return new_game_id;
}

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
int  compareGamesId (MapKeyElement first_id,MapKeyElement second_id)
{
    return (*(int *) first_id - *(int *) second_id);
}

/**
 * copyGameData: copying a game data as MapDataElement
 *e
 * @param game - game Data that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty Data;
 *     MapDataElement - that present the game's data. 
 */
MapDataElement copyGameData (MapDataElement game)
{
    if(!game)
    {
        return NULL;
    }
    ChessGame new_game = malloc(sizeof(*new_game));
    if(!new_game)
    {
        return NULL;
    }
    new_game ->first_player_id = ((ChessGame)game)->first_player_id;
    new_game ->second_player_id = ((ChessGame)game)->second_player_id;
    new_game ->game_result = ((ChessGame)game)->game_result;
    new_game ->game_time = ((ChessGame)game)->game_time;
    new_game ->first_player_deleted = ((ChessGame)game)->first_player_deleted;
    new_game ->second_player_deleted = ((ChessGame)game)->second_player_deleted;
    return (MapDataElement)new_game;
}

/**
* freeGameId: freeing game Id Key.
* @param game_id
* 	Target game id to remove all element from.
*/
void freeGameId (MapKeyElement game_id)
{
    if(!game_id)
    {
        return;
    }
    free(game_id);
}

/**
* freeGameData: freeing game data key.
* @param game
* 	Target game data to remove all element from.
*/
void freeGameData (MapDataElement game)
{
    if(!game)
    {
        return;
    }
    free(game);
}

#include "player.h"


/**
 * copyPlayerId: copying an player's Id as a key
 *
 * @param player_id - player Id that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty ID;
 *     MapKeyElement - that present the id. 
 */
MapKeyElement copyPlayerId (MapKeyElement player_id)
{
    if(!player_id)
    {
        return NULL;
    }
    int* new_player_id = malloc(sizeof(*new_player_id));
    if(!new_player_id)
    {
        return false;
    }
    *new_player_id = *(int*)player_id;
    return new_player_id;
}

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
int comparePlayerId (MapKeyElement first_id,MapKeyElement second_id)
{
    return (*(int *) first_id - *(int *) second_id);
}

/**
 * copyPlayerData: copying a player data as MapDataElement
 *e
 * @param player - player Data that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty Data;
 *     MapDataElement - that present the player's data. 
 */
MapDataElement copyPlayerData (MapDataElement player)
{
    if(!player)
    {
        return NULL;
    }
    Player new_player = malloc(sizeof(*new_player));
    if(!new_player)
    {
        return NULL;
    }
    new_player ->id_player = ((Player)player)->id_player;
    new_player ->loses = ((Player)player)->loses;
    new_player ->draws = ((Player)player)->draws;
    new_player ->wins = ((Player)player)->wins;
    new_player ->total_game_time =((Player)player)->total_game_time;
    return (MapDataElement)new_player;
}

/**
* freePlayerId: freeing Player Id Key.
* @param player_id
* 	Target player id to remove all element from.
*/
void freePlayerId (MapKeyElement player_id)
{
    if(!player_id)
    {
        return;
    }
    free(player_id);
}

/**
* freePlayerData: freeing player data key.
* @param player
* 	Target player data to remove all element from.
*/
void freePlayerData (MapDataElement player)
{
    if(!player)
    {
        return;
    }
    free(player);
}


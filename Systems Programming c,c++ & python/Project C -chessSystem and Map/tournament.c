#include "tournament.h"

/**
 * copyTouranmentId: copying an tournament's Id as a key
 *
 * @param tournament_id - tournament Id that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty ID;
 *     MapKeyElement - that present the id. 
 */
MapKeyElement copyTouranmentId (MapKeyElement tournament_id)
{
    if(!tournament_id)
    {
        return NULL;
    }
    int* new_tournament_id = malloc (sizeof(*new_tournament_id));
    if(!new_tournament_id)
    {
        return NULL;
    }
    *new_tournament_id = *(int*)tournament_id;
    return new_tournament_id;
}

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
int compareTournamentsId (MapKeyElement first_id,MapKeyElement second_id)
{
        return (*(int *) first_id - *(int *) second_id);
}
/**
 * copyTournamentData: copying a Tournament data as MapDataElement
 *e
 * @param tournament - Tournament Data that user want to copy
 *
 * @return
 *     NULL - in case of malloc error or case of empty Data;
 *     MapDataElement - that present the tournament's data. 
 */
MapDataElement copyTournamentData (MapDataElement tournament)
{
    
    if(!tournament)
    {
        return NULL;
    }
    Tournament new_tournament = malloc(sizeof(*new_tournament));
    if(!new_tournament)
    {
        return NULL;
    }
    Tournament tournament_original = (Tournament) tournament;
    char* location = malloc (sizeof(char)*(strlen(tournament_original->tournament_location)+1));
    if(!location)
    {
        free(new_tournament);
        return NULL;
    }
    Map game_to_copy = ((Tournament)tournament)->games;
    Map games = mapCopy (game_to_copy);
    if(!games)
    {
        free(location);
        free(new_tournament);
        return NULL;
    }
    new_tournament ->tournament_id=tournament_original->tournament_id;
    new_tournament ->max_games_per_player = tournament_original->max_games_per_player;
    strcpy(location,tournament_original->tournament_location);
    new_tournament ->tournament_location = location;
    new_tournament ->in_progress = tournament_original->in_progress;
    new_tournament->winner_id = tournament_original->winner_id;
    new_tournament->games = games;
    new_tournament->number_of_players = tournament_original->number_of_players;
    return (MapDataElement) new_tournament;
}

/**
* freeTournamentId: freeing tourament Id Key.
* @param tourament_id
* 	Target tourament id to remove all element from.
*/
void freeTournamentId (MapKeyElement tourament_id)
{
    if(!tourament_id)
    {
        return;
    }
    free(tourament_id);
}

/**
* freeTournamentData: freeing tournament data key.
* @param tournament
* 	Target tournament data to remove all element from.
*/
void freeTournamentData (MapDataElement tournament)
{
    if(!tournament)
    {
        return;
    }
    if ((((Tournament)tournament)->tournament_location))
    {
        free(((Tournament)tournament)->tournament_location);
    }
    if(((Tournament)tournament)->games)
    {
        mapDestroy(((Tournament)tournament)->games);
    }
    free(tournament);
}


/**
* tournamentLocationValid: freeing tournament data key.
* @param tournament_location
* 	    the tournament id. Must be non-negative, and unique.
* @return 
*    Ture - start with a capital letter (A -Z) followed by small letters (a -z) and spaces (' ').
*    False - Everything else, include empty
*/
bool tournamentLocationValid (const char* tournament_location)
{
    if(!tournament_location)
    {
        return false;
    }
    for(int i=0;tournament_location[i];i++)
    {
        if(i==0&&(tournament_location[i]<BOTTOM_CAPITAL_LETTERS||tournament_location[i]>TOP_CAPITAL_LETTERS))
        {
            return false;
        }
        if(i!=0&&(tournament_location[i]<BOTTOM_SMALL_LETTERS||tournament_location[i]>TOP_SMALL_LETTERS)&&tournament_location[i]!=SPACE_ASCII)
        {
            return false;
        }
    }
    return true;
}
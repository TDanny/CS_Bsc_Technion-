#ifndef FILES_H_
#define FILES_H_
#include <stdlib.h>
#include <string.h>
#include "chessSystem.h"
#include "chessGame.h"
#include "tournament.h"
#include "player.h"
#include "./mtm_map/map.h"
#endif

//----------------------------Defines------------------------------------//

#define CHESS_TABLE_SIZE 10
#define ADD_POINT 1 
#define REMOVE_POINT -1
#define ROWS 4
#define PLAYERS_EACH_GAME 2

#define PLAYER_LISTED 1
#define ROW_LEVELS 2

#define PLAYER_ID 0
#define PLAYER_POINTS 1
#define PLAYER_LOSES 2
#define PLAYER_WINS 3

#define LEVEL_WIN_VALUE 6
#define LEVEL_LOSE_VALUE 10
#define LEVEL_DRAW_VALUE 2

//----------------------------Defines------------------------------------//

//----------------------------Enums------------------------------------//
typedef enum {
    POINTS_FOR_LOSE,
    POINTS_FOR_DRAW,
    POINTS_FOR_WIN
} Points;

typedef enum{
    PLAYER_ONE,
    PLAYER_TWO
} PlayerSide;
//----------------------------Enums------------------------------------//

//----------------------------Struct------------------------------------//

/** Type for representing a chess system that organizes chess tournaments */
struct chess_system_t{
    Map chess_tournament_table;
    Map players_list;
};
//----------------------------Struct------------------------------------//

//----------------------------Functions------------------------------------//
static void UpdateHistogramPlayer (int ** histogram_points,ChessGame game,int player_position,PlayerSide player_side,int player_id);
static bool checkSamePointInHistogram (double **histogram,int histogram_size,double level_of_current_max,int index_of_current_max);
static void** createHistogram(int rows, int columns,size_t size);
static void freeHistogram(void** histogram,int rows);
static int playerExistInPointTable(int **histPoints  ,int player_id , int lenght_of_row_histogram);
static int longestGameTimeAndAverage (Tournament tournament, double* ptr_average_game_time_in_tournament);
static bool checkAlreadyExistGame (Tournament tournament,int first_player, int second_player);
static int getPlayerNumberOfGames (Tournament tournament,int player_id);
static MapResult addPlayer (Map players_list,int* player_id);
static void updatePlayerScore (Player player,PlayerSide player_side,Winner game_result,int add_score,int game_time);
static int generateGameId(ChessSystem chess);
static ChessResult printLevelsToFile (FILE* file,double **histogram_points,int num_of_players,double max_level,int max_level_index);
static int checkWinner(int** histogram_points , int arr_size);
static int indexOfSmallClosesNumber(double **histogram,int histogram_size,double number);
static bool checkIfAnyTournamentEnded(ChessSystem chess);
static ChessResult printTournamentsStatisticsToFile (FILE* file,Tournament tournament);
static void updateTournamentPlayers(Tournament tournament,int first_player,int second_player,int edit_command);
//----------------------------Functions------------------------------------//


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


//----------------------------Chess Functions-----------------------------------------//

/**
 * chessCreate: create an empty chess system.
 *
 * @return A new chess system in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
ChessSystem chessCreate()
{
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    if(!chess_system)
    {
        return NULL;
    }
    chess_system->chess_tournament_table = mapCreate(copyTournamentData,copyTouranmentId,freeTournamentData,freeTournamentId,compareTournamentsId);
    if(!chess_system->chess_tournament_table)
    {
        free(chess_system);
        return NULL; 
    }
    chess_system->players_list = mapCreate(copyPlayerData,copyPlayerId,freePlayerData,freePlayerId,comparePlayerId);
    if(!chess_system->players_list)
    {
        mapDestroy(chess_system->chess_tournament_table);
        free(chess_system);
        return NULL;
    }
    return chess_system;
}
/**
 * chessDestroy: free a chess system, and all its contents, from
 * memory.
 *
 * @param chess - the chess system to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void chessDestroy(ChessSystem chess)
{
    if(!chess)
    {
        return;
    }
    if(chess->chess_tournament_table)
    {
        mapDestroy(chess->chess_tournament_table);
    }
    if(chess->players_list)
    {
        mapDestroy(chess->players_list);
    }
    free(chess);
}

/**
 * chessAddTournament: add a new tournament to a chess system.
 *
 * @param chess - chess system to add the tournament to. Must be non-NULL.
 * @param tournament_id - new tournament id. Must be non-negative, and unique.
 * @param tournament_location - location in which the tournament take place. Must be non-empty.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess/tournament_location are NULL.
 *     CHESS_INVALID_ID - the tournament ID number is invalid.
 *     CHESS_INVALID_LOCATION - if the name is empty or doesn't start with a capital letter (A -Z)
 *                      followed by small letters (a -z) and spaces (' ').
 *     CHESS_TOURNAMENT_ALREADY_EXIST - if a tournament with the given id already exist.
 *     CHESS_SUCCESS - if tournament was added successfully.
 */
ChessResult chessAddTournament (ChessSystem chess, int tournament_id,
                                int max_games_per_player, const char* tournament_location)
{
    if(!chess||!tournament_location)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id<=0)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->chess_tournament_table,(MapKeyElement)&tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if(!tournamentLocationValid(tournament_location))
    {
        return CHESS_INVALID_LOCATION;
    }
    if(max_games_per_player<=0)
    {
        return CHESS_INVALID_MAX_GAMES;
    }
    Tournament tournament = malloc(sizeof(*tournament));
    if(!tournament)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    char* location = malloc (sizeof(char)*(strlen(tournament_location)+1));
    if(!location)
    {
        free(tournament);
        return CHESS_OUT_OF_MEMORY;
    }
    strcpy(location, tournament_location);
    tournament->tournament_location = location;
    tournament->max_games_per_player = max_games_per_player;
    tournament->games = mapCreate(copyGameData,copyGameId,freeGameData,freeGameId,compareGamesId);
    tournament->winner_id = 0;
    tournament->tournament_id = tournament_id;
    tournament->in_progress = true;
    tournament->number_of_players=0;
    MapResult result = mapPut(chess->chess_tournament_table,(MapKeyElement)(&(tournament->tournament_id)),(MapDataElement)tournament);
    freeTournamentData(tournament);
    if(result !=MAP_SUCCESS)
    {
        return CHESS_NULL_ARGUMENT;
    }
    return CHESS_SUCCESS;

}

/**
 * chessAddGame: add a new match to a chess tournament.
 *
 * @param chess - chess system that contains the tournament. Must be non-NULL.
 * @param tournament_id - the tournament id. Must be non-negative, and unique.
 * @param first_player - first player id. Must be non-negative.
 * @param second_player - second player id. Must be non-negative.
 * @param winner - indicates the winner in the match. if it is FIRST_PLAYER, then the first player won.
 *                 if it is SECOND_PLAYER, then the second player won, otherwise the match has ended with a draw.
 * @param play_time - duration of the match in seconds. Must be non-negative.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the tournament ID number, either the players or the winner is invalid.
 *     CHESS_TOURNAMENT_NOT_EXIST - if the tournament does not exist in the system.
 *     CHESS_GAME_ALREADY_EXIST - if there is already a game in the tournament with the same two players.
 *     CHESS_INVALID_PLAY_TIME - if the play time is negative.
 *     CHESS_SUCCESS - if game was added successfully.
 * 
 */
ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time)
{
    if(!chess)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id<=0||first_player<=0||second_player<=0||first_player==second_player
        ||(winner!=FIRST_PLAYER && winner!=SECOND_PLAYER && winner!=DRAW))
    {
        return CHESS_INVALID_ID;
    }
    if(!mapContains(chess->chess_tournament_table,(MapKeyElement)(&tournament_id)))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Tournament tournament = (Tournament)mapGet(chess->chess_tournament_table,(MapKeyElement)&tournament_id);
    if(!tournament->in_progress)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    if(checkAlreadyExistGame(tournament,first_player,second_player))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if(play_time<0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }
    if(getPlayerNumberOfGames(tournament,first_player)>=tournament->max_games_per_player
        ||getPlayerNumberOfGames(tournament,second_player)>=tournament->max_games_per_player)
    {
        return CHESS_EXCEEDED_GAMES;
    }
    ChessGame new_game = malloc(sizeof(*new_game));
    if(!new_game)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    new_game->first_player_id = first_player;
    new_game->second_player_id = second_player;
    new_game->game_time = play_time;
    new_game->game_result = winner;
    new_game->first_player_deleted = false;
    new_game->second_player_deleted = false;
    int new_game_id = generateGameId(chess);
    if(MAP_SUCCESS != mapPut(tournament->games ,(MapKeyElement)&new_game_id,(MapDataElement)new_game))
    {
        freeGameData(new_game);
        return CHESS_NULL_ARGUMENT;
    }
    updateTournamentPlayers(tournament,first_player,second_player,ADD_POINT);
    // add players to player chess system list (if needed) and update score game at player card
    Map players_list = chess->players_list;
    if(addPlayer(players_list,&first_player)!=MAP_SUCCESS || addPlayer(players_list,&second_player)!=MAP_SUCCESS)
    {
        mapRemove(tournament->games,(MapKeyElement)&new_game_id);
        updateTournamentPlayers(tournament,first_player,second_player,REMOVE_POINT);
        return CHESS_OUT_OF_MEMORY;
    }
    updatePlayerScore(mapGet(chess->players_list, &first_player),PLAYER_ONE,winner,ADD_POINT,play_time);
    updatePlayerScore(mapGet(chess->players_list, &second_player),PLAYER_TWO,winner,ADD_POINT,play_time);
    freeGameData(new_game);
    return CHESS_SUCCESS;
}


/**
 * checkAlreadyExistGame: check if there is same game in specific system
 * @return -
 *         true - if exist game in tournament system
 *         false - if doesn't
 *
 **/
static bool checkAlreadyExistGame (Tournament tournament,int first_player, int second_player)
{  
    MapKeyElement game_id=mapGetFirst(tournament->games);
    ChessGame game = (ChessGame) mapGet(tournament->games,game_id);
    freeGameId(game_id);
    while(game)
    {
            if(((game->first_player_id==first_player && game->second_player_id==second_player )||
            (game->first_player_id==second_player && game->second_player_id==first_player ))&&
            (!game->first_player_deleted&&!game->second_player_deleted))
            {
                return true;
            }
        game_id=mapGetNext(tournament->games);
        game =(ChessGame) mapGet(tournament->games,game_id);
        freeGameId(game_id);
        
    }
    return false;
}

/**
 * getPlayerNumberOfGames: count the number of games that player play in specific tournament
 * @param tournament - tournament that contain all game to check number of game that player already played
 * @param player_id - pointer to the player Id, non negative
 * @return - number of game that player already play in specif tournament
 *
 **/
static int getPlayerNumberOfGames (Tournament tournament,int player_id)
{
    MapKeyElement game_id=mapGetFirst(tournament->games);
    ChessGame game = (ChessGame)mapGet(tournament->games,game_id);
    int games_counter=0;
    while(game)
    {
        if(game->first_player_id == player_id && !game->first_player_deleted)
        {
           games_counter++;
        }
        if(game->second_player_id == player_id && !game->second_player_deleted)
        {
            games_counter++;
        }
        freeGameId(game_id);
        game_id=mapGetNext(tournament->games);
        game = (ChessGame)mapGet(tournament->games,game_id);
    }
    return games_counter;
}
/**
 * addPlayer: add player to chess players list, if already exist does nothing, and update
 * @param players_list - list all the players in chess system
 * @param player_id - pointer to the player Id, non negative
 * @return : 
* 	MAP_NULL_ARGUMENT if a Players List is null
* 	MAP_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
* 	MAP_SUCCESS the paired elements had been inserted successfully
 **/
static MapResult addPlayer (Map players_list,int* player_id)
{
    MapResult result = MAP_SUCCESS;
    if(!players_list)
    {
        return MAP_NULL_ARGUMENT;
    }
    if(!mapContains(players_list,(MapKeyElement)player_id)) //enter only if player not exist
    {
        Player player= malloc(sizeof(*player));
        if(!player)
        {
            return MAP_OUT_OF_MEMORY;
        }
        player->id_player = *player_id;
        player->draws=0;
        player->loses=0;
        player->total_game_time = 0;
        player->wins=0;
        result = mapPut(players_list,(MapKeyElement)player_id,player);
        freePlayerData(player);
    }
    return result;
 }

/**
 * updatePlayerScore void function that update game resualt in player profile.
 * @param player - that need to update - non-null
 * @param player_side - 0 if first player, 1 if second
 * @param game_result - 0 if first player wins, 1 if second player, 2 if draw
 * @param add_score = 1 if need to add point, -1 of need to remove point
 * @param game_time - time of the game (int seconds), 0 if no need to add time (in change game result)
 *  
*/
static void updatePlayerScore (Player player,PlayerSide player_side,Winner game_result,int add_score,int game_time)
 {
     if((int)player_side == (int)game_result)
     {
         player->wins+=add_score;
     }
     else
     {
        if(game_result==DRAW)
        {
         player->draws +=add_score;
        }
        else
        {
            player->loses +=add_score;
        }
     }
     player->total_game_time +=(game_time*add_score);
 }

/**
 * updateTournamentPlayers - void function that update number of players in specific tournament.
 *                                             
 * @param tournament - specific tournament to update in - Non-Null.
 * @param first_player - first player id - Positive number
 * @param second_player -second player id - Positive number
 * @param edit_command - 1 if you want to add number of players , -1 if you want to remove number of players
 *  
*/
static void updateTournamentPlayers(Tournament tournament,int first_player,int second_player,int edit_command)
{
    if(getPlayerNumberOfGames(tournament,first_player)==1) // if just
    {
        tournament->number_of_players+=edit_command;
    }
    if(getPlayerNumberOfGames(tournament,second_player)==1)
    {
        tournament->number_of_players+=edit_command;
    }
}
/**
 * generateGameId: generate game id, the function looking for free number to assaing to new game
 * @param chess - chess system that contains the tournament. Must be non-NULL.
 * @return - free integer that present game id 
 *
 **/
static int generateGameId(ChessSystem chess)
{
    int id = 1;
    bool find_free_id = false;
    Map table = chess->chess_tournament_table;
    MapKeyElement tournament_id;
    Tournament tournament;
    while(!find_free_id)
    {
        find_free_id = true;
        tournament_id= mapGetFirst(table);
        tournament= (Tournament)mapGet(table,tournament_id);
        freeTournamentId(tournament_id);
        for(int i=0;i<mapGetSize(table);i++) // check in all tornaments if the number id is already taken
        {
            if(mapContains(tournament->games,(MapKeyElement)&id))
            {
                find_free_id = false;
                id++;
                break;
            }
            tournament_id =mapGetNext(table);
            tournament = (Tournament)mapGet(table,tournament_id);
            freeTournamentId(tournament_id);
        }
    }

    return id;
}
/**
 * chessRemoveTournament: removes the tournament and all the games played in it from the chess system.
 *
 * @param chess - chess system that contains the tournament. Must be non-NULL.
 * @param tournament_id - the tournament id. Must be non-negative, and unique.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the tournament ID number is invalid.
 *     CHESS_TOURNAMENT_NOT_EXIST - if the tournament does not exist in the system.
 *     CHESS_SUCCESS - if tournament was removed successfully.
 */
ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if(!chess)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id<=0)
    {
        return CHESS_INVALID_ID;
    }
    Tournament tournament = (Tournament) mapGet(chess->chess_tournament_table,(MapKeyElement)&tournament_id);
    if(!tournament)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    MapKeyElement game_id = mapGetFirst(tournament->games);
    ChessGame game = (ChessGame)mapGet(tournament->games,game_id);
    Player player;
    while(game) //remove data from players
    {
        player = (Player)mapGet(chess->players_list,(MapKeyElement)&(game->first_player_id));
        if(player)//if player exist
        {
            updatePlayerScore(player,FIRST_PLAYER,game->game_result,REMOVE_POINT,game->game_time);
        }
        player = (Player)mapGet(chess->players_list,(MapKeyElement)&(game->second_player_id));
        if(player)//if player exist
        {
            updatePlayerScore(player,SECOND_PLAYER,game->game_result,REMOVE_POINT,game->game_time);
        }
        freeGameId(game_id);
        game_id = mapGetNext(tournament->games);
        game=(ChessGame)mapGet(tournament->games,game_id);
    }
    if(mapRemove(chess->chess_tournament_table,(MapKeyElement)&tournament_id)==MAP_SUCCESS)
    {
        return CHESS_SUCCESS;
    }
    return CHESS_INVALID_ID; //only if something wrong happend
}

/**
 * chessRemovePlayer: removes the player from the chess system.
 *                      In games where the player has participated and not yet ended,
 *                      the opponent is the winner automatically after removal.
 *
 * @param chess - chess system that contains the player. Must be non-NULL.
 * @param player_id - the player id. Must be non-negative.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the player ID number is invalid.
 *     CHESS_PLAYER_NOT_EXIST - if the player does not exist in the system.
 *     CHESS_SUCCESS - if player was removed successfully.
 */
ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    PlayerSide player_side;
    if(!chess)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(player_id<=0)
    {
        return CHESS_INVALID_ID;
    }
    if(!mapContains(chess->players_list,(MapKeyElement)&player_id))
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    MapKeyElement tournament_id = mapGetFirst(chess->chess_tournament_table);
    Tournament tournament = (Tournament)mapGet(chess->chess_tournament_table,tournament_id);
    while(tournament)
    {
        if(tournament->in_progress)
        {
            MapKeyElement game_id = mapGetFirst(tournament->games);
            ChessGame game = (ChessGame)mapGet(tournament->games,game_id);
            freeGameId(game_id);
            Player player=NULL;
            while(game)
            {
                if(game->first_player_id == player_id)
                {
                    player = (Player)mapGet(chess->players_list,(MapKeyElement)&game->second_player_id);
                    player_side = SECOND_PLAYER;
                    game->first_player_deleted = true;
                }
                if(game->second_player_id==player_id)
                {
                    player = (Player)mapGet(chess->players_list,(MapKeyElement)&game->first_player_id);
                    player_side = FIRST_PLAYER;
                    game->second_player_deleted = true;
                }
                if(player) // only if the player_id exist in game and player is lose/draw
                {
                    updatePlayerScore (player,player_side,game->game_result,REMOVE_POINT,0);
                    game->game_result = (Winner)player_side;
                    updatePlayerScore (player,player_side,game->game_result,ADD_POINT,0);
                }
                game_id = mapGetNext(tournament->games);
                game=(ChessGame)mapGet(tournament->games,game_id);
                freeGameId(game_id);
                player=NULL;
            }
        }
        freeTournamentId(tournament_id);
        tournament_id =mapGetNext(chess->chess_tournament_table);
        tournament = (Tournament)mapGet(chess->chess_tournament_table,tournament_id);
    }
    //remove player from data
    if(mapRemove(chess->players_list,(MapKeyElement)(&player_id)) == MAP_SUCCESS)
    {
        return CHESS_SUCCESS;
    }
    return CHESS_NULL_ARGUMENT;
}

/**
 * chessCalculateAveragePlayTime: the function returns the average playing time for a particular player.
 *
 * @param chess - a chess system that contains the player. Must be non-NULL.
 * @param player_id - player ID. Must be non-negative.
 * @param chess_result - this variable will contain the returned error code.
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the player ID number is invalid.
 *     CHESS_PLAYER_NOT_EXIST - if the player does not exist in the system.
 *     CHESS_SUCCESS - if average playing time was returned successfully.
 */
double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if(!chess)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0; 
    }
    if (player_id <= 0)
    {
        *chess_result = CHESS_INVALID_ID;
        return 0;
    }
    Player player_card = (Player)(mapGet(chess->players_list , (MapKeyElement)&player_id));
    if(!player_card)
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return 0;

    }
    double games_played = (double)(player_card->wins) + (double)(player_card->loses) + (double)(player_card->draws);
    if(games_played!=0)
    {
        double average_time = (double)(player_card->total_game_time) / games_played;
        *chess_result = CHESS_SUCCESS;
        return average_time;
    }
    else // no games played.
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0;
    }
    
}

/**
 * createHistogra : create generic new histogram and put zeros, 
 *
 * @param rows - integer present the how much rows in histogram
 * @param columns - integer present the how much columns in histogram
 * @param size - size of memory to allocate
 * @return
 *     histogram by size of row*columns by size.
 *     Null - if memory calloc is faild   
 */
static void** createHistogram(int rows, int columns,size_t size)
{
    void** histogram_points = calloc(rows,size);
    if(!histogram_points)
    {
        return NULL;
    }
    for(int i = 0;i<rows;i++) 
    {
        histogram_points[i] = calloc(columns, size);
        if(!histogram_points[i])
        {
            return NULL;
        }   
    }
    return histogram_points;
}
/**
 * freeHistogram : free histogram
 * @param histogram - integer present the how much columns in histogram
 * @param rows - integer present the how much rows in histogram
 *     
 *    
 */
static void freeHistogram(void** histogram,int rows)
{
    for(int i =rows-1;i>=0;i--)
    {
        free(histogram[i]);
    }
    free(histogram);
}

/**
 * chessEndTournament: The function will end the tournament and calculate the id of the winner.
 *                     The winner of the tournament is the player with the highest score.
 *                     Once the tournament is over, no games can be added for that tournament.
 *
 * @param chess - chess system that contains the tournament. Must be non-NULL.
 * @param tournament_id - the tournament id. Must be non-negative, and unique.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_INVALID_ID - if the tournament ID number is invalid.
 *     CHESS_TOURNAMENT_NOT_EXIST - if the tournament does not exist in the system.
 *     CHESS_SUCCESS - if tournament was ended successfully.
 */
ChessResult chessEndTournament (ChessSystem chess, int tournament_id)
{
    if(!chess)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id<=0)
    {
        return CHESS_INVALID_ID;
    }
    Tournament end_tournament = mapGet(chess->chess_tournament_table , (MapKeyElement)&(tournament_id));
    if(!end_tournament)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if(!end_tournament->in_progress)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    if(!mapGetSize(end_tournament->games))
    {
        return CHESS_NO_GAMES;
    }

    int lenght_of_row_histogram = (PLAYERS_EACH_GAME * mapGetSize(end_tournament->games));
    int** histogram_points = (int**)createHistogram(ROWS, lenght_of_row_histogram,sizeof(int*));
    if(!histogram_points)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    Map ptr_to_game = end_tournament->games;
    MapKeyElement current_game_id = mapGetFirst(ptr_to_game);
    ChessGame current_game = (ChessGame)mapGet(ptr_to_game,current_game_id);
    int column=0; // == point to column thta present a player in the tournaments    
    while(current_game)
    {
        if(!current_game->first_player_deleted)
        {
            column = playerExistInPointTable(histogram_points, current_game->first_player_id , lenght_of_row_histogram);
            UpdateHistogramPlayer (histogram_points,current_game,column,PLAYER_ONE,current_game->first_player_id);
        }
        if(!current_game->second_player_deleted)
        {
            column = playerExistInPointTable(histogram_points, current_game->second_player_id , lenght_of_row_histogram);
            UpdateHistogramPlayer (histogram_points,current_game,column,PLAYER_TWO,current_game->second_player_id);
        }
        freeGameId(current_game_id);
        current_game_id = mapGetNext(ptr_to_game);
        current_game=(ChessGame)mapGet(ptr_to_game,current_game_id);
    }
    end_tournament->winner_id = checkWinner(histogram_points ,lenght_of_row_histogram);
    end_tournament->in_progress = false;
    freeHistogram((void**)histogram_points,ROWS);
    return CHESS_SUCCESS;
}

/**
 * UpdateHistogramPlayer: Update player histogram by result of the specific game and insert him to histogram
 *                      
 * @param histogram_points - histogram that present all the players in the tournament and the result of the games
 * @param game - the game to take a details from
 * @param player_position - index the player in histogram
 * @param player_side - which side the player in player position is in the game
 * @param player_id - id of the player
 */
static void UpdateHistogramPlayer (int ** histogram_points,ChessGame game,int player_position,PlayerSide player_side,int player_id)
{
    histogram_points[PLAYER_ID][player_position] = player_id;
    if ((int)game->game_result != (int)player_side)//if player not win
        {
            if(game->game_result == DRAW) // draw
            {
                histogram_points[PLAYER_POINTS][player_position] += POINTS_FOR_DRAW;
                
            }
            else //lose
            {
                histogram_points[PLAYER_LOSES][player_position]++;
            }
        
        }
        else // player wins
        {
            histogram_points[PLAYER_WINS][player_position]+=1;
            histogram_points[PLAYER_POINTS][player_position] += POINTS_FOR_WIN;
        }
}

/**
 * playerExistInPointTable: looking player id in hisogram, and return index in histogram or free place to insert him.
 *                      
 * @param histogram_points - histogram that present all the players in the tournament and the result of the games
 * @param player_id - id of the player we looking for
 * @param lenght_of_row_histogram - the bounderies of the histogram
 * 
 * @return 
 *      if player exist the function return player's index in histogram
 *      if player not exist the function return free index to place him in the histogram
 */
static int playerExistInPointTable(int **histogram_points  ,int player_id , int lenght_of_row_histogram)
{
    int i=0;
    while(histogram_points[PLAYER_ID][i] != 0 && i<lenght_of_row_histogram) // not possible - case of out of bounderies
    {
        if (histogram_points[PLAYER_ID][i] == player_id)
        {
            return i;
        }
        else
        {
            i++;
        }
    }
    return i;
}

  /**
 * checkWinner: Find index of the winner of the tournament.
 *              The winner of the tournament is the player with the highest score.
 *                      
 * @param histogram_points - histogram that present all the players in the tournament and the result of the games
 * @param arr_size - the bounderies of the histogram
 * 
 * @return - player's id who wins the tournament
 */  
static int checkWinner(int** histogram_points , int arr_size)
{
    int playerWithMaxPoints=0;
    int winnerGamesLost=0;
    int winnerGamesWin=0;
    int maxPoints=0;
    int i=0;
    for(; i<arr_size ; i++)
    {
        if(histogram_points[PLAYER_POINTS][i]>maxPoints) // update the current winner
        {
            playerWithMaxPoints = i;
            maxPoints = histogram_points[PLAYER_POINTS][i];
            winnerGamesLost = histogram_points[PLAYER_LOSES][i];
            winnerGamesWin = histogram_points[PLAYER_WINS][i];
            continue;
        }
        if(histogram_points[PLAYER_POINTS][i]==maxPoints) // if there another player with same points
        {
            if(histogram_points[PLAYER_LOSES][i]<winnerGamesLost) //the current winner is the one with less loses
            {
                playerWithMaxPoints = i;
                winnerGamesLost = histogram_points[PLAYER_LOSES][i];
            }
            else // if same game amout of game lost
                {
                    if(histogram_points[PLAYER_LOSES][i]==winnerGamesLost &&histogram_points[PLAYER_WINS][i] > winnerGamesWin) 
                    {
                        playerWithMaxPoints = i;
                        winnerGamesLost = histogram_points[PLAYER_LOSES][i];
                        winnerGamesWin = histogram_points[PLAYER_WINS][i];
                    }
                    else // same amount of game lost but not more winnings games
                    {
                        if(histogram_points[PLAYER_LOSES][i]==winnerGamesLost 
                            && histogram_points[PLAYER_WINS][i]==winnerGamesWin) //same amout of win and same amout of lose
                        {
                            if(histogram_points[PLAYER_ID][i] < histogram_points[PLAYER_ID][playerWithMaxPoints])// winner == smaller id player
                                {
                                playerWithMaxPoints = i;
                                winnerGamesLost = histogram_points[PLAYER_LOSES][i];
                                winnerGamesWin = histogram_points[PLAYER_WINS][i];
                                }
                        }

                    }

                }

        }
    }
    return histogram_points[PLAYER_ID][playerWithMaxPoints];
}

/**
 * chessSavePlayersLevels: prints the rating of all players in the system as
 * explained in the *.pdf
 *
 * @param chess - a chess system. Must be non-NULL.
 * @param file - an open, writable output stream, to which the ratings are printed.
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_SAVE_FAILURE - if an error occurred while saving.
 *     CHESS_SUCCESS - if the ratings was printed successfully.
 */
ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    double level,max_level,num_of_games;
    int i=0,max_level_index=0;
    bool first_time =true;
    if(!chess||!file)
    {
        return CHESS_NULL_ARGUMENT;
    }
    MapKeyElement player_id = mapGetFirst(chess->players_list);
    Player player =mapGet(chess->players_list,player_id);
    int num_of_players = (mapGetSize(chess->players_list)); //num of players
    double** histogram_points = (double**)createHistogram(ROW_LEVELS, num_of_players,sizeof(double*));
    if(!histogram_points)
    {
        return CHESS_OUT_OF_MEMORY;
    }  // first raw is ID, and second raw in level
    while(player)//insert player to histogram
    {
        num_of_games = (double)player->wins+(double)player->loses+(double)player->draws;
        if(num_of_games)// check if player play any game
        {
            level = (((double)(player->wins*LEVEL_WIN_VALUE)-(double)(player->loses*LEVEL_LOSE_VALUE)
                    +(double)(player->draws*LEVEL_DRAW_VALUE))/num_of_games);
            if(first_time)
            {
                max_level = level;
                first_time = false;
            }
            if(max_level<level)
            {
                max_level = level;
                max_level_index = i;
            }
            histogram_points[PLAYER_ID][i] = (double)player->id_player;
            histogram_points[PLAYER_POINTS][i] = level;
            i++;
        }
        freePlayerId(player_id);
        player_id = mapGetNext(chess->players_list);
        player =mapGet(chess->players_list,player_id);
    }  
    num_of_players = i;//not inclue players with 0 games
    ChessResult result = printLevelsToFile(file,histogram_points,num_of_players,max_level,max_level_index);
    freeHistogram((void**)histogram_points,ROW_LEVELS);
    return result;
}


/**
 * printLevelsToFile: insert the rating of all players in the system as
 * explained in the *.pdf
 *
 * @param file - an open, writable output stream, to which the ratings are printed.
 * @param histogram_points - histogram that contain all the details of all the players
 * @param num_of_players - number of player that played in chess system
 * @param max_level - present the  highest level value in histogram
 * @param max_level_index - index of the highest level in histogram
 * @return
 *     CHESS_SAVE_FAILURE - if an error occurred while saving.
 *     CHESS_SUCCESS - if the ratings was printed successfully.
 */
static ChessResult printLevelsToFile (FILE* file,double **histogram_points,int num_of_players,double max_level,int max_level_index)
{
    int counter_printed_players=1;
    while(counter_printed_players<=num_of_players)  
    {
        if(checkSamePointInHistogram(histogram_points,(int)num_of_players,max_level,max_level_index)) //if more players with same level
        {
            for(int j=0;j<num_of_players;j++)
            {
                if(histogram_points[PLAYER_ID][j] && histogram_points[PLAYER_POINTS][j]==max_level) // check if same level
                {
                    counter_printed_players++;
                    if(fprintf(file,"%.0f %.2f\n",histogram_points[PLAYER_ID][j],histogram_points[PLAYER_POINTS][j])<=0)
                    {
                        return CHESS_SAVE_FAILURE;
                    }
                }
            }
        }
        else // only one with current level
        {
            counter_printed_players++;
            if(fprintf(file,"%.0f %.2f\n",histogram_points[PLAYER_ID][max_level_index],histogram_points[PLAYER_POINTS][max_level_index])<=0)
            {
                return CHESS_SAVE_FAILURE;
            }
        }
        if(counter_printed_players<=num_of_players&&num_of_players!=1)
        {
            max_level_index = indexOfSmallClosesNumber(histogram_points,num_of_players,max_level);
            max_level = histogram_points[PLAYER_POINTS][max_level_index];//update the next smaller level;
        }
    }
    return CHESS_SUCCESS;
}

/**
 * indexOfSmallClosesNumber: return int that present the next index in histogram that most close
 *
 * @param chess - a chess system. Must be non-NULL.
 * @param file - an open, writable output stream, to which the ratings are printed.
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_SAVE_FAILURE - if an error occurred while saving.
 *     CHESS_SUCCESS - if the ratings was printed successfully.
 */
static int indexOfSmallClosesNumber(double **histogram,int histogram_size,double number)
{
    bool first_time = true;
    double second_max_number=0;
    int index;
    for(int i=0;i<histogram_size;i++)
    {
        if(histogram[PLAYER_POINTS][i]<number &&(histogram[PLAYER_POINTS][i]>second_max_number||first_time))
        {
            first_time = false;
            second_max_number = histogram[PLAYER_POINTS][i];
            index = i;
            
        }
    }
    return index;
}

/**
 * checkSamePointInHistogram: search if there another player with same points like player in the index
 *
 * @param histogram - histogram that contain all the details of all the players
 * @param histogram_size - bounderies of the histogram - num of lenght of rows
 * @param level_of_current_max - present the  highest level value in histogram
 * @param index_of_current_max - index of the highest level in histogram
 * @return
 *     True - if there another player with same level
 *     False - there isn't another player with same level
 */
static bool checkSamePointInHistogram (double **histogram,int histogram_size,double level_of_current_max,int index_of_current_max)
{
        for(int i=0;i<histogram_size;i++)
    {
        if(histogram[PLAYER_POINTS][i]==level_of_current_max && index_of_current_max!=i)
        {
            return true;
        }
    }
    return false;
}

/**
 * chessSaveTournamentStatistics: prints to the file the statistics for each tournament that ended as
 * explained in the *.pdf
 *
 * @param chess - a chess system. Must be non-NULL.
 * @param path_file - the file path which within it the tournament statistics will be saved.
 * @return
 *     CHESS_NULL_ARGUMENT - if chess is NULL.
 *     CHESS_NO_TOURMENTS_ENDED - if there are no tournaments ended in the system.
 *     CHESS_SAVE_FAILURE - if an error occurred while saving.
 *     CHESS_SUCCESS - if the ratings was printed successfully.
 */
ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if(!chess)
    {
        return CHESS_NULL_ARGUMENT;
    }
    Map tournaments_list = chess->chess_tournament_table;
    MapKeyElement current_tournament_id = NULL;
    Tournament tournament = NULL;
    if(!checkIfAnyTournamentEnded(chess))
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    FILE *file_ptr = fopen(path_file, "w"); //creating a txt file in the specific path
    if(!file_ptr)
    {
        return CHESS_SAVE_FAILURE;
    }
    int number_of_tournaments = mapGetSize(tournaments_list),counter_num_of_tournaments = number_of_tournaments;
    while(counter_num_of_tournaments > 0)
    {
        if(counter_num_of_tournaments == number_of_tournaments)
        {
             current_tournament_id = mapGetFirst(tournaments_list);
        }
        else
        {
            current_tournament_id = mapGetNext(tournaments_list);
        }
        tournament = (Tournament)mapGet(tournaments_list, current_tournament_id);
        if(tournament->in_progress == false && mapGetSize(tournament->games)>0) // Deals only with tournaments which already over
        {
            if(printTournamentsStatisticsToFile (file_ptr,tournament)!=CHESS_SUCCESS) // print to the file, enter if something wrong
            {
                freeTournamentId(current_tournament_id);
                return CHESS_SAVE_FAILURE;
            }   
        }
        freeTournamentId(current_tournament_id);
        counter_num_of_tournaments--;
    }
    fclose(file_ptr);
    return CHESS_SUCCESS;
}

/**
 * printTournamentsStatisticsToFile: insert tournaments statistics to file by unique way
 * explained in the *.pdf
 *
 * @param file - an open, writable output stream, to which the statistics are printed.
 * @param tournament - specific tournament to print the statistics in the file
 * @return
 *     CHESS_SAVE_FAILURE - if an error occurred while saving, and close to file
 *     CHESS_SUCCESS - if the statistics was printed successfully.
 */
static ChessResult printTournamentsStatisticsToFile (FILE* file,Tournament tournament)
{
    double ptr_average_game_time_in_tournament;

    //fprintf to the file
    if(
        fprintf(file , "%d\n" , tournament->winner_id) <0 ||
        fprintf(file , "%d\n" , longestGameTimeAndAverage(tournament,&ptr_average_game_time_in_tournament))  <0 ||
        fprintf(file , "%.2f\n", ptr_average_game_time_in_tournament) <0 ||
        fprintf(file , "%s\n" , tournament->tournament_location) <0 ||
        fprintf(file , "%d\n" , mapGetSize(tournament->games)) <0 ||
        fprintf(file , "%d\n" , tournament->number_of_players) <0 )
    {
        fclose(file);
        return CHESS_SAVE_FAILURE;
    }
    return CHESS_SUCCESS;
}


/**
 * longestGameTimeAndAverage: Calculate the avarage game time and the longest game time and tournament
 *
 * @param tournament - specific tournament to calculate the times from
 * @param ptr_average_game_time_in_tournament - double pointer that you want to update the value of avarage game
 * @return
 *     integer - the time of the longest game.
 */
static int longestGameTimeAndAverage (Tournament tournament, double* ptr_average_game_time_in_tournament)
{
    MapKeyElement ptr_to_current_game = mapGetFirst(tournament->games);
    ChessGame game = (ChessGame)mapGet(tournament->games , ptr_to_current_game);
    freeGameId(ptr_to_current_game);
    int longest_game=0;
    double total_time_of_tournament = 0;
    
    int games_in_tournament = mapGetSize(tournament->games);
    if(game->game_time > longest_game)
    {
        longest_game = game->game_time;
        games_in_tournament--;
    }
    total_time_of_tournament += (double)game->game_time;

    while(games_in_tournament>0)
    {
        ptr_to_current_game = mapGetNext(tournament->games);
        game = (ChessGame)mapGet(tournament->games, ptr_to_current_game);
        if(game->game_time > longest_game)
        {
            longest_game = game->game_time;
        }
        total_time_of_tournament += (double)game->game_time;
        games_in_tournament--;
        freeGameId(ptr_to_current_game);
    }
    *ptr_average_game_time_in_tournament = (total_time_of_tournament/(double)mapGetSize(tournament->games));

    return longest_game;
}

/**
 * checkIfAnyTournamentEnded: checks if there is at least one tournament which ended.
 *
 * @param chess - Chess System the contation all tournaments
 * @return
 *     True - there at least one tournament that ended
 *     False - All the tournament in progress (if exist)
 */
static bool checkIfAnyTournamentEnded(ChessSystem chess)
{
    Map tournaments_list = chess->chess_tournament_table;
    MapKeyElement current_tournament = NULL;
    Tournament tournament = NULL;
    int number_of_tournaments = mapGetSize(tournaments_list);
    int counter_num_of_tournaments = number_of_tournaments;
    while(counter_num_of_tournaments>0)
    {
        if(counter_num_of_tournaments == number_of_tournaments)
        {
             current_tournament = mapGetFirst(tournaments_list);
             counter_num_of_tournaments--;
        }
        else
        {
            current_tournament = mapGetNext(tournaments_list);
            counter_num_of_tournaments--;
        }
        tournament = (Tournament)mapGet(tournaments_list, current_tournament);
        if(tournament->in_progress == false) // Deals only with tournaments which already over
        {
            freeTournamentId(current_tournament);
            return true;
        }
        freeTournamentId(current_tournament);
    }
    
    return false;
}

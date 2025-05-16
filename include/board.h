#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED


#include <stdbool.h>

#define EMPTY 0
#define player1 1
#define player2 2
#define player3 3


typedef struct board {
    int cell[24]; // is it empty or occupied
    int turn; // player 1 or player 2 or player3
}board_t;

typedef struct player {
    char player_name[50];
    int ID;
    int number_piece;
}player_t;

player_t players[3];
board_t game_board;

extern const int millstable[16][3];
extern const int adjacentCells[24][4];

void initialize_board(board_t* game); /* initialize the board and the player will start is player 1 red. */
void print_board();
int place_piece(board_t* game, char* name,int player);
int is_validemove(board_t* game, int player); // will work when you want to change your place
void remove_piece(int what, int player);
int check_mill(board_t* game, int cell_id);
void switch_turn(board_t *game);
void playervsplayer();
int is_adjacent(int source, int destination);
void player_turn(board_t *game, char* player_name, int pieces_count);
int no_adjacents(board_t *game, int player);
int move_anywhere(board_t *game);
int all_mills(board_t *board, int player);
void is_winner(int winner);

#endif // BOARD_H_INCLUDED

#ifndef ALGORITHME_H_INCLUDED
#define ALGORITHME_H_INCLUDED
#include <time.h>
#define DEPTH 3



typedef struct {
    int from;
    int to;
    int evaluation;
} Move;

typedef struct {
    Move nextMaxMove;
    Move nextMinMove;
} Minimax;

/* Random */

void switch_turn_algo(board_t* board);
void player_vs_random();

/* Minimax  */

// Forward declarations
int alphabeta(board_t* board, int depth, int alpha, int beta, int maximizing, int phase);
int evaluate_opening(board_t* board);
int evaluate_midgame(board_t* board);
int evaluate_endgame(board_t* board);
int count_potential_mills(board_t* board, int player);
int count_blocked_pieces(board_t* board, int player);
int count_pieces(board_t* board, int player);
int count_double_mills(board_t* board, int player);
int count_possible_moves(board_t* board, int player);
Move get_best_move(board_t* board, int phase);
int get_piece_count(board_t* board, int player);
void player_vs_minimax();

#endif // ALGORITHME_H_INCLUDED

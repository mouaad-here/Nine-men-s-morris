#include "../include/board.h"
#include "../include/color.h"
#include "../include/algorithme.h"



void switch_turn_algo(board_t* board)
{
    board->turn = (board->turn == 1) ? 3 : 1; // Switch between Player 1 and Player 2
}

void player_turn_algo(board_t *game, char* name, int pieces_count)
{
    int cellid;
    cellid = place_piece(game, name, game->turn);
    print_board(game);
    if (pieces_count >= 5) {
        remove_piece(check_mill(&game_board, cellid), (game->turn == 1 ? 3 : 1));
    }
    switch_turn_algo(&game_board);

}

/* Random */
void player_vs_random()
{
    srand(time(NULL));

    int pieces_placed = 0, winner = 3, numberdestination;

    initialize_board(&game_board);

    printf("enter your name: \n");
    printf("player 1: ");
    scanf("%s", players[0].player_name);
    players[0].ID = 1;

    // random player
    strcpy(players[2].player_name, "random");
    players[2].ID = 3;

    players[0].number_piece = 0;
    players[2].number_piece = 0;

    /* Phase 1: Placing pieces */

    while (pieces_placed < 18 ){

        print_board(&game_board);
        color(14, 0);
        printf("Phase 1: Placing pieces\n");
        printf("pieces counter : %d pieces\n", pieces_placed);
        color(4, 0);
        printf("player %s : %d\n ", players[0].player_name, players[0].number_piece);
        color(2, 0);
        printf("player %s : %d\n", players[2].player_name, players[2].number_piece);
        pieces_placed++;

        if (game_board.turn == 1) {
            player_turn_algo(&game_board, players[0].player_name, pieces_placed);
            players[0].number_piece++;
        }else {
            player_turn_algo(&game_board, players[2].player_name, pieces_placed);
            players[2].number_piece++;
        }

    }

    /* Phase 2: Moving pieces */

    while (players[0].number_piece > 3 && players[2].number_piece > 3 ) {

        print_board(&game_board);
        color(14, 0);
        printf("Phase 2: Moving pieces\n");
        color(4, 0);
        printf("player %s : %d piece\n ", players[0].player_name, players[0].number_piece);
        color(2, 0);
        printf("player %s : %d piece\n", players[2].player_name, players[2].number_piece);
        color(15, 0);
        printf("Turn of %s \n ", players[game_board.turn - 1].player_name);

        /* check if they have no adjacent points to move to */
        if (no_adjacents(&game_board, game_board.turn)) {
            winner = (game_board.turn == 1 ? 3 : 1) - 1;
            is_winner(winner);
            break;

        }

        numberdestination = is_validemove(&game_board, game_board.turn);
        print_board(&game_board);
        remove_piece(check_mill(&game_board, numberdestination), (game_board.turn == 1 ? 3 : 1));
        switch_turn_algo(&game_board);

    }
   /* Phase 3: Flying */

    while(winner != 0 && winner != 2){


        if (players[0].number_piece == 2) {
            winner = 2;
            is_winner(winner);
            break;
        }else if (players[2].number_piece == 2) {
            winner = 0;
            is_winner(winner);
            break;
        }

        // because we sort from the phase 2 when we switch turn and the condition not verified with that player
        print_board(&game_board);
        color(14, 0);
        printf("Phase 3: Flying \n");
        color(4, 0);
        printf("player %s : %d piece\n ", players[0].player_name, players[0].number_piece);
        color(2, 0);
        printf("player %s : %d piece\n", players[2].player_name, players[2].number_piece);
        color(15, 0);
        printf("Turn of %s \n ", players[game_board.turn - 1].player_name);

        /* check if they have no adjacent points to move to */
        if (no_adjacents(&game_board, game_board.turn)) {
            winner = (game_board.turn == 1 ? 3 : 1) - 1;// that mean players[0 or 1].player_name or the other
            is_winner(winner);
            break;
        }

        /*here we should check for both players because maybe have the case where the two player have 3 pieces. */
        if (players[game_board.turn -1].number_piece == 3) {
            numberdestination = move_anywhere(&game_board);
            /* u can  add print board here for more visibility*/
            remove_piece(check_mill(&game_board, numberdestination), (game_board.turn == 1 ? 3 : 1));
        }else {
            numberdestination = is_validemove(&game_board, game_board.turn);
            remove_piece(check_mill(&game_board, numberdestination), (game_board.turn == 1 ? 3 : 1));
        }
        print_board(&game_board);
        switch_turn_algo(&game_board);

    }




}


#define INFINITY 1000000
#define NEG_INFINITY -1000000

int ai_make_move(board_t* board, int phase) {
    Move best_move = get_best_move(board, phase);

    if (phase == 1) {
        board->cell[best_move.to] = 3;
        return best_move.to;
    } else {
        board->cell[best_move.from] = EMPTY;
        board->cell[best_move.to] = 3;
        return best_move.to;
    }
}

int evaluate_position(board_t* board, int phase) {
    switch(phase) {
        case 1: return evaluate_opening(board);
        case 2: return evaluate_midgame(board);
        case 3: return evaluate_endgame(board);
        default: return 0;
    }
}
int evaluate_opening(board_t* board) {
    int ai_pieces = get_piece_count(board, 2);
    int human_pieces = get_piece_count(board, 1);
    int potential_mills = count_potential_mills(board, 2) - count_potential_mills(board, 1);

    return (ai_pieces - human_pieces) * 10 + potential_mills * 5;
}

int evaluate_midgame(board_t* board) {
    int ai_pieces = get_piece_count(board, 2);
    int human_pieces = get_piece_count(board, 1);

    if (human_pieces <= 2) return INFINITY;
    if (ai_pieces <= 2) return NEG_INFINITY;

    int piece_diff = (ai_pieces - human_pieces) * 100;
    int mobility = count_possible_moves(board, 2) - count_possible_moves(board, 1);
    int blocked = count_blocked_pieces(board, 1) - count_blocked_pieces(board, 2);
    int double_mills = count_double_mills(board, 2) - count_double_mills(board, 1);

    return piece_diff + mobility * 5 + blocked * 3 + double_mills * 10;
    // we could separate the evaluation formula not a single formula
}
int evaluate_endgame(board_t* board) {
    int ai_pieces = get_piece_count(board, 2);
    int human_pieces = get_piece_count(board, 1);

    if (human_pieces <= 2) return INFINITY;
    if (ai_pieces <= 2) return NEG_INFINITY;
    if (ai_pieces == 3) {
        return evaluate_midgame(board) + 500;
    }

    return evaluate_midgame(board);
}
int alphabeta(board_t* board, int depth, int alpha, int beta, int maximizing, int phase) {
    if (depth == 0) {
        return evaluate_position(board, phase);
    }

    if (maximizing) {
        int value = NEG_INFINITY;

        // Generate possible moves each phase
        if (phase == 1) {
            for (int i = 0; i < 24; i++) {
                if (board->cell[i] == EMPTY) {
                    board->cell[i] = 2;
                    if (check_mill(board, i)) {
                        for (int j = 0; j < 24; j++) {
                            if (board->cell[j] == 1 && !check_mill(board, j)) {
                                int temp = board->cell[j];
                                board->cell[j] = EMPTY;
                                value = max(value, alphabeta(board, depth - 1, alpha, beta, 0, phase));
                                board->cell[j] = temp;
                                alpha = max(alpha, value);
                                if (beta <= alpha) break;
                            }
                        }
                    } else {
                        value = max(value, alphabeta(board, depth - 1, alpha, beta, 0, phase));
                    }
                    board->cell[i] = EMPTY;
                    alpha = max(alpha, value);
                    if (beta <= alpha) break;
                }
            }
        } else { // Movement o Flying phase
            int piece_count = get_piece_count(board, 3);
            for (int i = 0; i < 24; i++) {
                if (board->cell[i] == 3) {
                    for (int j = 0; j < 24; j++) {
                        if (board->cell[j] == EMPTY) {
                            if (phase == 3 || piece_count == 3 || is_adjacent(i, j)) {
                                board->cell[i] = EMPTY;
                                board->cell[j] = 3;
                                if (check_mill(board, j)) {
                                    for (int k = 0; k < 24; k++) {
                                        if (board->cell[k] == 1 && !check_mill(board, k)) {
                                            int temp = board->cell[k];
                                            board->cell[k] = EMPTY;
                                            value = max(value, alphabeta(board, depth - 1, alpha, beta, 0, phase));
                                            board->cell[k] = temp;
                                        }
                                    }
                                } else {
                                    value = max(value, alphabeta(board, depth - 1, alpha, beta, 0, phase));
                                }
                                board->cell[j] = EMPTY;
                                board->cell[i] = 3;
                                alpha = max(alpha, value);
                                if (beta <= alpha) break;
                            }
                        }
                    }
                }
            }
        }
        return value;
    } else {
        int value = INFINITY;

        if (phase == 1) {
            for (int i = 0; i < 24; i++) {
                if (board->cell[i] == EMPTY) {
                    board->cell[i] = 1;
                    if (check_mill(board, i)) {
                        for (int j = 0; j < 24; j++) {
                            if (board->cell[j] == 2 && !check_mill(board, j)) {
                                int temp = board->cell[j];
                                board->cell[j] = EMPTY;
                                value = min(value, alphabeta(board, depth - 1, alpha, beta, 1, phase));
                                board->cell[j] = temp;
                                beta = min(beta, value);
                                if (beta <= alpha) break;
                            }
                        }
                    } else {
                        value = min(value, alphabeta(board, depth - 1, alpha, beta, 1, phase));
                    }
                    board->cell[i] = EMPTY;
                    beta = min(beta, value);
                    if (beta <= alpha) break;
                }
            }
        } else {
            int piece_count = get_piece_count(board, 1);
            for (int i = 0; i < 24; i++) {
                if (board->cell[i] == 1) {
                    for (int j = 0; j < 24; j++) {
                        if (board->cell[j] == EMPTY) {
                            if (phase == 3 || piece_count == 3 || is_adjacent(i, j)) {
                                board->cell[i] = EMPTY;
                                board->cell[j] = 1;
                                if (check_mill(board, j)) {
                                    for (int k = 0; k < 24; k++) {
                                        if (board->cell[k] == 3 && !check_mill(board, k)) {
                                            int temp = board->cell[k];
                                            board->cell[k] = EMPTY;
                                            value = min(value, alphabeta(board, depth - 1, alpha, beta, 1, phase));
                                            board->cell[k] = temp;
                                        }
                                    }
                                } else {
                                    value = min(value, alphabeta(board, depth - 1, alpha, beta, 1, phase));
                                }
                                board->cell[j] = EMPTY;
                                board->cell[i] = 1;
                                beta = min(beta, value);
                                if (beta <= alpha) break;
                            }
                        }
                    }
                }
            }
        }
        return value;
    }
}
int get_piece_count(board_t* board, int player) {
    int count = 0;
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == player) count++;
    }
    return count;
}

int count_potential_mills(board_t* board, int player) {
    int count = 0;

    // Check  mill configuration
    for (int i = 0; i < 16; i++) {
        int empty_count = 0;
        int player_count = 0;

        for (int j = 0; j < 3; j++) {
            if (board->cell[millstable[i][j]] == EMPTY) empty_count++;
            else if (board->cell[millstable[i][j]] == player) player_count++;
        }

        if (player_count == 2 && empty_count == 1) count++;
    }

    return count;
}

int count_blocked_pieces(board_t* board, int player) {
    int count = 0;
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == player) {
            bool blocked = true;
            for (int j = 0; j < 4 && adjacentCells[i][j] != -1; j++) {
                if (board->cell[adjacentCells[i][j]] == EMPTY) {
                    blocked = false;
                    break;
                }
            }
            if (blocked) count++;
        }
    }
    return count;
}

int count_double_mills(board_t* board, int player) {
    int count = 0;
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == player) {
            int mill_count = 0;
            for (int j = 0; j < 16; j++) {
                if (millstable[j][0] == i || millstable[j][1] == i || millstable[j][2] == i) {
                    if (board->cell[millstable[j][0]] == player &&
                        board->cell[millstable[j][1]] == player &&
                        board->cell[millstable[j][2]] == player) {
                        mill_count++;
                    }
                }
            }
            if (mill_count > 1) count++;
        }
    }
    return count;
}

int count_possible_moves(board_t* board, int player) {
    int count = 0;
    int piece_count = get_piece_count(board, player);

    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == player) {
            if (piece_count == 3) {
                for (int j = 0; j < 24; j++) {
                    if (board->cell[j] == EMPTY) count++;
                }
            } else {
                for (int j = 0; j < 4 && adjacentCells[i][j] != -1; j++) {
                    if (board->cell[adjacentCells[i][j]] == EMPTY) count++;
                }
            }
        }
    }
    return count;
}

Move get_best_move(board_t* board, int phase) {
    Move best_move = {-1, -1, NEG_INFINITY};

    if (phase == 1) {
        // Placement phase
        for (int i = 0; i < 24; i++) {
            if (board->cell[i] == EMPTY) {
                board->cell[i] = 3;
                int eval = alphabeta(board, DEPTH, NEG_INFINITY, INFINITY, 0, phase);
                if (eval > best_move.evaluation) {
                    best_move.evaluation = eval;
                    best_move.to = i;
                }
                board->cell[i] = EMPTY;
            }
        }
    } else {
        // Movement/Flying phase
        int piece_count = get_piece_count(board, 2);
        for (int i = 0; i < 24; i++) {
            if (board->cell[i] == 3) {
                for (int j = 0; j < 24; j++) {
                    if (board->cell[j] == EMPTY) {
                        if (phase == 3 || piece_count == 3 || is_adjacent(i, j)) {
                            board->cell[i] = EMPTY;
                            board->cell[j] = 3;
                            int eval = alphabeta(board, DEPTH, NEG_INFINITY, INFINITY, 0, phase);
                            if (eval > best_move.evaluation) {
                                best_move.evaluation = eval;
                                best_move.from = i;
                                best_move.to = j;
                            }
                            board->cell[j] = EMPTY;
                            board->cell[i] = 3;
                        }
                    }
                }
            }
        }
    }

    return best_move;
}
void player_vs_minimax() {
    int pieces_placed = 0, winner = 3, numberdestination;

    initialize_board(&game_board);

    printf("Enter your name: ");
    scanf("%s", players[0].player_name);
    strcpy(players[1].player_name, "AI");
    players[0].ID = 1;
    players[1].ID = 3;
    players[0].number_piece = 0;
    players[1].number_piece = 0;
    while (pieces_placed < 18) {
        print_board(&game_board);
        color(14, 0);
        printf("Phase 1: Placing pieces\n");
        printf("pieces counter : %d pieces\n", pieces_placed);
        color(4, 0);
        printf("player %s : %d\n ", players[0].player_name, players[0].number_piece);
        color(2, 0);
        printf("player %s : %d\n", players[1].player_name, players[1].number_piece);
        pieces_placed++;

        if (game_board.turn == 1) {
            player_turn(&game_board, players[0].player_name, pieces_placed);
            players[0].number_piece++;
        } else {
            color(15, 0);
            printf("AI turn  \n");
            numberdestination = ai_make_move(&game_board, 1); // Phase 1
            if (pieces_placed >= 5) {
                print_board(&game_board);
                remove_piece(check_mill(&game_board, numberdestination), 1);
            }
            players[1].number_piece++;
            switch_turn(&game_board);
        }
    }
    while (players[0].number_piece > 3 && players[1].number_piece > 3) {
        print_board(&game_board);
        color(14, 0);
        printf("Phase 2: Moving pieces\n");
        color(4, 0);
        printf("player %s : %d piece\n ", players[0].player_name, players[0].number_piece);
        color(2, 0);
        printf("player %s : %d piece\n", players[1].player_name, players[1].number_piece);
        color(15, 0);
        printf("Turn of %s \n ", players[game_board.turn - 1].player_name);

        /*  no adjacent points to move to */
        if (no_adjacents(&game_board, game_board.turn)) {
            winner = (game_board.turn == 1 ? 2 : 1) - 1;
            is_winner(winner);
            break;
        }

        if (game_board.turn == 1) {
            numberdestination = is_validemove(&game_board, game_board.turn);
            print_board(&game_board);
            remove_piece(check_mill(&game_board, numberdestination), 3);
        } else {
            printf("AI turn  \n");
            numberdestination = ai_make_move(&game_board, 3);
            print_board(&game_board);
            remove_piece(check_mill(&game_board, numberdestination), 1);
        }
        switch_turn(&game_board);
    }
    while (winner != 0 && winner != 1) {
        if (players[0].number_piece == 2) {
            winner = 1;
            is_winner(winner);
            break;
        } else if (players[1].number_piece == 2) {
            winner = 0;
            is_winner(winner);
            break;
        }

        print_board(&game_board);
        color(14, 0);
        printf("Phase 3: Flying \n");
        color(4, 0);
        printf("player %s : %d piece\n ", players[0].player_name, players[0].number_piece);
        color(2, 0);
        printf("player %s : %d piece\n", players[1].player_name, players[1].number_piece);
        color(15, 0);
        printf("Turn of %s \n ", players[game_board.turn - 1].player_name);

        /* check no adjacent points to move to */
        if (no_adjacents(&game_board, game_board.turn)) {
            winner = (game_board.turn == 1 ? 3 : 1) - 1;
            is_winner(winner);
            break;
        }
        if (game_board.turn == 1) {
            if (players[0].number_piece == 3) {
                numberdestination = move_anywhere(&game_board);
            } else {
                numberdestination = is_validemove(&game_board, game_board.turn);
            }
            print_board(&game_board);
            remove_piece(check_mill(&game_board, numberdestination), 3);
        } else {
            printf("AI turn  \n");
            numberdestination = ai_make_move(&game_board, 3);
            print_board(&game_board);
            remove_piece(check_mill(&game_board, numberdestination), 1);
        }
        switch_turn(&game_board);
    }
}

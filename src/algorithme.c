#include "../include/board.h"
#include "../include/color.h"
#include "../include/algorithme.h"
#define INFINITY 1000000
#define NEG_INFINITY -1000000


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

        // check  no adjacent points to move to
        if (no_adjacents(&game_board, game_board.turn)) {
            winner = (game_board.turn == 1 ? 3 : 1) - 1;
            is_winner(winner);
            break;
        }
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


#define PIECE_WEIGHT 200            //  weight for pieces
#define MILL_WEIGHT 80              //  mill importance
#define DOUBLE_MILL_WEIGHT 150     // double mills
#define BLOCKED_OPPONENT_WEIGHT 60   //  blocking
#define MOBILITY_WEIGHT 40          // mobility importance
#define TRAP_SETUP_WEIGHT 70        //  trap setups
#define ATTACK_POSITION_WEIGHT 50   // attacking positions

int evaluate_opening(board_t* board) {
    int ai_pieces = get_piece_count(board, 3);
    int human_pieces = get_piece_count(board, 1);

    // Count immediate mill threats
    int ai_threats = count_immediate_mill_threats(board, 3);
    int human_threats = count_immediate_mill_threats(board, 1);

    // Count trapped opponent pieces
    int trapped_opponent = count_trapped_pieces(board, 1);

    // Evaluate attacking formations
    int attack_score = evaluate_attack_formation(board);

    return (ai_pieces - human_pieces) * PIECE_WEIGHT +
           (ai_threats - human_threats) * MILL_WEIGHT +
           trapped_opponent * BLOCKED_OPPONENT_WEIGHT +
           attack_score * ATTACK_POSITION_WEIGHT;
}

int evaluate_midgame(board_t* board) {
    int ai_pieces = get_piece_count(board, 3);
    int human_pieces = get_piece_count(board, 1);

    if (human_pieces <= 2) return INFINITY;
    if (ai_pieces <= 2) return NEG_INFINITY;

    // Evaluate immediate threats and counters
    int immediate_threats = evaluate_immediate_threats(board);

    // Count pieces that can be captured next turn
    int capturable_pieces = count_capturable_pieces(board);

    // Evaluate double mill possibilities
    int double_mill_potential = evaluate_double_mill_potential(board);

    // Evaluate trap setups
    int trap_score = evaluate_trap_setups(board);

    return immediate_threats * MILL_WEIGHT +
           capturable_pieces * PIECE_WEIGHT +
           double_mill_potential * DOUBLE_MILL_WEIGHT +
           trap_score * TRAP_SETUP_WEIGHT;
}

// threat of a mill
int count_immediate_mill_threats(board_t* board, int player) {
    int threats = 0;
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == EMPTY) {
            board->cell[i] = player;
            if (check_mill(board, i)) threats++;
            board->cell[i] = EMPTY;
        }
    }
    return threats;
}

int count_trapped_pieces(board_t* board, int player) {
    int trapped = 0;
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == player) {
            bool can_move = false;
            for (int j = 0; j < 4 && adjacentCells[i][j] != -1; j++) {
                if (board->cell[adjacentCells[i][j]] == EMPTY) {
                    can_move = true;
                    break;
                }
            }
            if (!can_move) trapped++;
        }
    }
    return trapped;
}

int evaluate_immediate_threats(board_t* board) {
    int score = 0;

    // Check for immediate mill completion possibilities
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == EMPTY) {
            // Check if AI can form mill
            board->cell[i] = 3;
            if (check_mill(board, i)) score += 100;

            // Check if blocking opponent mill
            board->cell[i] = 1;
            if (check_mill(board, i)) score += 80;

            board->cell[i] = EMPTY;
        }
    }
    return score;
}

int count_capturable_pieces(board_t* board) {
    int count = 0;
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == 1) {  // opponent piece
            bool is_in_mill = false;
            for (int j = 0; j < 16; j++) {
                if ((millstable[j][0] == i || millstable[j][1] == i || millstable[j][2] == i) &&
                    board->cell[millstable[j][0]] == 1 &&
                    board->cell[millstable[j][1]] == 1 &&
                    board->cell[millstable[j][2]] == 1) {
                    is_in_mill = true;
                    break;
                }
            }
            if (!is_in_mill) count++;
        }
    }
    return count;
}

int evaluate_double_mill_potential(board_t* board) {
    int potential = 0;

    // Try each empty position
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == EMPTY) {
            board->cell[i] = 3;  // Place AI piece

            // Count how many mills this position could be part of
            int mill_count = 0;
            for (int j = 0; j < 16; j++) {
                if (millstable[j][0] == i || millstable[j][1] == i || millstable[j][2] == i) {
                    // Count AI pieces in this potential mill
                    int ai_pieces = 0;
                    int empty_spots = 0;
                    for (int k = 0; k < 3; k++) {
                        if (board->cell[millstable[j][k]] == 3) ai_pieces++;
                        else if (board->cell[millstable[j][k]] == EMPTY) empty_spots++;
                    }
                    if (ai_pieces == 2 && empty_spots == 1) mill_count++;
                }
            }
            if (mill_count >= 2) potential += 200;  // Position can form multiple mills

            board->cell[i] = EMPTY;  // Reset position
        }
    }
    return potential;
}

int evaluate_trap_setups(board_t* board) {
    int score = 0;

    // Check for fork situations where opponent must block one mill
    // but enables another
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == EMPTY) {
            board->cell[i] = 3;

            // Count threatening positions created
            int threats = count_immediate_mill_threats(board, 3);
            if (threats >= 2) score += 300;

            board->cell[i] = EMPTY;
        }
    }
    return score;
}

int evaluate_attack_formation(board_t* board) {
    int score = 0;

    // Look for pieces that control multiple lines
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == 3) {  // AI piece
            int control_count = 0;
            for (int j = 0; j < 16; j++) {
                if (millstable[j][0] == i || millstable[j][1] == i || millstable[j][2] == i) {
                    control_count++;
                }
            }
            score += control_count * 50;  // Reward pieces that control multiple lines
        }
    }
    return score;
}

int evaluate_endgame(board_t* board) {
    int ai_pieces = get_piece_count(board, 3);
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
        if (phase == 1) {
            for (int i = 0; i < 24; i++) {
                if (board->cell[i] == EMPTY) {
                    board->cell[i] = 3;
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
        } else {
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
                            if (board->cell[j] == 3 && !check_mill(board, j)) {
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

/*
// Enhanced evaluation weights and new strategic factors
#define PIECE_WEIGHT 1000
#define MILL_WEIGHT 300
#define DOUBLE_MILL_WEIGHT 500
#define BLOCKED_WEIGHT 150
#define MOBILITY_WEIGHT 200
#define POSITION_WEIGHT 100
#define POTENTIAL_MILL_WEIGHT 250
#define CENTER_CONTROL_WEIGHT 175
#define DEFENSE_FORMATION_WEIGHT 150

// Strategic positions (corners and center points)
const int strategic_positions[] = {0, 3, 6, 8, 11, 14, 16, 19, 22};

int evaluate_opening(board_t* board) {
    int ai_pieces = get_piece_count(board, 3);
    int human_pieces = get_piece_count(board, 1);

    // Count strategic position control
    int ai_strategic = count_strategic_positions(board, 3);
    int human_strategic = count_strategic_positions(board, 1);

    // Count potential mills with more weight
    int ai_potential = count_potential_mills(board, 3);
    int human_potential = count_potential_mills(board, 1);

    // Count center control (middle points of each line)
    int ai_center = count_center_control(board, 3);
    int human_center = count_center_control(board, 1);

    return (ai_pieces - human_pieces) * PIECE_WEIGHT +
           (ai_potential - human_potential) * POTENTIAL_MILL_WEIGHT +
           (ai_strategic - human_strategic) * POSITION_WEIGHT +
           (ai_center - human_center) * CENTER_CONTROL_WEIGHT;
}

int evaluate_midgame(board_t* board) {
    int ai_pieces = get_piece_count(board, 3);
    int human_pieces = get_piece_count(board, 1);

    if (human_pieces <= 2) return INFINITY;
    if (ai_pieces <= 2) return NEG_INFINITY;

    // Enhanced piece difference weight
    int piece_diff = (ai_pieces - human_pieces) * PIECE_WEIGHT;

    // Mobility calculation with higher weight
    int ai_mobility = count_possible_moves(board, 3);
    int human_mobility = count_possible_moves(board, 1);
    int mobility = (ai_mobility - human_mobility) * MOBILITY_WEIGHT;

    // Blocked pieces calculation
    int ai_blocked = count_blocked_pieces(board, 1);  // Count opponent's blocked pieces
    int human_blocked = count_blocked_pieces(board, 3);  // Count AI's blocked pieces
    int blocked = (ai_blocked - human_blocked) * BLOCKED_WEIGHT;

    // Mills and double mills
    int ai_mills = count_mills(board, 3);
    int human_mills = count_mills(board, 1);
    int mills = (ai_mills - human_mills) * MILL_WEIGHT;

    int ai_double = count_double_mills(board, 3);
    int human_double = count_double_mills(board, 1);
    int double_mills = (ai_double - human_double) * DOUBLE_MILL_WEIGHT;

    // Defense formation evaluation
    int defense = evaluate_defense_formation(board) * DEFENSE_FORMATION_WEIGHT;

    return piece_diff + mobility + blocked + mills + double_mills + defense;
}

// New helper functions for enhanced evaluation

int count_strategic_positions(board_t* board, int player) {
    int count = 0;
    for (int i = 0; i < sizeof(strategic_positions)/sizeof(int); i++) {
        if (board->cell[strategic_positions[i]] == player) {
            count++;
        }
    }
    return count;
}

int count_center_control(board_t* board, int player) {
    // Middle points of each line
    const int center_positions[] = {1, 4, 7, 9, 12, 15, 17, 20, 23};
    int count = 0;
    for (int i = 0; i < sizeof(center_positions)/sizeof(int); i++) {
        if (board->cell[center_positions[i]] == player) {
            count++;
        }
    }
    return count;
}

int count_mills(board_t* board, int player) {
    int count = 0;
    for (int i = 0; i < 16; i++) {
        if (board->cell[millstable[i][0]] == player &&
            board->cell[millstable[i][1]] == player &&
            board->cell[millstable[i][2]] == player) {
            count++;
        }
    }
    return count;
}

int evaluate_defense_formation(board_t* board) {
    int score = 0;
    // Check for defensive triangles and L-shapes
    const int defensive_patterns[][3] = {
        {0, 1, 3},   // Corner triangles
        {3, 4, 6},
        {16, 17, 19},
        {19, 20, 22}
    };

    for (int i = 0; i < 4; i++) {
        int ai_count = 0;
        int human_count = 0;
        for (int j = 0; j < 3; j++) {
            if (board->cell[defensive_patterns[i][j]] == 3) ai_count++;
            if (board->cell[defensive_patterns[i][j]] == 1) human_count++;
        }
        if (ai_count == 3) score += 2;  // Complete defensive formation
        else if (ai_count == 2 && human_count == 0) score += 1;  // Partial formation
    }
    return score;
}
*/

int get_piece_count(board_t* board, int player) {
    int count = 0;
    for (int i = 0; i < 24; i++) {
        if (board->cell[i] == player) count++;
    }
    return count;
}

int count_potential_mills(board_t* board, int player) {
    int count = 0;
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
    strcpy(players[1].player_name, "Machine");
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
        /*if (no_adjacents(&game_board, game_board.turn)) {
            winner = (game_board.turn == 1 ? 2 : 1) - 1;
            is_winner(winner);
            break;
        }
        */
        if (game_board.turn == 1) {
            numberdestination = is_validemove(&game_board, game_board.turn);
            print_board(&game_board);
            remove_piece(check_mill(&game_board, numberdestination), 2);
        } else {
            printf("AI turn \n");
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
            remove_piece(check_mill(&game_board, numberdestination), 2);
        } else {
            printf("AI turn \n");
            numberdestination = ai_make_move(&game_board, 3);
            print_board(&game_board);
            remove_piece(check_mill(&game_board, numberdestination), 1);
        }
        switch_turn(&game_board);
    }
}

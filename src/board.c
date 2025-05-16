#include "../include/color.h"
#include "../include/board.h"

const int millstable[16][3] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {9, 10, 11},
    {12, 13, 14},
    {15, 16, 17},
    {18, 19, 20},
    {21, 22, 23},
    {0, 9, 21},
    {3, 10, 18},
    {6, 11, 15},
    {1, 4, 7},
    {16, 19, 22},
    {8, 12, 17},
    {5, 13, 20},
    {2, 14, 23},

};

const int adjacentCells[24][4] = {
    {1, 9, -1, -1},    // Cell 0 and -1 for no option
    {0, 2, 4, -1},     // Cell 1
    {1, 14, -1, -1},   // Cell 2
    {4, 10, -1, -1},   // Cell 3
    {1, 3, 5, 7},      // Cell 4
    {4, 13, -1, -1},   // Cell 5
    {7, 11, -1, -1},   // Cell 6
    {4, 6, 8, -1},     // Cell 7
    {7, 12, -1, -1},   // Cell 8
    {0, 10, 21, -1},   // Cell 9
    {3, 9, 11, 18},    // Cell 10
    {6, 10, 15, -1},   // Cell 11
    {8, 13, 17, -1},   // Cell 12
    {5, 12, 14, 20},   // Cell 13
    {2, 13, 23, -1},   // Cell 14
    {11, 16, -1, -1},  // Cell 15
    {15, 17, 19, -1},  // Cell 16
    {12, 16, -1, -1},  // Cell 17
    {10, 19, -1, -1},  // Cell 18
    {16, 18, 20, 22},  // Cell 19
    {13, 19,-1, -1},  // Cell 20
    {9, 22, -1, -1},   // Cell 21
    {19, 21, 23, -1},  // Cell 22
    {14, 22, -1, -1}   // Cell 23
};

void initialize_board(board_t* game)
{
    int i ;
    for(i = 0; i < 24; i++){
            game->cell[i]= EMPTY;
    }
    game->turn = player1 ;
}

void print_board()
{
    system("cls");
    printf("\n\n\n");
    printf("\t\t\t ");co(0);printf("|00|");color(15,0);printf("=======================");co(1);printf("|01|");color(15,0);printf("=======================");co(02);printf("|02|");printf("\n");
    color(15,0);printf("\t\t\t  |  \t\t\t     |  \t\t        |\n");
                printf("\t\t\t  |  \t\t\t     |   \t\t        | \n");
                printf("\t\t\t  |  \t\t\t     |   \t\t        | \n");
    printf("\t\t\t  |         ");co(3);printf("|03|");color(15,0);printf("============");co(4);printf("|04|");color(15,0);printf("============");co(5);printf("|05|");color(15,0);printf("        |\n");
               printf("\t\t\t  |          |               |               |          |\n");
               printf("\t\t\t  |          |               |               |          |\n");
               printf("\t\t\t  |          |               |               |          |\n");
    printf("\t\t\t  |          |    ");co(6);printf("|06|");color(15,0);printf("======");co(7);printf("|07|");color(15,0);printf("======");co(8);printf("|08|");color(15,0);printf("   |          |\n");
               printf("\t\t\t  |          |     |                   |     |          |\n");
               printf("\t\t\t  |          |     |                   |     |          |\n");
               printf("\t\t\t  |          |     |                   |     |          |\n");
    printf("\t\t\t ");co(9);printf("|09|");color(15,0);printf("=======");co(10);printf("|10|");color(15,0);printf("==");co(11);printf("|11|");color(15,0);printf("                ");co(12);printf("|12|");color(15,0);printf("==");co(13);printf("|13|");color(15,0);printf("=======");co(14);printf("|14|");printf("\n");
    color(15,0);printf("\t\t\t  |          |     |                   |     |          |\n");
                printf("\t\t\t  |          |     |                   |     |          |\n");
                printf("\t\t\t  |          |     |                   |     |          |\n");
    printf("\t\t\t  |          |    ");co(15);printf("|15|");color(15,0);printf("======");co(16);printf("|16|");color(15,0);printf("======");co(17);printf("|17|");color(15,0);printf("   |          |\n");
               printf("\t\t\t  |          |               |               |          |\n");
               printf("\t\t\t  |          |               |               |          |\n");
               printf("\t\t\t  |          |               |               |          |\n");
    printf("\t\t\t  |         ");co(18);printf("|18|");color(15,0);printf("============");co(19);printf("|19|");color(15,0);printf("============");co(20);printf("|20|");color(15,0);printf("        |\n");
    color(15,0);printf("\t\t\t  |  \t\t\t     |  \t\t        |\n");
                printf("\t\t\t  |  \t\t\t     |   \t\t        | \n");
                printf("\t\t\t  |  \t\t\t     |   \t\t        | \n");
    printf("\t\t\t ");co(21);printf("|21|");color(15,0);printf("=======================");co(22);printf("|22|");color(15,0);printf("=======================");co(23);printf("|23|");printf("\n");color(15,0);

}

void player_turn(board_t *game, char* name, int pieces_count)
{
    int cellid;
    cellid = place_piece(game, name, game->turn);
    print_board(game);
    if (pieces_count >= 5) {
        remove_piece(check_mill(&game_board, cellid), (game_board.turn == 1 ? 2 : 1));
    }
    switch_turn(&game_board);

}

int place_piece(board_t* game, char* name, int player)
{
    /* I use test to check if the player enter a correct integer (bug with scanf) */
    int cellid = 0, test=0;
    if (player == 3) {
        do {
            cellid = rand() % 24;
        } while (game->cell[cellid] != EMPTY);

        game->cell[cellid] = player;
        return cellid;
    }
    color(15, 0);
    printf("Turn of ");
    color(14, 0);
    printf("%s\n",name);
    color(15, 0);
    printf("chose a position (0-23):");

    do {
        test = scanf("%d", &cellid);
        if (test == 1) {
            if (cellid < 0 || cellid > 23)
                printf("invalid cell id ! please choose a number between 0-23:");
            else if (game->cell[cellid] != EMPTY)
                printf("Cell %d is already occupied! Choose another cell: ", cellid);
                }
        else {
            printf("invalid cell id ! please choose a number between 0-23:");
            while (getchar() != '\n'); // consume invalid character in the buffer
        }
    }while(cellid < 0 || cellid > 23 || game->cell[cellid] != EMPTY);

    game->cell[cellid] = player;
    return cellid;
    /* we can add a valide move here ; */
}

void remove_piece(int what, int oponentplayer)
{
    int remove;
    color(14,0);
    printf("%s\n",players[game_board.turn - 1].player_name);
    color(15,0);
    if (what) {

        if (game_board.turn == 3) {
            do {
                remove = rand() % 24;
            } while (game_board.cell[remove] == game_board.turn ||
                    game_board.cell[remove] == EMPTY ||
                    (check_mill(&game_board, remove) && !all_mills(&game_board, (game_board.turn == 1) ? 3 : 1)));

            game_board.cell[remove] = EMPTY;
            --players[oponentplayer - 1].number_piece;
            return;
        }

        do {
            printf("Enter the position to remove (not part of a mill, if possible): ");
            scanf("%d", &remove);
            getchar();

            if (remove < 0 || remove > 23) {
                printf("Invalid position try again !!\n");
                continue;
            }

            if (game_board.cell[remove] != game_board.turn && game_board.cell[remove] != EMPTY) {

                if (check_mill(&game_board, remove)) {
                    if (all_mills(&game_board, (game_board.turn == 1) ? 2 : 1)) {
                        game_board.cell[remove] = EMPTY;

                        break;
                    } else {
                        printf("Position is part of a mill. Try again.\n");
                    }
                } else {
                    game_board.cell[remove] = EMPTY;
                    break;
                }
            } else {
                printf("Invalid position. It must belong to the opponent.\n");
            }
          }while (1);

    --players[oponentplayer - 1].number_piece;
        }

}

int check_mill(board_t *board, int cell_id)
{
     for (int i = 0; i < 16; i++) {

        if (millstable[i][0] == cell_id || millstable[i][1] == cell_id || millstable[i][2] == cell_id) {
            if (board->cell[millstable[i][0]] == board->cell[millstable[i][1]] &&
                board->cell[millstable[i][0]] == board->cell[millstable[i][2]] &&
                board->cell[millstable[i][1]] == board->cell[millstable[i][2]])
                    return 1;
        }
     }
     return 0;

}

int all_mills(board_t *board, int player)
{

    int mill , i;
    for (i = 0; i < 24; i++) {
        if (board->cell[i]== player && check_mill(&game_board, i))
            mill++;
    }

    if (mill == players[player - 1].number_piece)
        return 1;
    else
        return 0;
}

void switch_turn(board_t* board)
{
    board->turn = (board->turn == 1) ? 2 : 1; // Switch between Player 1 and Player 2
}

int is_validemove(board_t *board, int player)
{
    int position, destination;

    /* Random player but when we we'll check for the minmax we'll use strcmp*/
    if (player == 3) {
        do {
            position = rand() % 24;
            destination = rand() % 24;
        } while (board->cell[position] != player ||
                 board->cell[position] == EMPTY || board->cell[destination] != EMPTY
                 ||!is_adjacent(position, destination));

        board->cell[destination] = player;
        board->cell[position] = EMPTY;
        return destination;
    }

    do {
    printf("Enter the number of the position you want to move: ");
    scanf("%d", &position);
    getchar();
    if (position < 0 || position > 23 || board->cell[position] != player || board->cell[position] == EMPTY) {
        printf("invalid position try again .\n");
        continue;
    }

    printf("enter the destination: ");
    scanf("%d", &destination);
    if (destination < 0 || destination > 23 ) {
        printf("invalid destination try again .\n");
        continue;
    }
    if (board->cell[destination] == EMPTY && is_adjacent(position, destination))
        break;

    }while(1);

    board->cell[destination] = player;
    board->cell[position] = EMPTY;
    return destination;
}

int is_adjacent(int source, int destination)
{

    for(int i = 0; i < 4; i++){
        if(adjacentCells[source][i] == destination && adjacentCells[source][i] != -1)
            return 1;
    }
    return 0;
}

int no_adjacents(board_t* game, int player)
{
    int i, j,number_adjacent = 0;

    for (i = 0; i < 23; i++) {
        if (game->cell[i]== player) {
                for(j = 0; j < 4; j++){
                    if(game->cell[adjacentCells[i][j]] == EMPTY && adjacentCells[i][j] != -1)
                        number_adjacent++;
                }
        }
    }
    if (number_adjacent == 0)
        return 1;
    else
        return 0;
}

int move_anywhere(board_t *board)
{
    int position ,destination;

    // random
    if (board->turn == 3) {
        do {
            position = rand() % 24;
            destination = rand() % 24;
        } while (board->cell[position] != board->turn ||
                board->cell[position] == EMPTY ||
                board->cell[destination] != EMPTY);

        board->cell[destination] = board->turn;
        board->cell[position] = EMPTY;
        return destination;
    }

    printf("%s you can move to any empty place \n", players[board->turn - 1].player_name);
    do {
    printf("Enter the number of the position you want to move");
    scanf("%d", &position);
    getchar();
    if (position < 0 || position > 23 || board->cell[position] != board->turn || board->cell[position] == EMPTY) {
        printf("invalid position try again .\n");
        continue;
    }

    printf("enter the destination: ");
    scanf("%d", &destination);
    if (destination < 0 || destination > 23 ) {
        printf("invalid destination try again .\n");
        continue;
    }
    if (board->cell[destination] == EMPTY )
        break;

    }while(1);
    board->cell[destination] = board->turn ;
    board->cell[position] = EMPTY;
    return destination;
}

void is_winner(int winner) {
    system("cls");
    color(14, 0);
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           | \n");
    printf("\t\t\t\t\t|        %s                |\n",players[winner].player_name);
    printf("\t\t\t\t\t|___________________________| \n");
}
void playervsplayer()
{
    int pieces_placed = 0, winner = 3, numberdestination;

    initialize_board(&game_board);

    printf("enter the names of players: \n");
    printf("player 1: ");
    scanf("%s", players[0].player_name);
    players[0].ID = 1;
    printf("player 2: ");
    scanf("%s", players[1].player_name);
    players[1].ID = 2;

    players[0].number_piece = 0;
    players[1].number_piece = 0;

    /* Phase 1: Placing pieces */

    while (pieces_placed < 18 ){

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
        }else {
            player_turn(&game_board, players[1].player_name, pieces_placed);
            players[1].number_piece++;
        }

    }

    /* Phase 2: Moving pieces */

    while (players[0].number_piece > 3 && players[1].number_piece > 3 ) {

        print_board(&game_board);
        color(14, 0);
        printf("Phase 2: Moving pieces\n");
        color(4, 0);
        printf("player %s : %d piece\n ", players[0].player_name, players[0].number_piece);
        color(2, 0);
        printf("player %s : %d piece\n", players[1].player_name, players[1].number_piece);
        color(15, 0);
        printf("Turn of %s \n ", players[game_board.turn - 1].player_name);

        /* check if they have no adjacent points to move to */
        if (no_adjacents(&game_board, game_board.turn)) {
            winner = (game_board.turn == 1 ? 2 : 1) - 1;
            is_winner(winner);
            break;

        }

        numberdestination = is_validemove(&game_board, game_board.turn);
        print_board(&game_board);
        remove_piece(check_mill(&game_board, numberdestination), (game_board.turn == 1 ? 2 : 1));
        switch_turn(&game_board);

    }

    /* Phase 3: Flying */

    while(winner != 0 && winner != 1){


        if (players[0].number_piece == 2){
            winner = 1;
            is_winner(winner);
            break;
        }

        else if (players[1].number_piece == 2) {
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
        printf("player %s : %d piece\n", players[1].player_name, players[1].number_piece);
        color(15, 0);
        printf("Turn of %s \n ", players[game_board.turn - 1].player_name);

        /* check if they have no adjacent points to move to */
        if (no_adjacents(&game_board, game_board.turn)) {
            winner = (game_board.turn == 1 ? 2 : 1) - 1;// that mean players[0 or 1].player_name or the other
            is_winner(winner);
            break;
        }

        /*here we should check for both players because maybe have the case where the two player have 3 pieces. */
        if (players[game_board.turn -1].number_piece == 3) {
            numberdestination = move_anywhere(&game_board);
            /* u can  add print board here for more visibility*/
            remove_piece(check_mill(&game_board, numberdestination), (game_board.turn == 1 ? 2 : 1));
        }else {
            numberdestination = is_validemove(&game_board, game_board.turn);
            remove_piece(check_mill(&game_board, numberdestination), (game_board.turn == 1 ? 2 : 1));
        }
        print_board(&game_board);
        switch_turn(&game_board);

    }




}





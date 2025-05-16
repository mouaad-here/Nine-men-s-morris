#include <windows.h>
#include "../include/board.h"


void color (int text_color, int background_Color) {
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,background_Color*16+text_color);

}
void co (int i) {
    if (game_board.cell[i]==0)color(15,0);
    if (game_board.cell[i]==1)color(0,4);
    if (game_board.cell[i]==2)color(0,2);
    if (game_board.cell[i]==3)color(0,14);
}

#ifndef INTERFACE_MENU_H_INCLUDED
#define INTERFACE_MENU_H_INCLUDED
#define stop 'e'
#define BUFF_SIZE  100

#include <fcntl.h>
#include <string.h>
#include <synchapi.h> /* for the sleep function */

void opening();
void main_menu();
void startme();
void player_vs_machine();
int get_stop_pos(char *buff, int size);
void printloge();
#endif // INTERFACE_MENU_H

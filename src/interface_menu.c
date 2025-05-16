#include "../include/color.h"
#include "../include/board.h"
#include "../include/interface_menu.h"

void opening () {
    color(14, 0);
    /*
    system("mode con LINES=30 COLS=130");

    */
    system("mode con LINES=200 COLS=200");
    int i = 0;
	FILE *file;
	char buff[BUFF_SIZE];
	int nb_read = 0;
	int char_pos;

    file = fopen("./game_intro.txt", "rw");
	if (file  == NULL) print_exit("can't open file ");

    // printf("%s", BBLU);  // set color
	fflush(stdout); // flush stdout
    while (i < 11)
	{
        color(14, 0);
        memset(buff, '\0', BUFF_SIZE); // set null  charcter
		int nb_read = fread(buff, 1, BUFF_SIZE, file);
		if (nb_read == 0)
		{
			fseek(file  ,0, SEEK_SET);
			continue;
		}

		char_pos = get_stop_pos(buff, BUFF_SIZE);
		if (char_pos != -1)
            i++;
		if (char_pos == -1)
			fwrite(buff, 1, nb_read, stdout);
		else
		{

            fwrite(buff,  1, char_pos, stdout);
			Sleep(200);
			// usleep(50000);
			// system("clear");// using this in lunix or mac or if using git bash in windows
			if (i <= 10)
                system("cls");
			if ( nb_read - 2 - char_pos > 0 && char_pos + 2 < BUFF_SIZE)
			fwrite(buff + (char_pos + 2), 1, nb_read - 2 - char_pos, stdout);

		}
    }

    printf("\n press enter to start the game ;)");
    getchar();
    main_menu();
}

int get_stop_pos(char *buff, int size)
{
	if (size == 0 || buff == NULL)
		return -1;

		for (int i = 0; i < size; i++)
		{
			if (buff[i] == stop)
				return i;
		}
		return -1;

}
void printlogo(){
    color(14,0);
    printf("\n\n\n");
    printf("\t\t\t   _                  _                              _ _       \n");
    printf("\t\t\t  (_)                | |                            | (_)      \n");
    printf("\t\t\t   _  ___ _   _    __| | ___   _ __ ___   ___  _   _| |_ _ __  \n");
    printf("\t\t\t  | |/ _ \\ | | |  / _` |/ _ \\ | '_ ` _ \\ / _ \\| | | | | | '_ \\ \n");
    printf("\t\t\t  | |  __/ |_| | | (_| |  __/ | | | | | | (_) | |_| | | | | | |\n");
    printf("\t\t\t _|_|\\___|\\__,_|  \\__,_|\\___| |_| |_| |_|\\___/ \\__,_|_|_|_| |_|\n");
    printf("\t\t\t|__/\n");
    color(14, 0);
}
void  print_exit(char *str)
{
	printf("%s\n", str);
	exit(1);
}


void main_menu() {
    char choice;
    printlogo();
    do {
    system("cls");
    printlogo();
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           | \n");
    printf("\t\t\t\t\t|         A-START           |\n");
    printf("\t\t\t\t\t|___________________________| \n");
    color(5,0);
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           |\n");
    printf("\t\t\t\t\t|         B-Instructions    |\n");
    printf("\t\t\t\t\t|___________________________| \n");
    color(11,0);
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           |\n");
    printf("\t\t\t\t\t|          C-EXIT           |\n");
    printf("\t\t\t\t\t|___________________________| \n\n");
    printf("Enter your choice : ");
    scanf("%c",&choice);
    switch (choice) {
    case 'a':
    case 'A':
            startme();
            break;
    case 'b':
    case 'B':
            //instractions();
            break;
    case 'c':
    case 'C':
            break;
    default:
            printf("please verify your choice ");

    }
    }while(choice != 'a' && choice != 'A' && choice !='b' && choice != 'B' && choice != 'c' && choice != 'C');
}
void startme() {
    char choice;

    do {
    system("cls");
    printlogo();
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           | \n");
    printf("\t\t\t\t\t|      A-player vs player   |\n");
    printf("\t\t\t\t\t|___________________________| \n");
    color(5,0);
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           |\n");
    printf("\t\t\t\t\t|      B-player vs machine  |\n");
    printf("\t\t\t\t\t|___________________________| \n");
    color(11,0);
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           |\n");
    printf("\t\t\t\t\t|      C-return to menu     |\n");
    printf("\t\t\t\t\t|___________________________| \n\n");
    printf("Enter your choice : ");
    color(15, 0);
    scanf("%c",&choice);
    switch (choice) {
    case 'a':
    case 'A':
            playervsplayer();
            break;
    case 'b':
    case 'B':
            player_vs_machine();
            break;
    case 'c':
    case 'C':
            main_menu();
            break;
    default:
            printf("please verify your choice ");

    }
    }while(choice != 'a' && choice != 'A' && choice !='b' && choice != 'B' && choice != 'c' && choice != 'C');
}

void player_vs_machine () {
    char choice;
    do {
    system("cls");
    printlogo();
    color(2,0);
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           | \n");
    printf("\t\t\t\t\t|      A- Easy              |\n");
    printf("\t\t\t\t\t|___________________________| \n");
    color(5,0);
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           |\n");
    printf("\t\t\t\t\t|      B- Hard              |\n");
    printf("\t\t\t\t\t|___________________________| \n");
    color(11,0);
    printf("\t\t\t\t\t ___________________________\n");
    printf("\t\t\t\t\t|                           |\n");
    printf("\t\t\t\t\t|      D-return to menu     |\n");
    printf("\t\t\t\t\t|___________________________| \n\n");
    color(15, 0);
    printf("Enter your choice : ");
    scanf("%c",&choice);
    switch (choice) {
    case 'a':
    case 'A':
            player_vs_random();
            break;
    case 'b':
    case 'B':
            player_vs_minimax();
            break;
    case 'd':
    case 'D':
            system("cls");
            startme();
            break;
    default:
            printf("please verify your choice ");

    }
    }while(choice != 'a' && choice != 'A' && choice !='b' && choice != 'B' && choice != 'c' && choice != 'C' && choice != 'd' && choice != 'D');
}

void instruction () {
    printf("instruction");
    printf("click on something to return to main menu.");
    getchar();
    main_menu();
}




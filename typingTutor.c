#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define MAX 6
#define MIN 1
#define H 8
#define W 25

#define ESC 27
#define RETURN 10

int mainMenu();
void highliteOption(int highlitePosition, int actualPosition, char *option);
char *selectChallenge();
int playChallenge(char *challengeChoice);

int main(void){
	int menuOption = 0;
	char *challengeChoice;
	while ( menuOption != MAX-1){
		menuOption = mainMenu();
		switch(menuOption)
		{
			case 0:
				challengeChoice = selectChallenge();
				printw("You chose: %s", challengeChoice);
				refresh();
				playChallenge(challengeChoice);
				break;
			case 1:
				// Select User
				break;
			case 2:
				// Statistics
				break;
			case 3:
				// View User Records
				break;
			case 4:
				// About
				break;
			case 5:
				// Exit
				break;
			default:
				break;
		}
	}
	return 0;
}

int mainMenu(){
	int h, w, i;
	int keyPressed = 0;
    int position = 0;
	char *options[] = {"1. Select Challenge", "2. Select User", "3. Statistics", "4. View User Records", "5. About", "6. Exit"};
	
	//~ move(0, 1);
	//~ printw("  >> Use UP & DOWN arrows to traverse menu options");
	//~ wrefresh(stdscr);	
	
	initscr();	// initialize screen
	cbreak();	// disables line buffering & erase/kill char processing
	noecho();	// does not echo key entrances, good for interactive display
	
	getmaxyx(stdscr, h, w); 	// gets size of opened window
	WINDOW *win = newwin(H, 	// creates window win of size relative to originally opened window
				W, h-16, w-52);		
	box(win, 0, 0);				// box around window win
	wrefresh(win);				// to update win
	keypad(win, true);			// enable keys recognition for menu traversal with arrow keys

	do
	{
		wrefresh(win);
		switch (keyPressed)
		{
			case KEY_UP:
				if (position >= MIN)
					position --;
				break;
			case KEY_DOWN:
				if (position < MAX-1)
					position ++;
				break;
			case ESC:
				return (position = 4);
			case RETURN:
				return position;
			default: ;
		}
		for (i = 0; i < MAX; i++)
		{
			if (position == i)
				wattron(win, A_REVERSE);
			mvwprintw(win, i+1, 2, "%s", options[i]);
			wattroff(win, A_REVERSE);
		}		
		wmove(win, position+1, 2);
		
		//~ highliteOption(1, position, options[0]);
		//~ highliteOption(2, position, options[1]);
		//~ highliteOption(3, position, options[2]);
		//~ highliteOption(4, position, options[3]);
		//~ wmove(win, 0, position);
	} while ((keyPressed = wgetch(win)));
	
	delwin(win);
	//~ endwin();
	return position;
}	

// Improvements: 
// clear stdscr after exiting selectChallenge
// select again when making invalid choice
// after choice made, press enter to continue or choose again
char *selectChallenge(){
	FILE *fp;
	char *choice = (char *)malloc(20);
	char ch, num;
	int i = 0;
	noecho();
	fp = fopen("./challenges/challengeList.txt", "r");
	if (fp == NULL){
		printw("No file exists. Need to generate list file");
		refresh();
		return NULL;
	}
	else {
		printw("Choose a file # from the following\n\n");
		while ((ch = fgetc(fp)) != EOF){
			printw("%c", ch);
		}
		scanw("%c", &num);
		fseek(fp, 0L, SEEK_SET);
		while ((ch = fgetc(fp)) != EOF){
			if (ch == num){
				fseek(fp, ftell(fp)-2, SEEK_CUR);
				while ((ch = fgetc(fp)) != '\n'){
					choice[i++] = ch;
				}
				break;
			}
		}
		strcpy(choice+strlen(choice), ".txt");
	}
	wrefresh(stdscr);
	clear();
	//~ printw("%s", choice+3);
	//~ refresh();
	//~ getch();
	fclose(fp);
	return (choice+3);
}
int playChallenge(char *challengeChoice){
	clear();
	printw("%s",challengeChoice);
	refresh();
	getch();
	clear();
	int lines, i;
	char ch;
	size_t sz = strlen(challengeChoice);
	char *samplesDir = "./challenges/samples/";
	char *challengeFilePath = (char *)malloc(strlen(samplesDir)+sz);
	strcpy(challengeFilePath, samplesDir);
	strcpy(challengeFilePath + strlen(samplesDir) , challengeChoice);
	//~ printw("%s", challengeFilePath);
	//~ getch();
	FILE *fp = fopen(challengeFilePath, "r");
	if (fp == NULL){
		printw("No file exists. Need to generate list file");
		refresh();
		return 0;
	}
	printw("How many lines of text do you want for the challenge?  ");
	echo();
	scanw("%d", &lines);
	clear();
	for (i = 0; i < lines; i++)
	{
		while ((ch = fgetc(fp)) != '\n'){
			if (ch == EOF)
				break;
			printw("%c", ch);
		}
		if (ch == EOF)
			break;	
		printw("\n");
	}
	printw("\nPress \"enter\" when you're ready to start.");
	getch();
	printw("Press \"escape\" to complete challenge.\n\n\n");
	getch();
	// function to read user input & compare to challenge text
	// records speed & accuracy
	// typeCompare
	fclose(fp);
	return 1;
}
	
void highliteOption(int highlitePosition, int actualPosition, char *option){
	
	if(highlitePosition == actualPosition){
		
		start_color();
		init_pair(1, COLOR_BLACK, COLOR_YELLOW);
		attron(COLOR_PAIR(1));
		printw("%s", option);
		attroff(COLOR_PAIR(1));
	}
	else printw("%s", option);
}

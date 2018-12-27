#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <ctype.h>
#include <time.h>

#define ARR 94
#define ESC 27
#define DEL 127

long int lineSize(int line, FILE *fp);
long int fileSize(FILE *fp);
double typeCompare();

double typeCompare(int num_lines, char *file_path){
	FILE *fp = fopen(file_path, "r");
	char ch;
	long int i = 0, j = 0, flength, charcnt = 0, lnsize;
	long int lines;
	int x_cur = 0, y_cur = 0;
	clock_t t;
	//~ double time_taken;
	
	// initialize screen, input mode, and noecho
	initscr();
	cbreak();
	noecho();
	while ( ((ch = fgetc(fp)) != EOF) && (i < num_lines)){
		if (ch == '\n'){
			i++;
			printw("\n");
		}
		printw("%c", ch);
	}
	x_cur = i+5;
	//~ fseek(fp, 0L, SEEK_END);
	flength = fileSize(fp);
	lines = i;
	// initialize typing guide
	i=0;
	lnsize = lineSize(i, fp);
	mvaddch(i+1,j,ARR);
	refresh();
	move(x_cur, y_cur);
	t = clock();
	do
	{
		ch = getch();
		if (ch == ESC)
			mvdelch(x_cur-1, y_cur);
		if ( ch == DEL ){
			if(j > 0){ // if text not at beg of line
				mvdelch(i*2+1, j--);
				mvdelch(x_cur, y_cur);
				if (y_cur > 0)
					y_cur--;
				else x_cur--;
				mvaddch(i*2+1, j, ARR);
				move(x_cur, y_cur);
			}
			else if (j == 0 && i > 0){ // if text at beg of line but not at 0th line
				mvdelch(i*2+1, j);
				i--;
				j = lnsize = lineSize(i, fp);
				mvaddch(i*2+1, j, ARR);
				mvdelch(x_cur, y_cur);
				if (y_cur == 0){
					x_cur--;
					y_cur = lineSize(i, fp);
					move(x_cur, y_cur);
				}
				else if (y_cur > 0){
					y_cur--;
					move(x_cur, y_cur);
				}
				else ;
			}
			else if (j == 0 && i == 0) // curr at beg of doc
				continue;
			else ;
		}
		else if((isalnum(ch) || isspace(ch) || ispunct(ch)) && (ch != ESC)){
			if (ch != '\n')
			{
				if (i == 0){
					mvdelch(i*2+1, j);
					move(i*2+1, ++j);
					printw("%c", ARR);
					mvprintw(x_cur, y_cur++, "%c", ch);
					charcnt++;
				}
				else {
					if (i < 2){
						mvdelch(i*2+1, j);
						move(i*2+1, ++j);
					}
					else {
						mvdelch(i*2+1, j);
						move(i*2+1, ++j);
					}						
					printw("%c", ARR);
					mvprintw(x_cur, y_cur++, "%c", ch);
					charcnt++;
				}
			}
			else if (ch == '\n'){
				if (i == 0 && j == lnsize){ // if on 0th line and at end of line
					mvdelch(i*2+1, j);
					j = 0;
					i++;
					move(i*2+1, j);
					printw("%c", ARR);
					mvprintw(x_cur++, y_cur, "%c", ch);
					y_cur = 0;						
					//~ charcnt++;
				}
				else if (i > 0 && j == lnsize){ // if at end of any line except 0th
					mvdelch(i*2+1, j);
					i++;
					j = 0;
					move(i*2+1, j);
					printw("%c", ARR);
					mvprintw(x_cur++, y_cur, "%c", ch);
					y_cur = 0;
					//~ // charcnt++;
				}
				else {	// if press enter in middle of line (before reaching end of text)
					mvdelch(i*2+1, j);
					j++;
					move(i*2+1, j);
					printw("%c", ARR);
					refresh();
					
					}
				x_cur++;
				y_cur = 0;
				move(x_cur, y_cur);
				//~ charcnt++;				
			}
			else;
			
			if (i < lines){
				lnsize = lineSize(i, fp);
			}
		}
	} while (ch != ESC);
	t = clock() - t;
	//~ printw("\n%d", i);
	refresh();
	float time_taken = ((float)t)/CLOCKS_PER_SEC;
	printw("It took you %.2f seconds\n", time_taken);
	getch();
	return (((double)t)/CLOCKS_PER_SEC);
}

long int lineSize(int line, FILE *fp){
	char ch;
	long int i = 0, size = 0;
	fseek(fp, 0L, SEEK_SET);
	while ( (ch = fgetc(fp)) != EOF){
		if (ch == '\n')
			i++;
		if (i==line){
			if (line == 0)
				fseek(fp, -1L, SEEK_CUR);
			else
				fseek(fp, 0L, SEEK_CUR);
			while(1){
				ch = fgetc(fp);
				if (ch != '\n')
					size++;
				else if( ch == '\n')
					break;
			}
			break;
		}
	}
	return size;
}


long int fileSize(FILE *fp){
	char ch;
	long int i = 0, size = 0;
	fseek(fp, 0L, SEEK_SET);
	while ( (ch = fgetc(fp)) != EOF){
		if (ch == '\n')
			i++;
		else size++;
	}
	return size;
}

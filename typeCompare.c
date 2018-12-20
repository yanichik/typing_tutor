#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define ARR 94

long int lineSize(int line, FILE *fp);
long int fileSize(FILE *fp);
int main(){
	initscr();
	cbreak();
	echo();
	FILE *fp = fopen("./challenges/samples/plato.txt", "r");
	char ch;
	long int i = 0, j = 0, lines, flength, charcnt = 0, lnsize;
	int x_cur = 0, y_cur = 0;
	while ((ch = fgetc(fp)) != EOF){
		if (ch == '\n'){
			i++;
			printw("\n");
		}
		printw("%c", ch);
	}
	x_cur = i+2;
	//~ fseek(fp, 0L, SEEK_END);
	flength = fileSize(fp);
	lines = i;
	// initialize typing guide
	i=0;
	lnsize = lineSize(i, fp);
	mvaddch(i+1,j,ARR);
	refresh();
	move(x_cur, y_cur);
	do
	{
		ch = getch();
		if ( ch == 127 ){
			if(j > 0){ // if text not at beg of line
				mvdelch(i, j--);
				mvdelch(x_cur, y_cur--);
				mvaddch(i, j, ARR);
				move(x_cur, y_cur);
			}
			else if (j == 0 && i > 0){ // if text at beg of line but not at 0th line
				mvdelch(i--, j);
				j = lineSize(i, fp)+1;
				mvaddch(i, j, ARR);
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
			else if (j == 0 && i == 0){ // curr at beg of doc
				continue;
			}
		}
		else {
			if (i <= lines)
			{
				if (ch != '\n')
				{
					mvdelch(i+1, j-1);
					if (i == 0)
						move(i+1, j);
					else move(i+2, j);
					printw("%c", ARR);
					j++;
				}
				else if (ch == '\n'){
					mvdelch(i+1, j-1);
					i++;
					move(i+2, 0);
					printw("%c", ARR);
					j++;					
				}
				else if (j == lnsize){
					j = 0;
					i++;
					lnsize = lineSize(i, fp);
				}
			}
			charcnt++;
			if (ch == '\n')
				move(y_cur+1, 0);
			else move(y_cur, x_cur+1);
		}
	} while (charcnt < flength);
	printw("\n%d", i);
	return 0;
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
	

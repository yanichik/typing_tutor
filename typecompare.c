#include <stdio.h>
#include <stdlib.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (x), (y))

int main(){
	FILE *fp = fopen("./challenges/samples/plato.txt", "r");
	char ch;
	long int i = 0, j = 0, lines, flength;
	while ((ch = fgetc(fp)) != EOF){
		if (ch == '\n'){
			i++;
			printf("\n");
		}
		printf("%c", ch);
	}
	fseek(fp, 0L, SEEK_END);
	flength = ftell(fp);
	lines = i;
	
	while (j < flength){
		if ( ((ch = getchar()) == 8) && (j > 0)){
			fflush(stdin);
			j--;
			gotoxy(i, j);
			putchar('94');
		}
		else {
			i = 1;
			if (i <= lines+1)
			{
				if (j < flength)
				{
					gotoxy(i, j);
					printf("%c", 94);
					j++;
				}
				i++;
			}
		}
	}
	printf("\n%d", i);
	return 0;
}

long int lineSize(int line, FILE *fp){
	char ch;
	long int i = 0, size = 0;
	while ( (ch = fgetc(fp)) != EOF){
		if (ch == '\n')
			i++;
		if (i==line){
			while( (ch = fgetc(fp)) != '\n')
				size++;
			break;
		}
	}
	return size;
}
	

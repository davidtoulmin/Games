#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void print_board(char square[]);
void change_board(char square[], int x_coord, int y_coord, char player);
void play(char square[], char player, int previous[][3]);
void win(char square[], char player);
int is_valid(int previous[][3], int x_coord, int y_coord);

#define num_plays 9

int
main(int argc, char *argv[]){
	char square[9]={' ',' ',' ',' ',' ',' ',' ',' ',' '};
	int previous[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int i;
	char c;
	print_board(square);
	for(i=0; i<num_plays; i++){
		if(i%2 == 0){
			play(square, 'X', previous);
		}else{
			play(square, 'O', previous);
		}
	}
	printf("\nIt's a draw!");
	printf("\nPress enter key to exit.");
	while((c=getchar())!='\n'){
		if(!isalpha(c)){
			do{
				c=fgetc(stdin);
			}while(c != '\n');
			break;
		}
	}
	if(scanf("%c", &c)){
		exit(EXIT_SUCCESS);
	}
	return 0;
}

void print_board(char square[]){
	printf("\n%c|%c|%c\n", square[0], square[1], square[2]);
	printf("-----\n");
	printf("%c|%c|%c\n", square[3], square[4], square[5]);
	printf("-----\n");
	printf("%c|%c|%c\n", square[6], square[7], square[8]);
}

void change_board(char square[], int x_coord, int y_coord, char player){
	if(x_coord==1){
		if(y_coord==1){
			square[0]=player;
		}else if(y_coord==2){
			square[1]=player;
		}else if(y_coord==3){
			square[2]=player;
		}
	}else if(x_coord==2){
		if(y_coord==1){
			square[3]=player;
		}else if(y_coord==2){
			square[4]=player;
		}else if(y_coord==3){
			square[5]=player;
		}
	}else if(x_coord==3){
		if(y_coord==1){
			square[6]=player;
		}else if(y_coord==2){
			square[7]=player;
		}else if(y_coord==3){
			square[8]=player;
		}
	}
}

void play(char square[], char player, int previous[][3]){
	int x_coord;
	int y_coord;
	printf("\n%c to play, choose a position x y: ", player);
	if(scanf("%d%d", &y_coord, &x_coord)!=2){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid play, input two numbers between 1 and 3, seperated by a space. \nTry again: ");
		play(square, player, previous);
	}
	else if(x_coord < 1 || x_coord > 3 || y_coord < 1 || y_coord > 3){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid play, input two numbers between 1 and 3, seperated by a space. \nTry again: ");
		play(square, player, previous);
	}else{
		if(is_valid(previous, x_coord, y_coord)){
			change_board(square, x_coord, y_coord, player);
			print_board(square);
			win(square, player);
		}else{
			play(square, player, previous);
		}
	}
}

void win(char square[], char player){
	int i;
	int winna=0;
	char c;
	for(i=0; i<=6; i+=3){
		if(square[i]==square[i+1] && square[i]==square[i+2]
							&& square[i]!=' '){
			winna=1;
		}
	}
	for(i=0; i<3; i++){
		if(square[i]==square[i+3] && square[i]==square[i+6]
							&& square[i]!=' '){
			winna=1;
		}
	}
	if(square[0]==square[4] && square[0]==square[8]	&& square[0]!=' '){
		winna=1;
	}
	if(square[2]==square[4] && square[2]==square[6]	&& square[2]!=' '){
		winna=1;
	}
	if(winna){
		printf("\n%c has won!", player);
		printf("\nPress enter key to exit.");
		while((c=getchar())!='\n'){
			if(!isalpha(c)){
				do{
					c=fgetc(stdin);
				}while(c != '\n');
				break;
			}
		}
		if(scanf("%c", &c)){
			exit(EXIT_SUCCESS);
		}
	}
}


int is_valid(int previous[][3], int x_coord, int y_coord){
	if(previous[x_coord-1][y_coord-1]){
		printf("That square has already been played. \nTry again: ");
		return 0;
	}else{
		previous[x_coord-1][y_coord-1]=1;
		return 1;
	}
}

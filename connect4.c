#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void print_board(char board[6][7]);
void play(char board[6][7], char player);
void win(char board[6][7], char player);
void end_game();
void full_board(char board[6][7]);

#define player1 'O'
#define player2 'X'

int main(int argc, char *argv[]){
	char board[6][7];
	int i, j;
	char player;
	for(i=0; i<6; i++){
		for(j=0; j<7; j++){
			board[i][j]=' ';
		}
	}
	print_board(board);
	while(1){
		if(i%2 == 0){
			player=player2;
		}else{
			player=player1;
		}
		full_board(board);
		play(board, player);
		print_board(board);
		win(board, player);
		i++;
	}
	return 1;
}

void print_board(char board[6][7]){
	int i, j, k;
	printf("\n ");
	for(k=0; k<29; k++){
		printf("-");
	}
	printf("\n");
	for(i=0; i<6; i++){
		printf(" |");
		for(j=0; j<7; j++){
			printf(" %c |", board[i][j]);
		}
		printf("\n ");
		for(k=0; k<29; k++){
			printf("-");
		}
		printf("\n");
	}
}

void play(char board[6][7], char player){
	int col, i, placed=0;
	char skip_ch;
	printf("\n%c to play, ", player);
	printf("choose a column to play in: ");
	if(scanf("%d", &col)!=1){
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid input, try again:\n");
		play(board, player);
		return;
	}
	if(col<1 || col>7){
		printf("Invalid play, try again:\n");
		play(board, player);
		return;
	}
	col--;
	for(i=5; i>=0; i--){
		if(board[i][col]==' '){
			board[i][col]=player;
			placed=1;
			break;
		}
	}
	if(!placed){
		printf("Full column, try again:\n");
		play(board, player);
		return;
	}
}

void win(char board[6][7], char player){
	int i, j;
	for(i=0; i<6; i++){
		for(j=0; j<7; j++){
			if(board[i][j]==player){
				/*printf("%c", board[i][j]);*/
				if(i<3){
					if(board[i+1][j]==player){
						if(board[i+2][j]==player){
							if(board[i+3][j]==player){
								/*printf("buh");*/
								printf("%c as won!", player);
								end_game();
							}
						}
					}
				}
				if(j<4){
					if(board[i][j+1]==player){
						/*printf("%c", board[i][j+1]);*/
						if(board[i][j+2]==player){
							/*printf("%c", board[i][j+2]);*/
							if(board[i][j+3]==player){
								/*printf("%c", board[i][j+3]);
								printf("sneh");*/
								printf("%c as won!", player);
								end_game();
							}
						}
					}
				}
				if(i<3 && j<4){
					if(board[i+1][j+1]==player){
						if(board[i+2][j+2]==player){
							if(board[i+3][j+3]==player){
								/*printf("gub");*/
								printf("%c as won!", player);
								end_game();
							}
						}
					}
				}
				if(i>2 && j<4){
					if(board[i-1][j+1]==player){
						if(board[i-2][j+2]==player){
							if(board[i-3][j+3]==player){
								/*printf("lup");*/
								printf("%c as won!", player);
								end_game();
							}
						}
					}
				}
			}
		}
	}
}

void end_game(){
	char c;
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

void full_board(char board[6][7]){
	int i, j, full=1;
	for(i=0; i<6; i++){
		for(j=0; j<7; j++){
			if(board[i][j]==' '){
				full=0;
			}
		}
	}
	if(full){
		printf("It's a draw!");
		end_game();
	}
}

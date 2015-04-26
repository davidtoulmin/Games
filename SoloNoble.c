#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void print_board(char board[7][7]);
void change_board(char board[7][7], int x_coord, int y_coord, int new_x, int new_y);
int play(char board[7][7]);
int is_valid(char board[7][7], int x_coord, int y_coord, int new_x, int new_y);
int is_piece(char board[7][7], int x_coord, int y_coord);
int count_pieces(char board[7][7]);
int available_moves(char board[7][7]);
void end(char board[7][7]);
int is_valid_am(char board[7][7], int x_coord, int y_coord, int new_x, int new_y);

#define PIECE 'O'
#define NA 'X'
#define EMPTY ' '

/*Initialises the board, manages play*/
int
main(int argc, char *argv[]){
	char board[7][7]={{'X','X','O','O','O','X','X'},
			  {'X','X','O','O','O','X','X'},
			  {'O','O','O','O','O','O','O'},
			  {'O','O','O',' ','O','O','O'},
			  {'O','O','O','O','O','O','O'},
			  {'X','X','O','O','O','X','X'},
			  {'X','X','O','O','O','X','X'}};
	print_board(board);
	/*Infinite while loop, not great, but only option(?)*/ 
	while(1){
		play(board);
		print_board(board);
	}
	printf("Something's gone wrong");
	return 0;
}

/*This one is pretty self explanitory, it prints the board to the console*/
void print_board(char board[7][7]){
	int i, j; 
	printf("\n      -------\n");
	for(i=6; i>-1; i--){
		printf("%d ", i+1);
		for(j=0; j<7; j++){
			if(board[i][j]==NA){
				if(j>4){
					break;
				}else{
					printf("  ");
				}
			}else{
				printf("|%c", board[i][j]);
			}
		}
		if(i>1 && i<6){
			printf("|\n  ---------------\n");
		}else{
			printf("|\n      -------\n");
		}
	}
	printf("   1 2 3 4 5 6 7\n\n");
}

/*Everything involved in the player's move*/
int play(char board[7][7]){
	int x_coord;
	int y_coord;
	int new_x, new_y;
	printf("Choose a piece to play, with coords x y: ");
	if(scanf("%d%d", &x_coord, &y_coord)!=2){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid input, try again:\n");
		play(board);
		return 1;
	}
	if(x_coord < 1 || x_coord > 7 || y_coord < 1 || y_coord > 7){
		printf("Invalid play, try again:\n");
		play(board);
		return 1;
	}
	x_coord--;
	y_coord--;
	printf("You have selected position (%d,%d)\n", x_coord+1, y_coord+1);
	if(is_piece(board, x_coord, y_coord)){
		printf("Choose a place to move, with coords x y, enter negative coords to pick another piece: ");
		if(scanf("%d%d", &new_x, &new_y)!=2){
			char skip_ch;
			do{
				scanf("%c",&skip_ch);
			}while(skip_ch != '\n');
			printf("Invalid input, try again:\n");
			play(board);
			return 1;
		}
		if(new_x < 1 && new_y < 1){
			printf("Piece deselected.\n");
			return 0;
		}
		if(new_x < 1 || new_x > 7 || new_y < 1 || new_y > 7){
			printf("Invalid play, try again:\n");
			play(board);
			return 1;
		}
		/*nasty switching between values of input and values worked with within the program, fix this up for readability*/
		new_x--;
		new_y--;
		if(is_valid(board, x_coord, y_coord, new_x, new_y)){
			change_board(board, x_coord, y_coord, new_x, new_y);
		}
	} else {
		printf("Not a piece, try again:\n");
		play(board);
	}
	return 0;
}

/*Checks whether the chosen position is a piece*/
int is_piece(char board[7][7], int x_coord, int y_coord){
	if(board[y_coord][x_coord]!=PIECE){
		return 0;
	}
	return 1;
}

/*Updates the board with the new move, 
(effectively makes the move requested)*/
void change_board(char board[7][7], int x_coord, int y_coord, int new_x, int new_y){
	board[new_y][new_x]=PIECE;
	board[y_coord][x_coord]=EMPTY;
	board[(new_y + y_coord)/2][(new_x + x_coord)/2] = EMPTY;
	if(!available_moves(board)){
		print_board(board);
		end(board);
	}
}

/*Checks whether the given move is legal, based on possitions,
whether it would involve taking your own piece and whether it would
involve jumping over another piece*/
int is_valid(char board[7][7], int x_coord, int y_coord, int new_x, int new_y){
	if(x_coord==new_x && y_coord==new_y){
		printf("Can't play in the same place, try again:\n");
		play(board);
		return 0;
	}
	if(board[new_y][new_x]==NA){
		printf("Invalid play, try again:\n");
		play(board);
		return 0;
	}
	if(board[new_y][new_x]==PIECE){
		printf("Can't play on another piece, try again:\n");
		play(board);
		return 0;
	}
	if(board[(new_y + y_coord)/2][(new_x + x_coord)/2]==EMPTY){
		printf("You must jump another piece to move, try again:\n");
		play(board);
		return 0;
	}
	if(!(    ((abs(x_coord-new_x)==2) && (y_coord==new_y))
	      || ((abs(y_coord-new_y)==2) && (x_coord==new_x)))){
		printf("You must jump one piece over another (horizontally or vertically) to move, try again:\n");
		play(board);
		return 0;
	}
	if(board[y_coord][x_coord]==PIECE){
		if(((x_coord-new_x==0) && (abs(y_coord-new_y)==2))
		|| ((y_coord-new_y==0) && (abs(x_coord-new_x)==2))){
			return 1;
		}
	}
	printf("Unknown illegal move, try again:\n");
	play(board);
	return 0;
}

/*Counts the number of remaining pieces*/
int count_pieces(char board[7][7]){
	int i, j, num_pieces=0;
	for(i=0; i<7; i++){
		for(j=0; j<7; j++){
			if(board[i][j]==PIECE){
				num_pieces++;
			}
		}
	}
	return num_pieces;
}

int available_moves(char board[7][7]){
	int x, y, new_x, new_y;
	for(x=0; x<7; x++){
		for(y=0; y<7; y++){
			if(is_piece(board, x, y)){
				for(new_x=0; new_x<7; new_x++){
					for(new_y=0; new_y<7; new_y++){
						if(is_valid_am(board, x, y, new_x, new_y)){
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

void end(char board[7][7]){
	char c;
	printf("Game over, you have %d piece(s) remaining\n", count_pieces(board));
	printf("Press enter key to exit.");
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

/* Same as is_valid but without prompts or replays, used for checking available
 * moves
 */
int is_valid_am(char board[7][7], int x_coord, int y_coord, int new_x, int new_y){
	if(x_coord==new_x && y_coord==new_y){
		return 0;
	}
	if(board[new_y][new_x]==NA){
		return 0;
	}
	if(board[new_y][new_x]!=EMPTY){
		return 0;
	}
	if(board[(new_y + y_coord)/2][(new_x + x_coord)/2]==EMPTY){
		return 0;
	}
	if(!(    ((abs(x_coord-new_x)==2) && (y_coord==new_y))
	      || ((abs(y_coord-new_y)==2) && (x_coord==new_x)))){
		return 0;
	}
	if(board[y_coord][x_coord]=='O'){
		if(((x_coord-new_x==0) && (abs(y_coord-new_y)==2))
		|| ((y_coord-new_y==0) && (abs(x_coord-new_x)==2))){
			return 1;
		}
	}
	return 0;
}

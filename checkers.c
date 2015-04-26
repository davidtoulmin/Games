#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void print_board(char board[8][8]);
void change_board(char board[8][8], int x_coord, int y_coord, int new_x, int new_y, char player);
int play(char board[8][8], char player);
int second_jump(char board[8][8], char player, int x_coord, int y_coord);
int is_valid(char board[8][8], int x_coord, int y_coord, int new_x, int new_y, char player, int is_second_jump);
int is_piece(char board[8][8], int x_coord, int y_coord, char player);
int find_piece(char board[8][8], char player);
void end(char player);

#define BLACK 'b'
#define WHITE 'w'
#define BLACK_KING 'B'
#define WHITE_KING 'W'
#define EMPTY ' '

/*Does a whole bunch of stuff, including initialising the board, choosing whose
turn it is and seeing if it's check mate*/
int
main(int argc, char *argv[]){
	char board[8][8]={{'w',' ','w',' ','w',' ','w',' '},
			   {' ','w',' ','w',' ','w',' ','w'},
			   {'w',' ','w',' ','w',' ','w',' '},
			   {' ',' ',' ',' ',' ',' ',' ',' '},
			   {' ',' ',' ',' ',' ',' ',' ',' '},
			   {' ','b',' ','b',' ','b',' ','b'},
			   {'b',' ','b',' ','b',' ','b',' '},
			   {' ','b',' ','b',' ','b',' ','b'}};
	print_board(board);
	/*Infinite while loop, not great, but only option?*/ 
	while(1){
		play(board, WHITE);
		print_board(board);
		play(board, BLACK);
		print_board(board);
	}
	printf("Something's gone wrong");
	return 0;
}

/*This one is pretty self explanitory, it prints the board to the console*/
void print_board(char board[8][8]){
	int i, j;
	printf("\n  -----------------\n");
	for(i=7; i>-1; i--){
		printf("%d ", i+1);
		for(j=0; j<8; j++){
			printf("|%c", board[i][j]);
		}
		printf("|\n  -----------------\n");
	}
	printf("   1 2 3 4 5 6 7 8\n\n");
}

/*Everything involved in an individual player's turn*/
int play(char board[8][8], char player){
	int x_coord;
	int y_coord;
	int new_x, new_y;
	printf("%c to play, choose a piece to play, with coords x y: ", player);
	if(scanf("%d%d", &x_coord, &y_coord)!=2){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid input, try again:\n");
		play(board, player);
		return 1;
	}
	if(x_coord < 1 || x_coord > 8 || y_coord < 1 || y_coord > 8){
		printf("Invalid play, try again:\n");
		play(board, player);
		return 1;
	}
	x_coord--;
	y_coord--;
	printf("You have selected position (%d,%d)\n", x_coord+1, y_coord+1);
	if(is_piece(board, x_coord, y_coord, player)){
		printf("Choose a place to move, with coords x y, enter negative coords to skip your turn: ");
		if(scanf("%d%d", &new_x, &new_y)!=2){
			char skip_ch;
			do{
				scanf("%c",&skip_ch);
			}while(skip_ch != '\n');
			printf("Invalid input, try again:\n");
			play(board, player);
			return 1;
		}
		if(new_x < 1 && new_y < 1){
			printf("Turn skipped.\n");
			return 0;
		}
		if(new_x < 1 || new_x > 8 || new_y < 1 || new_y > 8){
			printf("Invalid play, try again:\n");
			play(board, player);
			return 1;
		}
		/*nasty switching between values of input and values worked with within the program, fix this up for readability*/
		new_x--;
		new_y--;
		if(is_valid(board, x_coord, y_coord, new_x, new_y, player, 0)){
			change_board(board, x_coord, y_coord, new_x, new_y, player);
		}
	} else {
		play(board, player);
	}
	return 0;
}

int second_jump(char board[8][8], char player, int x_coord, int y_coord){
	int new_x, new_y;
	printf("Successful take made, take another piece, or end turn by entering negative coordinates\n");
	printf("Choose a place to move, with coords x y: ");
	if(scanf("%d%d", &new_x, &new_y)!=2){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid input, try again:\n");
		second_jump(board, player, x_coord, y_coord);
		return 1;
	}
	if(new_x < 1 && new_y < 1){
		printf("No further jumps taken\n");
		return 0;
	}
	if(new_x < 1 || new_x > 8 || new_y < 1 || new_y > 8){
		printf("Invalid play, try again:\n");
		second_jump(board, player, x_coord, y_coord);
		return 1;
	}
	/*nasty switching between values of input and values worked with within the program, fix this up for readability*/
	new_x--;
	new_y--;
	if(is_valid(board, x_coord, y_coord, new_x, new_y, player, 1)){
		change_board(board, x_coord, y_coord, new_x, new_y, player);
	}
	return 0;
}

/*Checks whether the chosen position is one of that player's pieces*/
int is_piece(char board[8][8], int x_coord, int y_coord, char player){
	if(player == BLACK){
		if(board[y_coord][x_coord]!=BLACK && board[y_coord][x_coord]!=BLACK_KING){
			printf("Not your piece, try again:\n");
			return 0;
		}
	}else if(player == WHITE){
		if(board[y_coord][x_coord]!=WHITE && board[y_coord][x_coord]!=WHITE_KING){
			printf("Not your piece, try again:\n");
			return 0;
		}
	}
	return 1;
}

/*Updates the board with the new move, 
(effectively makes the move requested)*/
void change_board(char board[8][8], int x_coord, int y_coord, int new_x, int new_y, char player){
	board[new_y][new_x]=board[y_coord][x_coord];
	board[y_coord][x_coord]=EMPTY;
	if((new_y==0)&&(player == BLACK)){
		board[new_y][new_x] = BLACK_KING;
	} else if((new_y==7)&&(player == WHITE)){
		board[new_y][new_x] = WHITE_KING;
	}
	if(abs(new_y-y_coord)>1){
		board[(new_y + y_coord)/2][(new_x + x_coord)/2] = EMPTY;
		print_board(board);
		if(find_piece(board, player)){
				end(player);
			}
		second_jump(board, player, new_x, new_y);
	}
}

/*Checks whether the given move is legal, based on possitions,
whether it would involve taking your own piece and whether it would
involve jumping over another piece*/
int is_valid(char board[8][8], int x_coord, int y_coord, int new_x, int new_y, char player, int is_second_jump){
	if(x_coord==new_x && y_coord==new_y){
		printf("Can't play in the same place, try again:\n");
		if(is_second_jump){
			second_jump(board, player, x_coord, y_coord);
		}else{
			play(board, player);
		}
		return 0;
	}
	if(board[new_y][new_x]!=EMPTY){
		printf("Can't play on another piece, try again:\n");
		if(is_second_jump){
			second_jump(board, player, x_coord, y_coord);
		}else{
			play(board, player);
		}
		return 0;
	}
	if(board[y_coord][x_coord]=='B' || board[y_coord][x_coord]=='W'){
		if(!(((abs(x_coord-new_x)<=1) && (abs(y_coord-new_y)<=1) && !is_second_jump)
		|| ((abs(x_coord-new_x)==2) && (abs(y_coord-new_y)==2)
			&& toupper(board[(new_y + y_coord)/2][(new_x + x_coord)/2]) != toupper(board[y_coord][x_coord])
			&& board[(new_y + y_coord)/2][(new_x + x_coord)/2] != EMPTY))){
				printf("That peice can't move like that, try again:\n");
				if(is_second_jump){
					second_jump(board, player, x_coord, y_coord);
				}else{
					play(board, player);
				}
				return 0;
		}
	}else if(board[y_coord][x_coord]=='b'){
		if(!(((abs(x_coord-new_x)==1) && (y_coord-new_y==1) && !is_second_jump)
		|| ((abs(x_coord-new_x)==2) && (y_coord-new_y==2)
			&&( board[(new_y + y_coord)/2][(new_x + x_coord)/2] == WHITE
			|| board[(new_y + y_coord)/2][(new_x + x_coord)/2] == WHITE_KING)))){
			printf("That peice can't move like that, try again:\n");
			if(is_second_jump){
				second_jump(board, player, x_coord, y_coord);
			}else{
				play(board, player);
			}
			return 0;
		}
	}else if(board[y_coord][x_coord]=='w'){
		printf("%c %d %d", board[(new_y + y_coord)/2][(new_x + x_coord)/2], (new_y + y_coord)/2+1, (new_x + x_coord)/2+1);
		if(!(((abs(x_coord-new_x)==1) && (y_coord-new_y==-1) && !is_second_jump)
		|| ((abs(x_coord-new_x)==2) && (y_coord-new_y==-2)
			&&( board[(new_y + y_coord)/2][(new_x + x_coord)/2] == BLACK
			|| board[(new_y + y_coord)/2][(new_x + x_coord)/2] == BLACK_KING)))){
			printf("That peice can't move like that, try again:\n");
			if(is_second_jump){
				second_jump(board, player, x_coord, y_coord);
			}else{
				play(board, player);
			}
			return 0;
		}
	}
	return 1;
}

/*Checks if one player is out of pieces*/
int find_piece(char board[8][8], char player){
	int i, j;
	if(player == BLACK){
		for(i=0; i<8; i++){
			for(j=0; j<8; j++){
				if(board[i][j]==WHITE || board[i][j]==WHITE_KING){
					return 0;
				}
			}
		}
		return 1;
	}else if(player == WHITE){
		for(i=7; i>-1; i--){
			for(j=7; j>-1; j--){
				if(board[i][j]==BLACK || board[i][j]==BLACK_KING){
					return 0;
				}
			}
		}
		return 1;
	}
	printf("ERROR: That's not a valid player value\n");
	return 1;
}

void end(char player){
	char c;
	if(player==BLACK){
		printf("Black");
	}else{
		printf("White");
	}
	printf(" has won, congratulations\n");
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

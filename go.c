/* To add; Ko checking (make it illegal to reproduce a previous board), and
 * add end of game scoring, possibly also adding a count of how many pieces
 * were taken
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
	char square[19][19];
	char valency_map[19][19];
	char player;
	int x_coord;
	int y_coord;
	int skipped;
} board_t;

void print_board(char square[19][19]);
int play(board_t *board);
int valency_check(board_t *board);
void check_adjacent(board_t *board, int i, int j, int *found, int *free_flag);
void check_square(board_t *board, int i, int j, int di, int dj, int *found, int *free_flag);

#define BLACK '@'
#define WHITE 'O'
#define EMPTY '|'
#define FREE_B 'B'
#define FREE_W 'W'
#define TRAP_B 'b'
#define TRAP_W 'w'

int
main(int argc, char *argv[]){
	board_t board;
	int i, j, free_flag, trash, handicap;
	for(i=0; i<19; i++){
		for(j=0; j<19; j++){
			board.square[i][j]=EMPTY;
		}
	}
	board.skipped=0;
	board.player=BLACK;
	printf("Choose a handicap: ");
	while(scanf("%d", &handicap)!=1){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid handicap, try again: ");
	}
	for(j=0; j<handicap; j++){
		print_board(board.square);
		play(&board);
	}
	for(i=0; 1; i++){
		free_flag=1;
		if(i%2 == 0){
			board.player=BLACK;
			print_board(board.square);
			play(&board);
			board.player=WHITE;
			check_adjacent(&board, board.x_coord, board.y_coord, &trash, &free_flag);
			if(!free_flag){
				board.square[board.y_coord][board.x_coord]=EMPTY;
				printf("Can't play there it's suicide");
				i--;
				/*this effectively starts that turn again*/
			}
		}else{
			board.player=WHITE;
			print_board(board.square);
			play(&board);
			board.player=BLACK;
			check_adjacent(&board, board.x_coord, board.y_coord, &free_flag, &free_flag);
			if(!free_flag){
				board.square[board.y_coord][board.x_coord]=EMPTY;
				printf("Can't play there it's suicide");
				i--;
				/*this effectively starts that turn again*/
			}
		}
	}
	return 0;
}

void print_board(char square[19][19]){
	int i, j;
	printf("\n");
	printf("    - - - - - - - - - 1 1 1 1 1 1 1 1 1 1\n");
	printf("    1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9\n");
	for(i=19; i>0; i--){
		printf("%3.d ", i);
		for(j=0; j<18; j++){
			printf("%c-", square[i-1][j]);
		}
		printf("%c", square[i-1][j]);
		printf("%3.d\n", i);

	}
	printf("    - - - - - - - - - 1 1 1 1 1 1 1 1 1 1\n");
	printf("    1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9\n\n");
}

int play(board_t *board){
	char c;
	/*int i, j;*/
	printf("If you wish to skip, enter negative values, otherwise,\n");
	printf("%c to play, choose a place to play, with coords x y: ", board->player);
	if(scanf("%d%d", &(board->x_coord), &(board->y_coord))!=2){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid input, try again:\n");
		play(board);
		return 1;
	}
	if(board->x_coord < 1 || board->y_coord < 1){
		if(board->skipped==1){
			printf("End of game!\n");
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
		board->skipped=1;
		printf("Turn skipped");
		return 1;
	}
	board->skipped=0;
	if(board->x_coord > 19 || board->y_coord > 19){
		printf("Invalid play, try again:\n");
		play(board);
		return 1;
	}
	(board->x_coord)--;
	(board->y_coord)--;
	if((board->square)[board->y_coord][board->x_coord]!=EMPTY){
		printf("Can't play over another piece, try again:\n");
		play(board);
		return 1;
	} else {
		(board->square)[board->y_coord][board->x_coord]=(board->player);
	}
	valency_check(board);
	return 0;
}

int valency_check(board_t *board){
	/*check the whole board, documenting a value for each place, as to 
	whether it has connections to a valency, saving a value in the respective
	place in board->valency_map*/
	
	/*Currently doesn't work for having three pieces in the corner, because
	the freeing doesn't flow through it well enough, a zig-zag line will make
	things very difficult*/ /*have a flag and go through untill the flag that
	something has been changed stops being raised*/
	int i, j, found=1, trash;
	for(i=0; i<19; i++){
		for(j=0; j<19; j++){
			board->valency_map[j][i]=EMPTY;
		}
	}
	while(found){
		found=0;
		for(i=0; i<19; i++){
			for(j=0;j<19; j++){
				check_adjacent(board, i, j, &found, &trash);
			}
		}
		for(i=18; i>-1; i--){
			for(j=18; j>-1; j--){
				check_adjacent(board, i, j, &found, &trash);
			}
		}
	}
	found=0;
	for(i=0; i<19; i++){
		for(j=0; j<19; j++){
			if(board->player==BLACK){
				if(board->valency_map[j][i]==TRAP_W){
					board->square[j][i]=EMPTY;
					board->valency_map[j][i]=EMPTY;
					found=1;
				}
			} else {
				if(board->valency_map[j][i]==TRAP_B){
					board->square[j][i]=EMPTY;
					board->valency_map[j][i]=EMPTY;
					found=1;
				}
			}
		}
	}
	return found;
	/*int space=0;
	if(board->square[board->y_coord+1][board->x_coord+1]==EMPTY){
		something=space*/
}

void check_adjacent(board_t *board, int i, int j, int *found, int *free_flag){
	if(i-1>=0 && board->valency_map[j][i]!=FREE_B && board->valency_map[j][i]!=FREE_W){
		check_square(board, i, j, -1, 0, found, free_flag);
	}
	if(i+1<19 && board->valency_map[j][i]!=FREE_B && board->valency_map[j][i]!=FREE_W){
		check_square(board, i, j, 1, 0, found, free_flag);
	}
	if(j-1>=0 && board->valency_map[j][i]!=FREE_B && board->valency_map[j][i]!=FREE_W){
		check_square(board, i, j, 0, -1, found, free_flag);
	}
	if(j+1<19 && board->valency_map[j][i]!=FREE_B && board->valency_map[j][i]!=FREE_W){
		check_square(board, i, j, 0, 1, found, free_flag);
	}
}

void check_square(board_t *board, int i, int j, int di, int dj, int *found, int *free_flag){
	if(board->square[j][i]==BLACK){
		if(board->square[j+dj][i+di]==EMPTY ||
		   board->valency_map[j+dj][i+di]==FREE_B){
			board->valency_map[j][i]=FREE_B;
			*found=1;
		} else {
			board->valency_map[j][i]=TRAP_B;
			*free_flag=0;
		}
	} else if(board->square[j][i]==WHITE){
		if(board->square[j+dj][i+di]==EMPTY ||
		   board->valency_map[j+dj][i+di]==FREE_W){
			board->valency_map[j][i]=FREE_W;
			*found=1;
		} else {
			board->valency_map[j][i]=TRAP_W;
			*free_flag=0;
		}
	}
}

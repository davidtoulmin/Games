#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void print_board(char square[8][8]);
void change_board(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player);
int play(char square[8][8], char *player, int pawn_move[2]);
/*Left over function from TicTacToe, couldn't bring myself to delete it.
void win(char square[8][8], char *player);*/
int is_valid(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player, int pawn_move[2]);
int is_piece(char square[8][8], int x_coord, int y_coord, char *player);
int anything_inbetween(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player);
int check(char square[8][8], char *player);
int find_king(char square[8][8], char *player, int king[2]);
void w_o_o(char square[8][8], char *player);
int check_mate(char square[8][8], char *player);
int anything_inbetween_cm(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player);
int is_valid_cm(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player);

/*#define num_plays 9*/
#define lower_player "Lower Case"
#define upper_player "Upper Case"
#define not_double_move -10

/*Does a whole bunch of stuff, including initialising the board, choosing whose
turn it is and seeing if it's check mate*/
int
main(int argc, char *argv[]){
	char square[8][8]={{'R','N','B','K','Q','B','N','R'},
			   {'P','P','P','P','P','P','P','P'},
			   {' ',' ',' ',' ',' ',' ',' ',' '},
			   {' ',' ',' ',' ',' ',' ',' ',' '},
			   {' ',' ',' ',' ',' ',' ',' ',' '},
			   {' ',' ',' ',' ',' ',' ',' ',' '},
			   {'p','p','p','p','p','p','p','p'},
			   {'r','n','b','k','q','b','n','r'}};
	/*A leftover from TicTacToe
	int previous[3][3] = {{0,0,0},{0,0,0},{0,0,0}};*/
	/*int check=0;*/
	int i;
	char c;
	int pawn_move[2]={not_double_move, not_double_move};
	/*char mate;*/
	/*char skip_ch=0;*/
	/*Meaninglessly using skip_ch here to stop the compiler bugging me
	about supposedly not using the variable*/
	/*skip_ch++;*/
	print_board(square);
	/*Infinite for loop, not great, but only option?*/ 
	for(i=0; /*!win*/; i++){
		if(i%2 == 0){
			play(square, upper_player, pawn_move);
			print_board(square);
			if(check(square, lower_player)){
				printf("%s is in check!\n", lower_player);
				/*My cheeky way of getting around
				having to calculate check mate, before I
				wrote the code for check_mate*/
				/*printf("Is it checkmate? Y/N:");
				skip_ch = fgetc(stdin);
				if(scanf("%c", &mate)!=1){
					printf("Invalid input, abort");
					exit(EXIT_FAILURE);
				}
				if(mate=='Y'){*/
				if(check_mate(square, lower_player)){
					printf("In fact it's checkmate!\n\nCongratulations %s, you won!\n", upper_player);
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
					printf("Something's gone wrong");
				}
			}else{
				/*check_mate() here being used to calculate if it's stalemate*/
				check_mate(square, lower_player);
			}
		}else{
			play(square, lower_player, pawn_move);
			print_board(square);
			if(check(square, upper_player)){
				printf("%s is in check!\n", lower_player);
				/*printf("Is it checkmate? Y/N:");
				skip_ch = fgetc(stdin);
				if(scanf("%c", &mate)!=1){
					printf("Invalid input, abort");
					exit(EXIT_FAILURE);
				}
				if(mate=='Y'){*/
				if(check_mate(square, upper_player)){
					printf("\nIn fact it's checkmate!\n\nCongratulations %s, you won!\n", lower_player);
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
					printf("Something's gone wrong");
				}
			}else{
				/*check_mate() here being used to calculate if it's stalemate*/
				check_mate(square, upper_player);
			}
		}
	}
	printf("\nIt's a draw!\n");
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
	printf("Something's gone wrong");
	return 0;
}

/*This one is pretty self explanitory, it prints the board to the console*/
void print_board(char square[8][8]){
	int i, j;
	printf("\n  -----------------\n");
	for(i=7; i>-1; i--){
		printf("%d ", i+1);
		for(j=0; j<8; j++){
			printf("|%c", square[i][j]);
		}
		printf("|\n  -----------------\n");
	}
	printf("   1 2 3 4 5 6 7 8\n\n");
}

/*Everything involved in an individual player's turn*/
int play(char square[8][8], char *player, int pawn_move[2]){
	int x_coord;
	int y_coord;
	int new_x, new_y;
	char taken_piece = ' ';
	/*printf("pawn_move is {%d,%d}\n", pawn_move[0], pawn_move[1]);*/
	printf("%s to play, choose a piece to play, with coords x y: ", player);
	if(scanf("%d%d", &x_coord, &y_coord)!=2){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid input, try again:\n");
		play(square, player, pawn_move);
		return 1;
	}
	if(x_coord==42 && y_coord==42){
		w_o_o(square, player);
		return 0;
	}
	if(x_coord < 1 || x_coord > 8 || y_coord < 1 || y_coord > 8){
		printf("Invalid play, try again:\n");
		play(square, player, pawn_move);
		return 1;
	}
	x_coord--;
	y_coord--;
	printf("Your peice is '%c', at (%d,%d)\n", square[y_coord][x_coord], x_coord+1, y_coord+1);
	if(is_piece(square, x_coord, y_coord, player)){
		printf("Choose a place to move, with coords x y: ");
		if(scanf("%d%d", &new_x, &new_y)!=2){
			char skip_ch;
			do{
				scanf("%c",&skip_ch);
			}while(skip_ch != '\n');
			printf("Invalid input, try again:\n");
			play(square, player, pawn_move);
			return 1;
		}
		if(new_x < 1 || new_x > 8 || new_y < 1 || new_y > 8){
			printf("Invalid play, try again:\n");
			play(square, player, pawn_move);
			return 1;
		}
		/*nasty switching between values of input and values worked with within the program, fix this up for readability*/
		new_x--;
		new_y--;
		if(is_valid(square, x_coord, y_coord, new_x, new_y, player, pawn_move)){
			taken_piece=square[new_y][new_x];
			change_board(square, x_coord, y_coord, new_x, new_y, player);
			if(check(square, player)){
				print_board(square);
				printf("You're in check now, you can't do that!\n");
				if(square[new_y][new_x]=='p' && (new_x==pawn_move[0])&&(new_y==pawn_move[1]-1)&&(square[pawn_move[1]][pawn_move[0]]==' ')){
					square[pawn_move[1]][pawn_move[0]]='P';
				} else if(square[new_y][new_x]=='P' && (new_x==pawn_move[0])&&(new_y==pawn_move[1]+1)&&(square[pawn_move[1]][pawn_move[0]]==' ')){
					square[pawn_move[1]][pawn_move[0]]='p';
				}
				change_board(square, new_x, new_y, x_coord, y_coord, player);
				square[new_y][new_x]=taken_piece;
				print_board(square);
				printf("Try again:\n");
				play(square, player, pawn_move);
			}else{
				if((new_y==7)&&(player[0]=='U')&&(square[new_y][new_x]=='P')){
						square[new_y][new_x]='Q';
				} else if((new_y==0)&&(player[0]=='L')&&(square[new_y][new_x]=='p')){
						square[new_y][new_x]='q';
				}
				if(square[new_y][new_x]=='P' && (new_y == y_coord+2) && (y_coord==1)){
					pawn_move[0] = new_x;
					pawn_move[1] = new_y;
				} else if(square[new_y][new_x]=='p' && (new_y == y_coord-2) && (y_coord==6)){
					pawn_move[0] = new_x;
					pawn_move[1] = new_y;
				} else {
					pawn_move[0] = not_double_move;
					pawn_move[1] = not_double_move;
				}
				/*win(square, player);*/
			}
		}
	} else {
		play(square, player, pawn_move);
	}
	return 0;
}

/*Checks whether the chosen position is one of that player's pieces*/
int is_piece(char square[8][8], int x_coord, int y_coord, char *player){
	if(player[0]=='L'){
		if(!(islower(square[y_coord][x_coord]))){
			printf("Not your piece, try again:\n");
			return 0;
		}
	}else if(player[0]=='U'){
		if(!(isupper(square[y_coord][x_coord]))){
			printf("Not your piece, try again:\n");
			return 0;
		}
	}
	return 1;
}

/*Updates the board with the new move, 
(effectively makes the move requested)*/
void change_board(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player){
	square[new_y][new_x]=square[y_coord][x_coord];
	square[y_coord][x_coord]=' ';
}

/*Checks whether the given move is legal, based on types of moves for that
piece, whether it would involve taking your own piece and whether it would
involve jumping over another piece (last one done through call to 
anything_inbetween)*/
int is_valid(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player, int pawn_move[2]){
	if(x_coord==new_x && y_coord==new_y){
		printf("Can't play in the same place, try again:\n");
			play(square, player, pawn_move);
			return 0;
	}
	if(player[0]=='U'){
		if(isupper(square[new_y][new_x])){
			printf("Can't take your own piece, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}else if(player[0]=='L'){
		if(islower(square[new_y][new_x])){
			printf("Can't take your own piece, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}
	if(toupper(square[y_coord][x_coord])=='R'){
		if( !(
			(x_coord == new_x && y_coord != new_y)
			||
			(y_coord == new_y && x_coord != new_x)
		)){
			printf("That peice can't move like that, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}else if(toupper(square[y_coord][x_coord])=='Q'){
		if( !(
			(x_coord == new_x && y_coord != new_y)
			||
			(y_coord == new_y && x_coord != new_x)
			||
			(abs(x_coord-new_x) == abs(y_coord-new_y))
		)){
			printf("That peice can't move like that, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}else if(toupper(square[y_coord][x_coord])=='K'){
		if(!((abs(x_coord-new_x)<=1) && (abs(y_coord-new_y)<=1))){
			printf("That peice can't move like that, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}else if(toupper(square[y_coord][x_coord])=='B'){
		if( !((abs(x_coord-new_x)) == (abs(y_coord-new_y)))){
			printf("That peice can't move like that, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}else if(square[y_coord][x_coord]=='P'){
		if( !(
			(
					(new_x==x_coord)
				&&
					(square[new_y][new_x]==' ')
				&&
					(	
						(new_y == y_coord+1)
					||
						(
						(new_y == y_coord+2)
						&&
						(y_coord==1)
						)
					)
			)
			||
			(
					(abs(new_x-x_coord) == 1)
				&&
					(new_y == y_coord+1)
				&&
					((square[new_y][new_x]!=' ')
					||
					((new_x==pawn_move[0])&&(new_y==pawn_move[1]+1)&&(square[pawn_move[1]][pawn_move[0]]=' ')))
			)
		)){
			printf("That peice can't move like that, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}else if(square[y_coord][x_coord]=='p'){
		if( !(
			(
					(new_x==x_coord)
				&&
					(square[new_y][new_x]==' ')
				&&
					(	
						(new_y == y_coord-1)
					||
						(
						(new_y == y_coord-2)
						&&
						(y_coord==6)
						)
					)
			)
			||
			(
					(abs(new_x-x_coord) == 1)
				&&
					(new_y == y_coord-1)
				&&
					((square[new_y][new_x]!=' ')
					||
					((new_x==pawn_move[0])&&(new_y==pawn_move[1]-1)&&(square[pawn_move[1]][pawn_move[0]]=' ')))
			)
		)){
			printf("That peice can't move like that, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}else if(toupper(square[y_coord][x_coord])=='N'){
		if( !(
			((abs(x_coord-new_x)==2) && (abs(y_coord-new_y)==1))
			||
			((abs(x_coord-new_x)==1) && (abs(y_coord-new_y)==2))
		)){
			printf("That peice can't move like that, try again:\n");
			play(square, player, pawn_move);
			return 0;
		}
	}
	if(anything_inbetween(square, x_coord, y_coord, new_x, new_y, player)){
		play(square, player, pawn_move);
		return 0;
	}
	return 1;
}

/*Calculates whether the given move is legal based on whether it would involve
jumping over another piece*/
int anything_inbetween(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player){
	int i, j;
	int min_x, max_x, min_y, max_y;
	if(x_coord>=new_x){
		min_x=new_x;
		max_x=x_coord;
	}else{
		min_x=x_coord;
		max_x=new_x;
	}
	if(y_coord>=new_y){
		min_y=new_y;
		max_y=y_coord;
	}else{
		min_y=y_coord;
		max_y=new_y;
	}
	if(x_coord==new_x){
		for(i=min_y+1; i<max_y; i++){
			if(square[i][x_coord]!=' '){
				printf("Can't jump over another peice, try again:\n");
				return 1;
			}
		}
	}else if(y_coord==new_y){
		for(i=min_x+1; i<max_x; i++){
			if(square[y_coord][i]!=' '){
				printf("Can't jump over another peice, try again:\n");
				return 1;
			}
		}
	}
	else if((abs(x_coord-new_x)) == (abs(y_coord-new_y))){
		if((x_coord>new_x && y_coord>new_y) || (x_coord<new_x && y_coord<new_y)){
			for(i=min_y+1, j=min_x+1; i<max_y && j<max_x; i++, j++){
				if(square[i][j]!=' '){
					printf("Can't jump over another peice, try again:\n");
					return 1;
				}
			}
		}
	}else if((abs(x_coord-new_x)) == (abs(y_coord-new_y))){
		if((x_coord<new_x && y_coord>new_y) || (x_coord>new_x && y_coord<new_y)){
			for(i=max_y-1, j=min_x+1; i>min_y && j<max_x; i--, j++){
				if(square[i][j]!=' '){
					printf("Can't jump over another peice, try again:\n");
					return 1;
				}
			}
		}
	}
	return 0;
}

/*Calculates whether player is in check*/
int check(char square[8][8], char *player){
	int i, j, check;
	int king[2];
	char c;
	if(find_king(square, player, king)){
		printf("Something's wrong, didn't find a king");
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
		printf("Something's gone wrong");
	}
	check=0;
	if(player[0]=='U'){
		/*checking horizonally*/
		for(i=king[0]; i<8; i++){
			if((square[king[1]][i]!=' ')&&(square[king[1]][i]!='K')){
				if((square[king[1]][i]=='q')||(square[king[1]][i]=='r')){
					check=1;
					return check;
				}else{break;}
			}
		}
		for(i=king[0]; i>-1; i--){
			if((square[king[1]][i]!=' ')&&(square[king[1]][i]!='K')){
				if((square[king[1]][i]=='q')||(square[king[1]][i]=='r')){
					check=1;
					return check;
				}else{break;}
			}
		}
		/*checking vertically*/
		for(i=king[1]; i<8; i++){
			if((square[i][king[0]]!=' ')&&(square[i][king[0]]!='K')){
				if((square[i][king[0]]=='q')||(square[i][king[0]]=='r')){
					check=1;
					return check;
				}else{break;}
			}
		}
		for(i=king[1]; i>-1; i--){
			if((square[i][king[0]]!=' ')&&(square[i][king[0]]!='K')){
				if((square[i][king[0]]=='q')||(square[i][king[0]]=='r')){
					check=1;
					return check;
				}else{break;}
			}
		}
		/*checking diagonally*/
		for(i=king[1], j=king[0]; i<8 && j<8; i++, j++){
			if((square[i][j]!=' ')&&(square[i][j]!='K')){
				if((square[i][j]=='q')||(square[i][j]=='b')){
					check=1;
					return check;
				}else{break;}
			}
		}
		for(i=king[1], j=king[0]; i>-1 && j>-1; i--, j--){
			if((square[i][j]!=' ')&&(square[i][j]!='K')){
				if((square[i][j]=='q')||(square[i][j]=='b')){
					check=1;
					return check;
				}else{break;}
			}
		}
		/*checking other diagonal*/
		for(i=king[1], j=king[0]; i>-1 && j<8; i--, j++){
			if((square[i][j]!=' ')&&(square[i][j]!='K')){
				if((square[i][j]=='q')||(square[i][j]=='b')){
					check=1;
					return check;
				}else{break;}
			}
		}
		for(i=king[1], j=king[0]; i<8 && j>-1; i++, j--){
			if((square[i][j]!=' ')&&(square[i][j]!='K')){
				if((square[i][j]=='q')||(square[i][j]=='b')){
					check=1;
					return check;
				}else{break;}
			}
		}
		/*checking for knights*/
		if(king[1]+2<8){
			if(king[0]+1<8){
				if(square[king[1]+2][king[0]+1]=='n'){
					check=1;
					return check;
				}
			}
			if(king[0]-1>-1){
				if(square[king[1]+2][king[0]-1]=='n'){
					check=1;
					return check;
				}
			}
		}
		if(king[1]-2>-1){
			if(king[0]+1<8){
				if(square[king[1]-2][king[0]+1]=='n'){
					check=1;
					return check;
				}
			}
			if(king[0]-1>-1){
				if(square[king[1]-2][king[0]-1]=='n'){
					check=1;
					return check;
				}
			}
		}
		if(king[1]+1<8){
			if(king[0]+2<8){
				if(square[king[1]+1][king[0]+2]=='n'){
					check=1;
					return check;
				}
			}
			if(king[0]-2>-1){
				if(square[king[1]+1][king[0]-2]=='n'){
					check=1;
					return check;
				}
			}
		}
		if(king[1]-1>-1){
			if(king[0]+2<8){
				if(square[king[1]-1][king[0]+2]=='n'){
					check=1;
					return check;
				}
			}
			if(king[0]-2>-1){
				if(square[king[1]-1][king[0]-2]=='n'){
					check=1;
					return check;
				}
			}
		}
		/*checking for pawns*/
		if(king[1]+1<8){
			if(king[0]+1<8){
				if(square[king[1]+1][king[0]+1]=='p'){
					check=1;
					return check;
				}
			}
			if(king[0]-1>-1){
				if(square[king[1]+1][king[0]-1]=='p'){
					check=1;
					return check;
				}
			}
		}
	}else if(player[0]=='L'){
		/*checking horizonally*/
		for(i=king[0]; i<8; i++){
			if((square[king[1]][i]!=' ')&&(square[king[1]][i]!='k')){
				if((square[king[1]][i]=='Q')||(square[king[1]][i]=='R')){
					check=1;
					return check;
				}else{break;}
			}
		}
		for(i=king[0]; i>-1; i--){
			if((square[king[1]][i]!=' ')&&(square[king[1]][i]!='k')){
				if((square[king[1]][i]=='Q')||(square[king[1]][i]=='R')){
					check=1;
					return check;
				}else{break;}
			}
		}
		/*checking vertically*/
		for(i=king[1]; i<8; i++){
			if((square[i][king[0]]!=' ')&&(square[i][king[0]]!='k')){
				if((square[i][king[0]]=='Q')||(square[i][king[0]]=='R')){
					check=1;
					return check;
				}else{break;}
			}
		}
		for(i=king[1]; i>-1; i--){
			if((square[i][king[0]]!=' ')&&(square[i][king[0]]!='k')){
				if((square[i][king[0]]=='Q')||(square[i][king[0]]=='R')){
					check=1;
					return check;
				}else{break;}
			}
		}
		/*checking diagonally*/
		for(i=king[1], j=king[0]; i<8 && j<8; i++, j++){
			if((square[i][j]!=' ')&&(square[i][j]!='k')){
				if((square[i][j]=='Q')||(square[i][j]=='B')){
					check=1;
					return check;
				}else{break;}
			}
		}
		for(i=king[1], j=king[0]; i>-1 && j>-1; i--, j--){
			if((square[i][j]!=' ')&&(square[i][j]!='k')){
				if((square[i][j]=='Q')||(square[i][j]=='B')){
					check=1;
					return check;
				}else{break;}
			}
		}
		/*checking other diagonal*/
		for(i=king[1], j=king[0]; i>-1 && j<8; i--, j++){
			if((square[i][j]!=' ')&&(square[i][j]!='k')){
				if((square[i][j]=='Q')||(square[i][j]=='B')){
					check=1;
					return check;
				}else{break;}
			}
		}
		for(i=king[1], j=king[0]; i<8 && j>-1; i++, j--){
			if((square[i][j]!=' ')&&(square[i][j]!='k')){
				if((square[i][j]=='Q')||(square[i][j]=='B')){
					check=1;
					return check;
				}else{break;}
			}
		}
		/*checking for knights*/
		if(king[1]+2<8){
			if(king[0]+1<8){
				if(square[king[1]+2][king[0]+1]=='N'){
					check=1;
					return check;
				}
			}
			if(king[0]-1>-1){
				if(square[king[1]+2][king[0]-1]=='N'){
					check=1;
					return check;
				}
			}
		}
		if(king[1]-2>-1){
			if(king[0]+1<8){
				if(square[king[1]-2][king[0]+1]=='N'){
					check=1;
					return check;
				}
			}
			if(king[0]-1>-1){
				if(square[king[1]-2][king[0]-1]=='N'){
					check=1;
					return check;
				}
			}
		}
		if(king[1]+1<8){
			if(king[0]+2<8){
				if(square[king[1]+1][king[0]+2]=='N'){
					check=1;
					return check;
				}
			}
			if(king[0]-2>-1){
				if(square[king[1]+1][king[0]-2]=='N'){
					check=1;
					return check;
				}
			}
		}
		if(king[1]-1>-1){
			if(king[0]+2<8){
				if(square[king[1]-1][king[0]+2]=='N'){
					check=1;
					return check;
				}
			}
			if(king[0]-2>-1){
				if(square[king[1]-1][king[0]-2]=='N'){
					check=1;
					return check;
				}
			}
		}
		/*checking for pawns*/
		if(king[1]-1<8){
			if(king[0]+1<8){
				if(square[king[1]-1][king[0]+1]=='P'){
					check=1;
					return check;
				}
			}
			if(king[0]-1>-1){
				if(square[king[1]-1][king[0]-1]=='P'){
					check=1;
					return check;
				}
			}
		}
	}
	return check;
}

/*Finds the king and places its coordinates into the input array king*/
int find_king(char square[8][8], char *player, int king[2]){
	int i, j;
	if(player[0]=='U'){
		for(i=0; i<8; i++){
			for(j=0; j<8; j++){
				if(square[i][j]=='K'){
					king[0]=j;
					king[1]=i;
					return 0;
				}
			}
		}
		return 1;
	}else if(player[0]=='L'){
		for(i=7; i>-1; i--){
			for(j=7; j>-1; j--){
				if(square[i][j]=='k'){
					king[0]=j;
					king[1]=i;
					return 0;
				}
			}
		}
		return 1;
	}
	return 1;
}

/*Calculates whether player is in check mate
has been modified slightly to calculate stalemate as well*/
int check_mate(char square[8][8], char *player){
	int x_coord, y_coord, new_x, new_y;
	char taken_piece, c;
	for(x_coord=0; x_coord<8; x_coord++){
		for(y_coord=0; y_coord<8; y_coord++){
			if((player[0]=='U' && isupper(square[y_coord][x_coord])) 
				|| (player[0]=='L' && islower(square[y_coord][x_coord]))){
				for(new_x=0; new_x<8; new_x++){
					for(new_y=0; new_y<8; new_y++){
						if(is_valid_cm(square, x_coord, y_coord, new_x, new_y, player)){
							taken_piece=square[new_y][new_x];
							change_board(square, x_coord, y_coord, new_x, new_y, player);
							if(!check(square, player)){
								change_board(square, new_x, new_y, x_coord, y_coord, player);
								square[new_y][new_x]=taken_piece;
								if(check(square, player)){
									printf("One possible move is moving the '%c' from (%d,%d) to (%d,%d)\n",
									square[y_coord][x_coord], x_coord+1, y_coord+1, new_x+1, new_y+1);
								}
								return 0;
							}
							change_board(square, new_x, new_y, x_coord, y_coord, player);
							square[new_y][new_x]=taken_piece;
						}
					}
				}
			}
		}
	}
	if(check(square, player)){
		return 1;
	}else{
		printf("\nThe board is in stalemate.\n%s cannot move.\n\nIt's a draw!\n", player);
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
		printf("Something's gone wrong");
		exit(EXIT_FAILURE);
	}
}

/*Slightly modified is_valid function which doesn't call the play
function. This function is for use in the check_mate function*/
int is_valid_cm(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player){
	if(x_coord==new_x && y_coord==new_y){
			return 0;
	}
	if(player[0]=='U'){
		if(isupper(square[new_y][new_x])){
			return 0;
		}
	}else if(player[0]=='L'){
		if(islower(square[new_y][new_x])){
			return 0;
		}
	}
	if(toupper(square[y_coord][x_coord])=='R'){
		if( !(
			(x_coord == new_x && y_coord != new_y)
			||
			(y_coord == new_y && x_coord != new_x)
		)){
			return 0;
		}
	}else if(toupper(square[y_coord][x_coord])=='Q'){
		if( !(
			(x_coord == new_x && y_coord != new_y)
			||
			(y_coord == new_y && x_coord != new_x)
			||
			(abs(x_coord-new_x) == abs(y_coord-new_y))
		)){
			return 0;
		}
	}else if(toupper(square[y_coord][x_coord])=='K'){
		if(!((abs(x_coord-new_x)<=1) && (abs(y_coord-new_y)<=1))){
			return 0;
		}
	}else if(toupper(square[y_coord][x_coord])=='B'){
		if( !((abs(x_coord-new_x)) == (abs(y_coord-new_y)))){
			return 0;
		}
	}else if(square[y_coord][x_coord]=='P'){
		if( !(
			(
					(new_x==x_coord)
				&&
					(square[new_y][new_x]==' ')
				&&
					(	
						(new_y == y_coord+1)
					||
						(
						(new_y == y_coord+2)
						&&
						(y_coord==1)
						)
					)
			)
			||
			(
					(abs(new_x-x_coord) == 1)
				&&
					(new_y == y_coord+1)
				&&
					(square[new_y][new_x]!=' ')
			)
		)){
			return 0;
		}
	}else if(square[y_coord][x_coord]=='p'){
		if( !(
			(
					(new_x==x_coord)
				&&
					(square[new_y][new_x]==' ')
				&&
					(	
						(new_y == y_coord-1)
					||
						(
						(new_y == y_coord-2)
						&&
						(y_coord==6)
						)
					)
			)
			||
			(
					(abs(new_x-x_coord) == 1)
				&&
					(new_y == y_coord-1)
			)
		)){
			return 0;
		}
	}else if(toupper(square[y_coord][x_coord])=='N'){
		if( !(
			((abs(x_coord-new_x)==2) && (abs(y_coord-new_y)==1))
			||
			((abs(x_coord-new_x)==1) && (abs(y_coord-new_y)==2))
		)){
			return 0;
		}
	}
	if(anything_inbetween_cm(square, x_coord, y_coord, new_x, new_y, player)){
		return 0;
	}
	return 1;
}

/*Slightly modified anything_inbetween function which doesn't call the play
function. This function is for use in the is_valid_cm function*/
int anything_inbetween_cm(char square[8][8], int x_coord, int y_coord, int new_x, int new_y, char *player){
	int i, j;
	int min_x, max_x, min_y, max_y;
	if(x_coord>=new_x){
		min_x=new_x;
		max_x=x_coord;
	}else{
		min_x=x_coord;
		max_x=new_x;
	}
	if(y_coord>=new_y){
		min_y=new_y;
		max_y=y_coord;
	}else{
		min_y=y_coord;
		max_y=new_y;
	}
	if(x_coord==new_x){
		for(i=min_y+1; i<max_y; i++){
			if(square[i][x_coord]!=' '){
				return 1;
			}
		}
	}else if(y_coord==new_y){
		for(i=min_x+1; i<max_x; i++){
			if(square[y_coord][i]!=' '){
				return 1;
			}
		}
	}
	else if((abs(x_coord-new_x)) == (abs(y_coord-new_y))){
		if((x_coord>new_x && y_coord>new_y) || (x_coord<new_x && y_coord<new_y)){
			for(i=min_y+1, j=min_x+1; i<max_y && j<max_x; i++, j++){
				if(square[i][j]!=' '){
					return 1;
				}
			}
		}
	}else if((abs(x_coord-new_x)) == (abs(y_coord-new_y))){
		if((x_coord<new_x && y_coord>new_y) || (x_coord>new_x && y_coord<new_y)){
			for(i=max_y-1, j=min_x+1; i>min_y && j<max_x; i--, j++){
				if(square[i][j]!=' '){
					return 1;
				}
			}
		}
	}
	return 0;
}

/*w_o_o, or wipe_out_opponent, my little easter egg*/
void w_o_o(char square[8][8], char *player){
	int x_coord, y_coord;
	printf("\nYou found my easter egg!\n");
	for(x_coord=0; x_coord<8; x_coord++){
		for(y_coord=0; y_coord<8; y_coord++){
			if((player[0]=='U' && islower(square[y_coord][x_coord]) && square[y_coord][x_coord]!='k') 
			|| (player[0]=='L' && isupper(square[y_coord][x_coord]) && square[y_coord][x_coord]!='K')){
				square[y_coord][x_coord]=' ';
			}
		}
	}
}

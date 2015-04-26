#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

void get_guess(int guess[4]);
void check_guess(int sequence[4], int guess[4], int* won);

int main(int argc, char *argv[]){
	int sequence[4], guess[4];
	int i, won=0;
	char c;
	srand(time(NULL));
	for(i=0; i<4; i++){
		sequence[i]=rand()%6+1;
	}
	printf("The computer has generated a set of four numbers, each ranging from 1-6, pick the correct sequence to win\n");
	/*for(i=0; i<4; i++){
		printf("%d ", sequence[i]);
	}
	printf("\n");*/
	while(!won){
		get_guess(guess);
		check_guess(sequence, guess, &won);
	}
	printf("Correct! Your guess was: ");
	for(i=0; i<4; i++){
		printf("%d ", guess[i]);
	}
	printf("\nAnd the correct answer was: ");
	for(i=0; i<4; i++){
		printf("%d ", sequence[i]);
	}
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
		return 0;
	}
	return 0;
}


void get_guess(int guess[4]){
	int i;
	/*for(i=0; i<4; i++){
		guess[i]=i;
	}*/
	printf("Choose a sequence of 4 numbers from 1-6: ");
	if(scanf("%d%d%d%d", &guess[0], &guess[1], &guess[2], &guess[3])!=4){
		char skip_ch;
		do{
			scanf("%c",&skip_ch);
		}while(skip_ch != '\n');
		printf("Invalid input, try again:\n");
		get_guess(guess);
		return;
	}
	for(i=0; i<4; i++){
		if(guess[i] < 1 || guess[i] > 6){
			printf("Invalid play, try again:\n");
			get_guess(guess);
			return;
		}
	}
}

void check_guess(int sequence[4], int guess[4], int* won){
	int i, j, count=0;
	int sequence_copy[4], guess_copy[4];
	char* correct[4]={"Miss","Miss","Miss","Miss"};
	for(i=0; i<4; i++){
		sequence_copy[i]=sequence[i];
		guess_copy[i]=guess[i];
	}
	for(i=0; i<4; i++){
		if(guess_copy[i]==sequence_copy[i] && guess_copy[i]!=0){
			correct[count]="Hit";
			count++;
			//printf("%d %d\n", guess_copy[i], sequence_copy[i]);
			guess_copy[i]=0;
			sequence_copy[i]=0;
		}
	}
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if(guess_copy[i]==sequence_copy[j] && guess_copy[i]!=0){
				correct[count]="Partial Hit";
				count++;
				//printf("%d %d\n", guess_copy[i], sequence_copy[j]);
				guess_copy[i]=0;
				sequence_copy[j]=0;
			}
		}
	}
	for(i=0; i<3; i++){
		printf("%s, ", correct[i]);
	}
	printf("%s\n", correct[3]);
	if(!strcmp(correct[3], "Hit")){
		*won=1;
		return;
	}
}

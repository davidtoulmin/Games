#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define arbitrary_word_length 1000

int in_word(char word[], int length, char character,
	int *found, char blank_word[]);
void print_man(int failures);


int
main(int argc, char *argv[]){
	int tries=0, length=0, found=0, instances, failures=0, i;
	/*char character;*/
	char word[arbitrary_word_length], blank_word[arbitrary_word_length],
		whole_word[arbitrary_word_length], guess[arbitrary_word_length];
	char c;
	char skip_ch;
	printf("Input a word to play:");
	/*scanf("%s", word);
	while(word[length]!='\0'){
		length++;
	}*/
	while((c=getchar())!='\n'){
		if(!isalpha(c)){
			do{
				skip_ch=fgetc(stdin);
			}while(skip_ch != '\n');
			break;
		}
		word[length]=c;
		blank_word[length]='_';
		whole_word[length]=c;
		length++;
		if(length>=arbitrary_word_length){
			printf("Word is too long, watcha doing making a word over 100 characters long?");
			exit(EXIT_FAILURE);
		}
	}
	if(length<1){
		printf("Input is not a word!");
		exit(EXIT_FAILURE);
	}
	word[length]='\0';
	blank_word[length]='\0';
	whole_word[length]='\0';
	printf("You chose %s, with length %d",word, length);
	/*for(i=0; i<100; i++){
		printf("\n");
	}*/
	printf("\n");
	while(found<length){
		tries++;
		printf("Input a character or whole word guess: ");
		/*if(scanf("%c", &character)!=1){
			printf("Invalid input");
			exit(EXIT_FAILURE);
		}
		do{
			skip_ch=fgetc(stdin);
		}while(skip_ch != '\n');*/
		i=0;
		guess[0]='\0';
		while((c=getchar())!='\n'){
			if(!isalpha(c)){
				do{
					skip_ch=fgetc(stdin);
				}while(skip_ch != '\n');
				break;
			}
			guess[i]=c;
			i++;
		}
		if(guess[0]=='\0') {
			continue;
		}
		guess[i]='\0';
		if(i>1) {
			if(!strcmp(guess, whole_word)) {
				printf("You correctly guessed the word!");
				exit(EXIT_SUCCESS);
			} else {
				failures++;
				printf("'%s' is not the word.\n", guess);
				print_man(failures);
			}
		} else {
			/*if(in_word(blank_word, length, guess[0], &found,blank_word) {
					printf("You've already found that letter");
					continue;
			}*/
			instances=in_word(word, length, guess[0], &found, blank_word);
			if(instances){
				printf("'%c' is in the word: %s\n", guess[0], blank_word);
			} else {
				failures++;
				printf("'%c' is not in the word.\n", guess[0]);
				print_man(failures);
			}
		}
	}
	printf("You got the word!");
	return 0;
}

int in_word(char word[], int length, char character,
	int *found, char blank_word[]){
	int i, instances=0;
	for(i=0; i<length; i++){
		if(word[i]==character){
			instances++;
			blank_word[i]=word[i];
			word[i]='_';
			(*found)++;
		}
	}
	return instances;
}

void
print_man(int failures){
	if(failures==0){
		return;
	}else if(failures==1){
		printf("\n\n\n\n\n\n_________\n");
		return;
	}else if(failures==2){
		printf("        \n       |\n       |\n       |\n");
		printf("       |\n       |\n_______|_\n");
		return;
	}else if(failures==3){
		printf("  ______\n  |    |\n       |\n       |\n");
		printf("       |\n       |\n_______|_\n");
		return;
	}else if(failures==4){
		printf("  ______\n  |    |\n  o    |\n       |\n");
		printf("       |\n       |\n_______|_\n");
		return;
	}else if(failures==5){
		printf("  ______\n  |    |\n  o    |\n  |    |\n");
		printf("  |    |\n       |\n_______|_\n");
		return;
	}else if(failures==6){
		printf("  ______\n  |    |\n  o    |\n  |\\   |\n");
		printf("  |    |\n       |\n_______|_\n");
		return;
	}else if(failures==7){
		printf("  ______\n  |    |\n  o    |\n /|\\   |\n");
		printf("  |    |\n       |\n_______|_\n");
		return;
	}else if(failures==8){
		printf("  ______\n  |    |\n  o    |\n /|\\   |\n");
		printf("  |    |\n   \\   |\n_______|_\n");
		return;
	}else if(failures==9){
		printf("  ______\n  |    |\n  o    |\n /|\\   |\n");
		printf("  |    |\n / \\   |\n_______|_");
		printf("\nHe's dead!\n");
		exit(EXIT_SUCCESS);
	}
}
	
	
	/*
	  ______
	  |    |
	  o    |
	 /|\   |
	  |    |
	 / \   |
	_______|_
	*/

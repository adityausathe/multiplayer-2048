/*****************************************************************************
 * Copyright (C) Aditya Sathe satheaditya2014@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include<unistd.h> 
#include<termios.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>

int d, a[5][5], restore, SCORE;



void clear_scrn() {
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t");
}

void init(void) {
	int i, j;
	for(i = 0;i < d;i++) {
		for(j = 0;j < d;j++)
			a[i][j] = 0;
	}
	restore = 0;
	SCORE = 0;
}

void QUIT(void) {
	int terminate;
	char choice;
	struct termios old, t;
	clear_scrn();
	printf("Are You Surely want to QUIT? Your progress will be lost.\n\n\t1. YES\n\t2. Nope\n");
	clear_scrn();
	tcgetattr(STDIN_FILENO, &old);
        t = old;
       	cfmakeraw(&t);
       	tcsetattr(STDIN_FILENO, TCSANOW, &t);
       	choice = getc(stdin);
       	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	if(terminate) {
		init();
		main();
	}
}
void name_2048(void) {
	printf("\n\t\t\t||||||||||||\t||||||||||||\t|||      |||\t||||||||||||\n");
          printf("\t\t\t         |||\t|||      |||\t|||      |||\t|||      |||\n");
	  printf("\t\t\t         |||\t|||      |||\t|||      |||\t|||      |||\n");
	  printf("\t\t\t||||||||||||\t|||      |||\t|||      |||\t||||||||||||\n");
	  printf("\t\t\t|||         \t|||      |||\t||||||||||||\t|||      |||\n");
	  printf("\t\t\t|||         \t|||      |||\t         |||\t|||      |||\n");
	  printf("\t\t\t||||||||||||\t||||||||||||\t         |||\t||||||||||||\n\n\n\n");
}
void SAVE(char *player) {
	char score[8], *file;
	int f1, i, j, k, len;
	FILE *fp;
	f1 = open(player, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	write(f1, &SCORE, 4);
	write(f1, &d, 4);
	for(i = 0;i < d;i++) {
		for(j = 0;j < d;j++)
			write(f1, (a[i] + j), 4);
	}
	close(f1);
	switch(d) {
		case 3: {
			file = "High_scores3";
			break;
		}
		case 4: {
			file = "High_scores4";
			break;
		}
		case 5: {
			file = "High_scores5";
			break;
		}
		default:
			break;
	}
	fp = fopen(file, "a");
	strcat(player, "->>>>");		
	sprintf(score, "%d", SCORE);
	strcat(player, score);
	strcat(player, " ");
	fputs(player, fp);
	fclose(fp);
	clear_scrn();
	init();
	main();
}




/*
 * prints the checkboard after each move.
 */
void print() {
	int i, j, k, b, flag = 0;
	char *box;
	clear_scrn();
	printf("\t\t\tSCORE = %d\t\tPRESS 'X' to SAVE AND EXIT\t\tPRESS 'Q' to QUIT\n----------------------------------------------------------\n", SCORE);
	b = d;
        while(b) {
                printf("_______");
                b--;
  	}
	printf("\n");
	for(i = 0;i < d;i++) {
                for(j = 0;j < d;j++) {
			box = (char *) malloc(7);
			strcat(box, " ");
			if(*(a[i] + j) != 0)
				sprintf(box, "%d", *(a[i] + j));
			flag = 0;
			for(k = 0;k < 7;k++) {
				if(box[k] == '\0') {
					box[k] = ' ';
					flag = 1;
				}
				if(flag)
					box[k] = ' ';
			}
			box[6] = '\0';
                        printf("|%s", box);
			free(box);
		}
		printf("|\n");
		b = d;
		while(b) {
			printf("|______");
			b--;
		}
		printf("|\n");
        }
        printf("\n----------------------------------------------------------\n");
	clear_scrn();
}


//Handles the condition of game over.

void END(char *player) {
	char call_main;
	int i, j, end = 1;
	struct termios old, t;
	for(i = 0;i < d;i++) {
		for(j = 0;j < d;j++) {
			if(a[i][j] == a[i][j + 1])
				end = 0;
			if(a[i][j] == a[i + 1][j])
				end = 0;
		}
	}
	if(end) {
		clear_scrn();
		printf("\n\t\t\tGAME OVER....try again\n\n\t\t\tPress any key to continue\n");
		clear_scrn();
		tcgetattr(STDIN_FILENO, &old);
        	t = old;
        	cfmakeraw(&t);
        	tcsetattr(STDIN_FILENO, TCSANOW, &t);
        	call_main = getc(stdin);
        	tcsetattr(STDIN_FILENO, TCSANOW, &old);
		if(call_main)
			SAVE(player);
	}
}

//Generates random 2 or 4 for a fresh move.

void Random(char *player) {
	int i, j, iter = 0;
	static int c,first = 0, count = 0;
	if(!restore)
		c = strlen(player) + 4;
		
	while(1) {
		if(iter == (d * d)){
			printf("Game Over\n");
			break;
		}
		if(c > (d * d))
                        c -= (d * d);
		i = c / d;
		j = c % d;
		if(a[i][j] == 0) {
			count++;
			if(count == 8) {
				a[i][j] = 4;
				count = 0;
			}
			else
				a[i][j] = 2;
			c += (d - 1);
			break;
		}
		c += (d - 1);
		iter++;
	}
		
	if(first)
        	print();
	first = 1;
}

//handles the consecquences of the user after pressing down key
 
void DOWN(int *flag, int *flag2) {
        int curr, j = (d-1), i, k;
	*flag = 0;
	*flag2 = 1;
        while(j >= 0) {
                i = (d - 1);
                curr = 0;
                while(i >= 0) {
                        if(*(a[i] + j) == 0) {
				curr++;
				*flag2 = 0;
			}
                        else if(curr != 0) {
                                *(a[i + curr] + j) = *(a[i] + j);
                                *(a[i] + j) = 0;
				*flag = 1;
                        }
                        i--;
                }i = (d - 1);
                while(i > 0) {
                        if((*(a[i] + j) == *(a[i - 1] + j)) && (*(a[i] + j) != 0)) {
                                *(a[i] + j) = 2 * *(a[i] + j);
				SCORE += *(a[i] + j);
                                *(a[i - 1] + j) = 0;
                                        k = (d - 1);
					*flag = 1;
					*flag2 = 0;
                                        while(k != 0) {
                                                if(*(a[k] + j) == 0) {
                                                        *(a[k] + j) = *(a[k -1] + j);
                                                        *(a[k - 1] + j) = 0;
                                                }
                                                k--;
                                        }
                        }
                	i--;
                }
                j--;
        }
}

//Event handling after user presses up key

void UP(int *flag, int *flag2) {
        int curr, j = 0, i, k;
	*flag = 0;
	*flag2 = 1;
        while(j < d) {
                i = 0;
                curr = 0;
                while(i != d) {
                        if(*(a[i] + j) == 0) {
				*flag2 = 0;
				curr++;
			}
                        else if(curr != 0) {
                                *(a[i - curr] + j) = *(a[i] + j);
                                *(a[i] + j) = 0;
				*flag = 1;
                        }
                        i++;
                }i = 0;
                while(i < (d -1)) {
                        if((*(a[i] + j) == *(a[i + 1] + j)) && (*(a[i] + j) != 0)) {
                                *(a[i] + j) = 2 * *(a[i] + j);
				SCORE += *(a[i] + j);
                                *(a[i + 1] + j) = 0;
                                        k = 0;
                                   	*flag = 1;
					*flag2 = 0;
                                        while(k != (d - 1)) {
                                                if(*(a[k] + j) == 0) {
                                                        *(a[k] + j) = *(a[k + 1] + j);
                                                        *(a[k + 1] + j) = 0;
                                                }
                                                k++;
                                        }
                        }
                	i++;
                }
                j++;
        }
}

//handles events when user press left key
			
void LEFT(int *flag, int *flag2) {
	int curr, i = 0, j, k;
	*flag = 0;
	*flag2 = 1;
	while(i < d) {
		j = 0;
		curr = 0;
		while(j != d) {
			if(*(a[i] + j) == 0) {
				curr++;
				*flag2 = 0;
			}
			else if(curr != 0) {
				*(a[i] + j - curr) = *(a[i] + j);
				*(a[i] + j) = 0;
				*flag = 1;
			}
			j++;
		}j = 0;
		while(j < (d - 1)) {
			if((*(a[i] + j) == *(a[i] + j + 1)) && (*(a[i] + j) != 0)) {
				*(a[i] + j) = 2 * *(a[i] + j);
				SCORE += *(a[i] + j);
				*(a[i] + j + 1) = 0;
					k = 0;
	              			*flag = 1;
					*flag2 = 0;
      		       			while(k != (d - 1)) {
             	   	       	 		if(*(a[i] + k) == 0) {
                        	        		*(a[i] + k) = *(a[i] + k + 1);
							*(a[i] + k + 1) = 0;
                        			}
                 			        k++;
                			}		
			}
			j++;
		}
		i++;
	}
	
}

//handles events when user press left key

void RIGHT(int *flag, int *flag2) {
        int curr, i = (d - 1), j, k;
	*flag = 0;
	*flag2 = 1;
        while(i >= 0) {
                j = (d -1);
                curr = 0;
                while(j >= 0) {
                        if(*(a[i] + j) == 0) {
				curr++;
				*flag2 = 0;
			}
                        else if(curr != 0) {
                                *(a[i] + j + curr) = *(a[i] + j);
                                *(a[i] + j) = 0;
 				*flag = 1;
                        }
                        j--;
                }j = (d - 1);
                while(j > 0) {
                        if((*(a[i] + j) == *(a[i] + j - 1)) && (*(a[i] + j) != 0)) {
                                *(a[i] + j) = 2 * *(a[i] + j);
				SCORE += *(a[i] + j);
                                *(a[i] + j - 1) = 0;
                                        k = (d - 1);
					*flag = 1;
					*flag2 = 0;
                                        while(k != 0) {               
                                                if(*(a[i] + k) == 0) {
                                                        *(a[i] + k) = *(a[i] + k - 1);
                                                        *(a[i] + k - 1) = 0;
                                                }
                                                k--;
                                        }
                        }
                	j--;
                }
                i--;
        }
}

//check the user input
//It uses '2','4','6','8' for down, left,right,up respectively.

void check(char *arrow, char *player) {
	int move_check, end_check, choice_move = 0;
	switch(*arrow) {
		case '4':{
			LEFT(&move_check, &end_check);
			choice_move = 1;
			break;
		}
		case '6':{
			RIGHT(&move_check, &end_check);
			choice_move = 1;
			break;
		}
		case '8':{
                        UP(&move_check, &end_check);
			choice_move = 1;
	                break;
                }
		case '2':{
                        DOWN(&move_check, &end_check);
			choice_move = 1;
                        break;
                }
		case 'x':{
                        SAVE(player);
                        break;
                }
		case 'q':{
                        QUIT();
			print();
                        break;
                }
		default:
			break;
	}
	if(choice_move) {
		if(move_check)
        	 	Random(player);
   	    	if(end_check)
   		     	END(player);
	}	
}

//main method to control the user response once the game started

void head(char *player) {
	int i, j, k;
	char c;
	struct termios old, t;
	if(!restore) {
		Random(player);
		restore = 1;
		Random(player);
	}
	if(restore)
		print();
	while(1) {
     	   	tcgetattr(STDIN_FILENO, &old);
        	t = old;
        	cfmakeraw(&t);
        	tcsetattr(STDIN_FILENO, TCSANOW, &t);
        	c = getc(stdin);
        	tcsetattr(STDIN_FILENO, TCSANOW, &old);
        	check(&c, player);
	}
}

//Saves record for each game and new player

void record(char *player) {
	int f, k, i;
	char choice;
	struct termios old, t;
	f = open(player, O_RDONLY);
	if(f == -1) {
		clear_scrn();
                printf("Your Game wasn't Saved. Press any key to continue\n");
		clear_scrn();
		
		tcgetattr(STDIN_FILENO, &old);
        	t = old;
        	cfmakeraw(&t);
        	tcsetattr(STDIN_FILENO, TCSANOW, &t);
        	choice = getc(stdin);
        	tcsetattr(STDIN_FILENO, TCSANOW, &old);
		if(choice)
			main();
	}else {
		i = 0;
		read(f, &SCORE, 4);
		read(f, &d, 4);
      	  	while(i < d) {
        		for(k = 0;k < d;k++) {
				if((k+1) == d) {
					read(f, (a[i] + k), 4);
			 		i++;
				}
				else
					read(f, (a[i] + k), 4);
			}
		}
		restore = 1;
		head(player);
	}
}

//checks for resume previous game

void old_player(void) {
	FILE *fp;
	int i = 0, j = 0, flag = 1, count = 0;
	char c, pl_name[32][16], choice;
	if ((fp = fopen("player_names","r"))==NULL){
		clear_scrn();
       		printf("No Previous Record. Press any key for MENU\n");
		scanf("%c", &choice);
		if(choice)
			main();         
		}
	clear_scrn();
	printf("\n\t\t\tWho are you\t 0: MAIN MENU\n-------------------------------------------\n");
	while((c = getc(fp)) !=	EOF) {
		count++;
		if(c == ' ') {
			pl_name[i][j] = '\0';
			i++;
			flag = 1;
			j = 0;
		}
		else {
			if(flag) {
				printf("\n%d.", (i + 1));
				flag = 0;
			}
			pl_name[i][j] = c;
			printf("%c", c);
			j++;
		}
	}
	
	if(!count){
		printf("\n\t\tNo Previous Record\n\t\tPress any key for MENU");
		clear_scrn();
		scanf("%c", &choice);
		if(choice)
			main();
	} else {
		clear_scrn();
		scanf("%d", &i);
		if(d)
			record(pl_name[i-1]);
		else
			main();
	}
     	fclose(fp);
}
void Reinstall(void) {
	int choice;
	struct termios old, t;
	FILE *f;
	clear_scrn();
	printf("\t\t\tCAUTION: This will erase all the pevious record of this Game\n\n\t\t\t1.Yes\n\t\t\t2.Nope\n");
	clear_scrn();
	tcgetattr(STDIN_FILENO, &old);
       	t = old;
       	cfmakeraw(&t);
       	tcsetattr(STDIN_FILENO, TCSANOW, &t);
       	choice = getc(stdin);
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
	if(choice) {
		f = fopen ("player_names", "w");
		fclose(f);
	}
}


//Prints High Score in kall game format

void High_score(char catagory) {
	FILE *fp;
	struct termios old, t;
	int i = 0, j = 0, flag = 1, count = 0, ch;
	char c, pl_name[32][16], choice, *file;
	switch(catagory) {
		case '1': {
			file = "High_scores3";
			break;
		}
		case '2': {
			file = "High_scores4";
			break;
		}
		case '3': {
			file = "High_scores5";
			break;
		}
		default:{
			main();
			break;
		}
	}
	if ((fp = fopen(file, "r")) == NULL){
		clear_scrn();
       		printf("No Previous Record. Press any key for MENU\n");
		tcgetattr(STDIN_FILENO, &old);
        	t = old;
        	cfmakeraw(&t);
        	tcsetattr(STDIN_FILENO, TCSANOW, &t);
        	choice = getc(stdin);
        	tcsetattr(STDIN_FILENO, TCSANOW, &old);
		if(choice)
			main();         
	}
	clear_scrn();
	printf("\n\t\t\tHALL OF FAME\tMAIN MENU: Press any key\n-------------------------------------------\n\n\t\t");
	while((c = getc(fp)) !=	EOF) {
		count++;
		if(c == ' ') {
			pl_name[i][j] = '\0';
			i++;
			flag = 1;
			j = 0;
		}
		else {
			if(flag) {
				printf("\n%d.", (i + 1));
				flag = 0;
			}
			pl_name[i][j] = c;
			printf("%c", c);
			j++;
		}
	}
	clear_scrn();
	fclose(fp);
	tcgetattr(STDIN_FILENO, &old);
       	t = old;
       	cfmakeraw(&t);
       	tcsetattr(STDIN_FILENO, TCSANOW, &t);
       	choice = getc(stdin);
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
	if(choice)
		main();
}



//MAIN method
int main() {
	FILE *fp1;
	char cata, *player, opt;
	int count;
	struct termios old, t;
	while(1) {
		clear_scrn();
		name_2048();
		printf("\t\t\t\tWELCOME\n\n\n\t\t\t\t1.New Game\n\n\t\t\t\t2.Continue\n\n\t\t\t\t3.High Scores\n\n\t\t\t\t4.Reinstall Game\n\n\t\t\t\t5.Exit");
		clear_scrn();
		tcgetattr(STDIN_FILENO, &old);
        	t = old;
        	cfmakeraw(&t);
        	tcsetattr(STDIN_FILENO, TCSANOW, &t);
        	opt = getc(stdin);
        	tcsetattr(STDIN_FILENO, TCSANOW, &old);
		switch(opt) {
			case '1':{
				clear_scrn();
				name_2048();
				player = (char *) malloc(16);
				printf("\t\tEnter your name\n\n\t");
				clear_scrn();
				scanf("%s", player);
				clear_scrn();
				fp1 = fopen ("player_names", "a");
				fputs(player, fp1);
				fputs(" ", fp1);
				fclose(fp1);
				clear_scrn();
				clear_scrn();
				name_2048();
				printf("\t\tEnter size of maze\n\n\t\t\t3--> 3 * 3\n\t\t\t4--> 4 * 4\n\t\t\t5--> 5 * 5\n");
				init();
				clear_scrn();
			        scanf("%d", &d);
				clear_scrn();
				head(player);
				break;
			}
			case '2':{
				old_player();
       	         	        break;
       		        }
			case '3':{
				clear_scrn();
				printf("\n\t\t\tChoose catagory\n--------------------------------------------------------\n\t\t\t1. 3x3\n\t\t\t2. 4x4\n\t\t\t3. 5x5\n\t\t\t");
				clear_scrn();
				tcgetattr(STDIN_FILENO, &old);
		        	t = old;
		        	cfmakeraw(&t);
		        	tcsetattr(STDIN_FILENO, TCSANOW, &t);
		        	cata = getc(stdin);
		        	tcsetattr(STDIN_FILENO, TCSANOW, &old);
				High_score(cata);
                	        break;
       		        }
			case '4':{
        	                Reinstall();
                	        break;
       		        }
			case '5':{
				system("clear");
                	        exit(1);
                	        break;
       		        }
			default:{
				clear_scrn();
				printf("Enter valid choice\n");
				break;
			}
		}
	}
	return 0;
}

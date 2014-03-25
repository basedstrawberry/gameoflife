//
//	Michael Wilson 2014
//	This is under the DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//	Or WTFPL
//	So do what the fuck you want with this code



#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include "main.h"


Cell *newCell(int x,int y) {
	Cell *c = malloc(sizeof(Cell));
	assert(c != NULL);
	c->alive = FALSE;
	c->x = x;
	c->y = y;
	c->age = 0;
	return c;
}

void generateGrid() {
	int i;
	int ii;
	for(i=0;i<=23;i++) {
		for(ii=0;ii<=79;ii++) {
			grid[i*80+ii] = newCell(ii,i);
			grid[i*80+ii]->alive = FALSE;
			if (rand_lim(100) > (100-birthrate)) grid[i*80+ii]->alive = TRUE;
		}
	}
	life_exists = TRUE;
}

void push(node_t **head,int val) {
	node_t *newnode = malloc(sizeof(node_t));
	assert(newnode != NULL);
	
	newnode->val = val;
	newnode->next = *head;
	
	*head = newnode;
	
}

int pop(node_t **head) {
	node_t *nextnode = NULL;
	
	if(head == NULL) return -1;
	
	nextnode = (*head)->next;
	free(*head);
	*head = nextnode;
	return 0;
}

int rand_lim(int limit) {
	int divisor = RAND_MAX / (limit+1);
	int retval;
	do {
		retval = rand() / divisor;
	} while (retval > limit);
	return retval;
}





int main(int argc, char *argv[]) {
	
	srand(time(NULL));
	
	
	
	
	
	BOOL playing = TRUE;
	FILE *log = fopen("/home/michael/toys/life/log.txt","w");
	if(log == NULL) {fprintf(stderr,"log not opened... :(\n");} else {fprintf(log,"log opened\n");}
	
	int i;
	char input;
	BOOL life_check;
	unsigned int usecs = 50000;
	node_t *toadd = NULL; node_t *todelete = NULL;
	node_t *current = NULL;
	
	
	printf("Input Birthrate: ");
	scanf("%d",&birthrate);
	printf("Automate?\n1 - yes|0 - no: ");
	scanf("%d",&automate);
	generateGrid();
	
	initscr();
	start_color();
	init_pair(1,COLOR_BLUE,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_BLACK);
	raw();
	noecho();
	curs_set(0);
	
	while(playing) {
		
		clear();
		////////////draw
		for(i=0;i<=GRIDSIZE;i++) {
			if(grid[i]->alive) {
				attron(COLOR_PAIR(1));
				mvaddch(grid[i]->y,grid[i]->x,64);
				attroff(COLOR_PAIR(1));
				refresh();
			}
		}
		refresh();
		
		///////////input
		if(!automate || !life_exists) {
			input = getch();
		
			switch(input) {
				case 103: // g
					generateGrid();
					break;
				case 113: // q
					playing = FALSE;
					break;
				default:
					//chill
					break;
			}
		}
		
		///////////update
		life_check = FALSE;
		for(i=0;i<=GRIDSIZE;i++) {
			if(grid[i]->alive) {
				life_check = TRUE;
				if(grid[i]->age > 99) push(&todelete,i);
				int nvalue = 0;
				int ii;
				int X = grid[i]->x;
				int Y = grid[i]->y;
				for(ii=0;ii<=GRIDSIZE;ii++) {
					if(grid[ii]->alive) {
						//n s e w 
						if(grid[ii]->x == X+1 && grid[ii]->y == Y)nvalue+=1;
						if(grid[ii]->x == X-1 && grid[ii]->y == Y)nvalue+=1;
						if(grid[ii]->x == X && grid[ii]->y == Y+1)nvalue+=1;
						if(grid[ii]->x == X && grid[ii]->y == Y-1)nvalue+=1;
						//diagonal
						if(grid[ii]->x == X+1 && grid[ii]->y == Y+1)nvalue+=1;
						if(grid[ii]->x == X-1 && grid[ii]->y == Y-1)nvalue+=1;
						if(grid[ii]->x == X-1 && grid[ii]->y == Y+1)nvalue+=1;
						if(grid[ii]->x == X+1 && grid[ii]->y == Y-1)nvalue+=1;
					}
				}
				switch(nvalue) {
					case 2:
						grid[i]->age += 1;
						break;
					case 3:
						grid[i]->age += 1;
						break;
					default:
						//grid[i]->alive=FALSE;
						push(&todelete,i);
						grid[i]->age = 0;
						break;
				}
			} else {
				int nvalue = 0;
				int ii;
				int X = grid[i]->x;
				int Y = grid[i]->y;
				for(ii=0;ii<=GRIDSIZE;ii++) {
					if(grid[ii]->alive) {
						if(grid[ii]->x == X+1 && grid[ii]->y == Y)nvalue+=1;
						if(grid[ii]->x == X-1 && grid[ii]->y == Y)nvalue+=1;
						if(grid[ii]->x == X && grid[ii]->y == Y+1)nvalue+=1;
						if(grid[ii]->x == X && grid[ii]->y == Y-1)nvalue+=1;
						//diagonal
						if(grid[ii]->x == X+1 && grid[ii]->y == Y+1)nvalue+=1;
						if(grid[ii]->x == X-1 && grid[ii]->y == Y-1)nvalue+=1;
						if(grid[ii]->x == X-1 && grid[ii]->y == Y+1)nvalue+=1;
						if(grid[ii]->x == X+1 && grid[ii]->y == Y-1)nvalue+=1;
					}
				}
				switch(nvalue) {
					case 3:
						push(&toadd,i);
						//grid[i]->alive=TRUE;
						break;
					default:
						push(&todelete,i);
						//grid[i]->alive=FALSE;
						break;
				}
			}
		}
		life_exists = life_check;
	
		/////cycle through lists
		current=todelete;
		while(current != NULL) {
			grid[current->val]->alive=FALSE;
			current = current->next;
			pop(&todelete);
		}
		current=toadd;
		while(current != NULL) {
			grid[current->val]->alive=TRUE;
			current = current->next;
			pop(&toadd);
		}
		if(automate) usleep(usecs);
	}
	
	endwin();
	fclose(log);
	for(i=0;i<=GRIDSIZE;i++) {
		free(grid[i]);
	}
	return 0;
}

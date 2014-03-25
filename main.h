#ifndef _main_h
#define _main_h

#define BOOL char
#define TRUE 1
#define FALSE 0
#define GRIDSIZE 1919
// 80 x 24

typedef struct{
	BOOL alive;
	int x;
	int y;
	int age;
} Cell;

typedef struct node {
	int val;
	struct node *next;
} node_t;


Cell *grid[GRIDSIZE+1];
int birthrate;
int automate;
BOOL life_exists = FALSE;
///////////////////////////
Cell *newCell(int x,int y);
void generateGrid();
void push(node_t **head,int val);
int pop(node_t **head);

int rand_lim(int limit);

#endif

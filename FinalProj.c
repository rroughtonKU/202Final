// Rebecca Roughton
// 202 Final
// Minesweeper for Terminal


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable:4996)
// Defines the size of the board to be 10x10
#define BOARDSIZE 10
// Defines the amount of mines to be 10
#define MINECOUNT 10
// Defines basic sizeof command so messy code isn't used later
#define SIZEOFA(arr) (sizeof(arr) / sizeof(0[arr]))


// Defines Section, which is basically an array with a count
typedef struct
{
	int Section[140];
	int count;
}SSection;

// all methods are described in detail when they are defined
int isIn(int value, int array[], int len);
int isSafe(int selection);
int checkPosition(int selection);
int checkAdjMines(int selection);
int mineList[MINECOUNT] = {0};
int board[BOARDSIZE][BOARDSIZE];
int hasWon();
int getRandom(int min, int max);

void addSection(int selection, int flag);
void moveEdge(int selection);
void moveEdgeRight(int selection);
void moveEdgeleft(int selection);
void moveEdgeUp(int selection);
void moveEdgeDown(int selection);
void printBoard( int flag);
void askInput(int* number, char* prompt);
void initialize();
void mineListGenerator();


SSection safe;
SSection edge;
SSection flags;

/****************************
Method : Main
Input = none
Return = 0 when dinifhed

Main gameloop. Repeats until the player has won or lost
****************************/
int main(){
   
   clock_t t;
   double timeUsed; 
   t = clock();
	
   int selection = -1;
	int flag = -1;
	int option = 0;

	initialize();

	int temp = checkAdjMines(0);
	
	printBoard( 0);

	while (1==1){
      // checks if the win condition is true
		if (hasWon()){
			printf("\nA winner is you! \n");
         t = clock()-t;
         timeUsed=((double)t) / CLOCKS_PER_SEC;
         printf("Time: %f", timeUsed);
			break;
		}
      
      // takes in one of two options
		askInput(&option, "\nType 1 to select, 2 to flag: ");

		if (option == 1){
         // takes in input as a double-digit number 
			askInput(&selection, "Select (type [row#][col#])): \n");

         // Checks to see if the selection has already been discovered
			if (isIn(selection, edge.Section, edge.count) || isIn(selection, safe.Section, safe.count) || isIn(selection, flags.Section, flags.count)){
				printf("Already marked, enter another select\n");
				continue;
			}

         // if the selection is the same as the number on the list of mines, give a gameover
			if (isIn(selection, mineList, SIZEOFA(mineList))){
				system("clear");
				printf("\nGameover\n\n");
				printBoard( 1); // send the trigger for the game being lost
				break;
			}
			else{
				system("clear");
            
            // isSafe checks to make sure that 
				if (isSafe(selection)){
					printBoard( 0);
				}
			}
		}
      // if the user inputs two for flagging, go through this loop
		else if (option == 2){
			askInput(&flag, "Flag: \n");
         
         // if the space is already checked ()
			if (isIn(flag, safe.Section, safe.count) || isIn(flag, edge.Section, edge.count) || 
            isIn(flag, flags.Section, flags.count)){
				
            printf("Already marked, please select again\n");
				continue;
			}
         // otherwise, if it checks out, put down a flag and add it 
			else{
            
            // the number of flags already put down must be less than the amount of mines
				if (flags.count < MINECOUNT){
            
					system("clear");
					flags.Section[flags.count] = flag;
					flags.count++;
					printBoard( 0);
				}
            
            // otherwise, say that there's too many flags placed 
				else{
            
					printf("Flagged too many. Please retry\n");
				}
			}
		}
      
      // otherwise, if they input anything else, say that something went wrong
      else {
         printf("Incorrect input, please retry");
         continue;
      }
	}
	return 0;
}

/****************************
Method : initialize
Input = none
        
Return = void
         

Starts off everything that needs to happen before the user may input
****************************/

void initialize(){
	int i, j, k = 0;

	safe.count = 0;
	edge.count = 0;
	flags.count = 0;

   // for everyspace, set its attributes to -1 to start
	for (i = 0; i<100; i++){
		safe.Section[i] = -1;
	}

	for (i = 0; i<100; i++){
		edge.Section[i] = -1;
	}

	for (i = 0; i<MINECOUNT; i++){ 
		flags.Section[i] = -1;
	}


	for (i = 0; i < BOARDSIZE; i++){
		for (j = 0; j<BOARDSIZE; j++){
			board[i][j] = k;
			k++;
		}
	}

	mineListGenerator(); // generates mineList
}

/****************************
Method : askInput
Input = int* number - 
        char* prompt - The string to prompt the input
        
Return = void
         

Basic request-input-from-user function. Used to minimize repeated code
****************************/

void askInput(int* number, char* prompt){
	
   int selection;

	printf("%s", prompt);
	scanf("%d", &selection);

	*number = selection;
}


/****************************
Method : printBoard
Input = int hasLost - trigger to print the "lost" screen
        
Return = void

Standard print function for the board for playing and losing states
****************************/

void printBoard(int hasLost){
	int i;
	int j;
 
	printf("    ");
   
	for (i = 0; i < 10; i++){
		printf("%d  ", i);
	}
	printf("\n\n");

   // for the size of the board, print out the proper symbols
	for (i = 0; i < BOARDSIZE; i++){
		printf("%d  ", i);
		for (j = 0; j < BOARDSIZE; j++){

         // if the user has lost, show where the bombs were
			if (hasLost == 1)
			{
				if (isIn(board[i][j], mineList, MINECOUNT)){
					printf(" B ");
				}
				else if (isIn(board[i][j], flags.Section, flags.count)){
					printf(" F ");
				}
				else if (isIn(board[i][j], edge.Section, edge.count)){
					printf(" %d ", checkAdjMines(board[i][j]));
				}
				else if (isIn(board[i][j], safe.Section, safe.count)){
					printf(" . ");
				}
				else{
					printf(" ? ");
				}
			}
         
         //otherwise just print the normal board
			else
			{
				if (isIn(board[i][j], flags.Section, flags.count)){
					printf(" F ");
				}
				else if (isIn(board[i][j], edge.Section, edge.count)){
					printf(" %d ", checkAdjMines(board[i][j]));
				}
				else if (isIn(board[i][j], safe.Section, safe.count)){
					printf(" . ");
				}
				else{
					printf(" ? ");
				}
			}
		}
		printf("\n");
	}

}

/****************************
Method : isIn
Input = int value - element to search for
        int array[] - array to search in
        int length - amount to search over
        
Return = int - 1 for is in, 0 for isn't in

Basic search-an-array-for-a-value function
****************************/

int isIn(int value, int array[], int length){
	int i = 0;
   
   // go through the array to see if it's in there
	for (i = 0; i < length; i++){
		if (array[i] == value){
			return 1; // it's in there
		}
	}

	return 0; // its not in there
}

/****************************
Method : isSafe
Input = int select - the value 
        
Return = int - 1 for is safe, - for isn't safe

Checks all conditions to see if the selection was anything
besides a number or blank space
****************************/

int isSafe(int selection){

	if (selection > 99 || selection < 0){
		printf("Select exceeds range\n");
		return 0;
	}

	int safeCount = safe.count;
	int edgeCount = edge.count;

   // as long as the selection doesn't have 0 mines next to it or 9 
	if ((checkAdjMines(selection) != 0 && checkAdjMines(selection) != 9) ){
		addSection(selection, 2);
	}

   // otherwise, shifts edge 
	else{
		addSection(selection, 1);
		moveEdge(selection);
	}
   
	return 1;

}

/****************************
Method : moveEdge
Input = selection - selection given
        
Return = void

Moves the edges of the Section
****************************/

void moveEdge(int selection){
   
   
   moveEdgeLeft(selection);
   moveEdgeRight(selection);
}

/****************************
Method : moveEdgeLeft 
Input = selection - selection given
        
Return = void

Moves the edge of Left. The next few funtions have
similar functionality, so comments here can be applied there.
To keep the code easy to read, redundant comments don't appear
****************************/

void moveEdgeLeft(int selection){
   
   int currentPos = selection;
	int position = checkPosition(selection);
	int flagEdge = 0;
   
   // while there aren't mines next to it
	while (checkAdjMines(currentPos) == 0)
	{
		addSection(currentPos, 1);

      // checks edges to the left and right
		moveEdgeDown(currentPos);
      moveEdgeUp(currentPos);

		currentPos--;
      
      // if its an edge
		if (position == 4 || position == 6 || position == 8){
			flagEdge = 1;
			break;
		}
		position = checkPosition(currentPos);
	}
   // only add if not an edge
	if (flagEdge == 0)
	{
		addSection(currentPos, 2);
	}

}

/****************************
Method : moveEdgeRight 
Input = selection - selection given
        
Return = void

Moves the edge of the Section Right
****************************/

void moveEdgeRight(int selection){
   // flags that its at an edge
	int flagEdge = 0;
	int currentPos = selection;
	int position = checkPosition(selection);

	while (checkAdjMines(currentPos) == 0)
	{
		addSection(currentPos, 1);

		moveEdgeDown(currentPos);
      moveEdgeUp(currentPos);

		currentPos++;
		if (position == 5 || position == 7 || position == 9){
			flagEdge = 1;
			break;
		}
		position = checkPosition(currentPos);
	}

	if (flagEdge == 0)
	{
		addSection(currentPos, 2);
	}

}

/****************************
Method : moveEdgeDown
Input = int slection - user input
        
Return = void
         

Moves the edge down
****************************/

void moveEdgeDown(int selection){

	int currentPos = selection;
	int position = checkPosition(selection);
	int flagEdge = 0;
   
	while (checkAdjMines(currentPos) == 0)
	{
		addSection(currentPos, 1);
      
      
		currentPos -= 10;
		if (position == 2 || position == 6 || position == 7){
			flagEdge = 1;
			break;
		}
		position = checkPosition(currentPos);
	}
   
	if (flagEdge == 0)
	{
		addSection(currentPos, 2);
	}

}

/****************************
Method : moveEdgeDown
Input = int slection - user input
        
Return = void
         

Moves the edge down
****************************/

void moveEdgeUp(int selection){
	int currentPos = selection;
	int position = checkPosition(selection);
	int flagEdge = 0;
   
	while (checkAdjMines(currentPos) == 0)
	{
		addSection(currentPos, 1);
      
      
		currentPos += 10;
		
      if (position == 3 || position == 8 || position == 9){
			flagEdge = 1;
			break;
		}
		position = checkPosition(currentPos);
	}
   
	if (flagEdge == 0)
	{
		addSection(currentPos, 2);
	}
   
	flagEdge = 0;

}

/****************************
Method : addSection
Input = int selection - user input
        int flag - 
        
Return = void
         

adds selection based on if its a safe add or an edge add
****************************/

void addSection(int selection, int option){
	
   switch (option)
	{
		case 1:
         // if the selection isn't in safe.Section
			if (!(isIn(selection, safe.Section, safe.count))){
				
            safe.Section[safe.count] = selection;
				safe.count++;
			}
			break;
		case 2:
         // if the selection isn't in edge.Section
			if (!(isIn(selection, edge.Section, edge.count))){
				
            edge.Section[edge.count] = selection;
				edge.count++;
			}
			break;
      // should never hit this condition
		default:
			break;
	}

}

/****************************
Method : hasWon
Input = None
        
Return = int - 1 for has won, 0 for hasn't won
         

Checks if the flags are at the right position to trigger the win state
****************************/

int hasWon(){

	int i = 0;
	int count = 0;

   // counts for every mine in the flag section
	for (i = 0; i < MINECOUNT; i++){
		if (isIn(mineList[i], flags.Section, flags.count)){
			count++;
		}
	}
   
   // if the number of mines equals the number of flags properly placed, win
	if (count == MINECOUNT){
		return 1;
	}
   
   //otherwise, it's not won yet
	else{
		return 0;
	}
}



/****************************
Method : check AdjMines
Input = int selection - user input
        
Return = int - the number of bombs around it
         

checks for mines around a space.
****************************/

int checkAdjMines(int selection){

	int count = 0;
	int currentPosition = checkPosition(selection);
	int col = selection % 10;
	int row = selection / (int)10;

	if (isIn(board[row][col], mineList, MINECOUNT))
	{
		return 9;
	}

   // depending on the location, it needs to check the different
   // areas around the current position, so while the if elses
   // are numerous, the general funciton is the same. Adds a count for
   // each one
   // 1 means that its within the board, not on the edge. This means it needs
   // to check every square around it
	if (currentPosition == 1){
		if (isIn(board[row + 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col - 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row + 1][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row + 1][col - 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col - 1], mineList, MINECOUNT))
			count++;
	}
   
   // 2 means that its within the first row, so don't check for mines
   // atop it
   
	else if (currentPosition == 2){
		if (isIn(board[row + 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col - 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row + 1][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row + 1][col - 1], mineList, MINECOUNT))
			count++;
	}
   
   // 3 means that its the bottommost row, so don't check bellow
	else if (currentPosition == 3){
		if (isIn(board[row - 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col - 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col - 1], mineList, MINECOUNT))
			count++;
	}
   
   //4 means its on the leftmost col, so don't check to the left
	else if (currentPosition == 4){
		if (isIn(board[row + 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row + 1][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col + 1], mineList, MINECOUNT))
			count++;
	}
   
   // five means that its all the way on the right, so don't check to the right
	else if (currentPosition == 5){
		if (isIn(board[row + 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col - 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row + 1][col - 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col - 1], mineList, MINECOUNT))
			count++;
	}
   
   // 6 means that its at 0,0, so only check right, bottom, and bottomright corner
	else if (currentPosition == 6){
		if (isIn(board[row + 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row + 1][col + 1], mineList, MINECOUNT))
			count++;
	}
   
   // 7 means that 0,9, so only check left, bottom, and bottomleft
	else if (currentPosition == 7){
		if (isIn(board[row + 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col - 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row + 1][col - 1], mineList, MINECOUNT))
			count++;
	}
   
   // 8 means its at 9,0, so only check top, right, and topright
	else if (currentPosition == 8){
		if (isIn(board[row - 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col + 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col + 1], mineList, MINECOUNT))
			count++;
	}
   
   // 9 means its at 9,9, so only check left, top, and topleft
	else if (currentPosition == 9){
		if (isIn(board[row - 1][col], mineList, MINECOUNT))
			count++;
		if (isIn(board[row][col - 1], mineList, MINECOUNT))
			count++;
		if (isIn(board[row - 1][col - 1], mineList, MINECOUNT))
			count++;
	}

	return count;
}

/****************************
Method : getRandom
Input = int min - minimum acceptable value
        int max - maximum acceptable value
        
Return = int - random value between the min and the max
         

gets a random number between the min and the max
****************************/

int getRandom(int min, int max){
	return rand() % (max - min + 1) + min;
}

/****************************
Method : mineListGenerator
Input = none
        
Return = void
         

generates the minelist
****************************/

void mineListGenerator(){

   int bombLocation = 0;	
   int i = 0;

	for (i = 0; i < MINECOUNT; i++){
   
		while (1==1){
			bombLocation = getRandom(0, 99);
			if (!isIn(bombLocation, mineList, MINECOUNT)){
				break; // if the random number isn't in, stop to add.
			}
		}
      
		mineList[i] = bombLocation; // add bomb
	}
}

/****************************
Method : checkPosition
Input = int selection - the selection from the user
        
Return = int - returns the position number
         

Used to find the location of the selection within the board.
This is used for edge detection
****************************/

int checkPosition(int selection){
	
   int col = selection % 10;
	int row = selection / (int)10;

   // not on the edge of the board
	if ((row > 0 && row < BOARDSIZE - 1) && (col>0 && col<BOARDSIZE - 1)){
		return 1;
	}
   
   // top row
	else if (row == 0 && (col>0 && col<BOARDSIZE - 1)){
		return 2;
	}
   
   // bottom row
	else if (row == BOARDSIZE - 1 && (col>0 && col<BOARDSIZE - 1)){
		return 3;
	}
   
   // leftmost col
	else if ((row>0 && row < BOARDSIZE - 1) && (col == 0)){
		return 4;
	}
   
   // rightmost col
	else if ((row>0 && row < BOARDSIZE - 1) && (col == BOARDSIZE - 1)){
		return 5;
	}
   
   // left top corner
	else if (row == 0 && col == 0){
		return 6;
	}
   
   // right top corner
	else if (row == 0 && col == BOARDSIZE - 1){
		return 7;
	}
   
   // bottom left corner
	else if (row == BOARDSIZE - 1 && col == 0){
		return 8;
	}
   
   // bottom right corner
	else if (row == BOARDSIZE - 1 && col == BOARDSIZE - 1){
		return 9;
	}

	return 0;
}
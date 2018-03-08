/***********************************************************************
* Program:
*    Lesson 08, Tic-Tac-Toe
* Summary: 
*    This program reads, displays, and writes a Tic-Tac-Toe board
************************************************************************/

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
using namespace std;
#define X 'X'
#define O 'O'
#define DOT '.'
#define BOARD_SIZE 5
#define MAX_LEVELS 5
bool read( char board[][BOARD_SIZE][BOARD_SIZE], const char* fileName, int & levels);
bool write(  const char boar[][BOARD_SIZE][BOARD_SIZE], const char* fileName, int levels);
void display(const char board[][BOARD_SIZE][BOARD_SIZE], int levels);
bool didWin( const char board[][BOARD_SIZE][BOARD_SIZE], char turn, int levels);

/**********************************************************************
 * Keeps the data and calles the read/display/write functions
 ***********************************************************************/
int main()
{
   char board[MAX_LEVELS][BOARD_SIZE][BOARD_SIZE];
   int levels = 0;
   // read the board
   char fileName[256];
   cout << "Enter source filename: ";
   cin  >> fileName;
   if (!read(board, fileName, levels))
   {
      cout << "ERROR: Unable to open file \"" << fileName << "\"\n";
      return 1;
   }

   // display the board
   display(board, levels);

   // write the board to a file
   cout << "Enter destination filename: ";
   cin  >> fileName;
   if (!write(board, fileName, levels))
   {
      cout << "ERROR: Unable to open file \"" << fileName << "\"\n";
      return 1;
   }

   return 0;
}

/**************************************************************
 * READ
 * Read the board from the specified filename
 *************************************************************/
bool read(char board[][BOARD_SIZE][BOARD_SIZE], const char* fileName, int & levels)
{
   assert(*fileName);

   // open the file
   ifstream fin(fileName);
   if (fin.fail())
      return false;
   // read 9 symbols, hopefully they are . X O
   for (int r = 0; r < BOARD_SIZE; r++)
	   for (int c = 0; c < BOARD_SIZE; c++)
	   {

		   fin >> board[levels][r][c];
		   if (fin.eof())
		   {
			   fin.close();
			   return true;
		   }
		   assert(!fin.fail());
		   assert(board[levels][r][c] == X ||
			   board[levels][r][c] == O ||
			   board[levels][r][c] == DOT);
		   if (c == BOARD_SIZE - 1 && r == BOARD_SIZE - 1 && !fin.eof())
		   {
			   levels++;
			   c = -1;
			   r = 0;
		   }
	   }
   // close the file
   fin.close();
   return true;
}

/**********************************************************
 * WRITE
 * Write to fileName the board data
 *********************************************************/
bool write(const char board[][BOARD_SIZE][BOARD_SIZE], const char* fileName, int levels)
{
   assert(fileName[0] != '\0');

   // open the file
   ofstream fout(fileName);
   if (fout.fail())
      return false;

   for (int i = 0; i < levels; i++)
   {
	   for (int r = 0; r < BOARD_SIZE; r++)
		   for (int c = 0; c < BOARD_SIZE; c++)
			   fout << board[i][r][c] << (c == (BOARD_SIZE - 1) ? '\n' : ' ');
   }

   // close it!
   fout.close();
   cout << "File written\n";
   return true;
}

/*******************************************************
 * DISPLAY
 * Display the contents the the screen
 *****************************************************/
void display(const char board[][BOARD_SIZE][BOARD_SIZE], int levels)
{
	//make the divider length appropiate
	string divider = "";
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		
		if (i != (BOARD_SIZE - 1))
		{
			divider += "---+";
		}
		else
		{
			divider += "---\n";
		}
	}
	for (int i = 0; i < levels; i++)
	{
		if (i == 0 && levels > 0)
		{
			cout << "Level " << i + 1 << endl;
		}
		else if (levels > 0)
		{
			cout << "\nLevel " << i + 1 << endl;
		}
		// loop through each row
		for (int r = 0; r < BOARD_SIZE; r++)
		{
			// only the first row is not preceeded with the --+-- magic
			if (r != 0)
				cout << divider.c_str();

			// now, on each row, do the column stuff
			for (int c = 0; c < BOARD_SIZE; c++)
			{
				// display a space for the dot
				if (board[i][r][c] == DOT)
					cout << "   ";
				else
					cout << " " << board[i][r][c] << " ";

				// end with a | or a newline
				cout << (c == (BOARD_SIZE - 1) ? '\n' : '|');
			}
		}
	}

	// display who won
	if (didWin(board, X, levels))
		cout << "X won!\n";
	if (didWin(board, O, levels))
		cout << "O won!\n";
   
   return;
}

/********************************************
 * DID WIN
 * Did a given player (determined by the "turn"
 * variable) win the game?
 *******************************************/
bool didWin(const char board[][BOARD_SIZE][BOARD_SIZE], char turn, int levels)
{
	
	for (int i = 0; i < levels; i++)
	{
		bool isLeftToRightDiagnolWon = true;
		bool isRightToLeftDiagnolWon = true;
		int colCounter = 0;
		int highestCount = 0;
		//check col first
		for (int r = 0; r < BOARD_SIZE; r++)
		{
			for (int c = 0; c < BOARD_SIZE; c++)
			{

				//we do not have a match
				if (board[i][r][c] != turn)
				{
					//check if it is diagnol
					//if so and it isnt the turn diagnol is false
					if (r == c && isLeftToRightDiagnolWon)
					{
						isLeftToRightDiagnolWon = false;
					}
					else if ((r + c) == (BOARD_SIZE - 1) && isRightToLeftDiagnolWon)
					{
						isRightToLeftDiagnolWon = false;
					}
				}
				else
				{
					//count the columns
					colCounter++;
				}
				//we are about to change rows, reset
				if (c == (BOARD_SIZE - 1))
				{
					if (highestCount < colCounter)
					{
						//store the count
						highestCount = colCounter;
					}
					colCounter = 0;
				}
			}
		}
		//check if we won diagnally, or with our columns
		if (isLeftToRightDiagnolWon || isRightToLeftDiagnolWon || highestCount == BOARD_SIZE)
		{
			return true;
		}
	}
	
	//loop through the levels
	for (int i = 0; i < levels; i++)
	{
		//loop through again this time checking our rows
		for (int c = 0; c < BOARD_SIZE; c++)
		{
			for (int r = 0; r < BOARD_SIZE; r++)
			{
				//we do not have a match move to next row
				if (board[i][r][c] != turn)
				{
					break;
				}
				//check if we are at the end and it matches if so we won
				else if (r == (BOARD_SIZE - 1) && board[i][r][c] == turn)
				{
					return true;
				}
			}
		}
	}
    return false;
}

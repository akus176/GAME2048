#pragma once
#include "Functions.h"

// Function to initialize game
void initializeGame(GamePlay2048& GamePlay);

// Function to create a new game
void NewGame(GamePlay2048& GamePlay, List& list);  

// Function to add an element to the game board
void addToTable(GamePlay2048& GamePlay); 

// Function to create a player by entering a name
void createPlayer(string name, GamePlay2048& GamePlay); 

// Function to normalize the player's name
void reduceName(string& name); 

// Function to find the best score 
int findBestScore(List list);

// Function to find player's rank
int findPlayerRank(List list, string name);

// Function to find the content of a cell given its starting coordinates
string findContent(Point current, const Point initial);

// Function to find the text color of a cell given its starting coordinates
int findTextColor(Point current, const Point initial);

// Function to check if username already exists
bool checkNameExist(string name, List list); 

// Function to sort the list 20 players
void sortByScoreAndTime(List& list);

// Function to realease memory for a double pointer
void deleteMemories(int** Matrix, int n);

// Function to release the linked list except for Player
void releaseList(List& list); 

// Function to write the list to a binary file
void writeListToFileBin(List list, string fileName); 

// Function to update the linked list
void updateList(List& list, Player* newPlayer);

// Function to read the list from a binary file
void readPlayerFromFileBin(List& list, string fileName);

// Function to create a player from a binary file
Player* createPlayerFromFileBin(fstream& input); 

// Function to update the List of 5 accounts
void updateListAccount(List& listAccount, Player* account, int pos = 0);

// Function to save the game state to a binary file
void saveGameToFile(GamePlay2048 GamePlay, string fileName);

// Function to load the game from a binary file
void loadGameFromFile(GamePlay2048& GamePlay, string fileName); 

// Function to save the stack to a file
void saveStackToFile(Stack stack, string fileName);

// Function to create a new GamePlay from a file
GamePlay2048* createGamePlayFromFile(fstream& input);

// Function to add a GamePlay tail of the stack
void addTailStack(Stack& stack, GamePlay2048* GamePlay);

// Function to read a file into the stack 
void readStackFromFile(Stack& stack, string fileName);

// Initial function to start the game 2048
void playGame();
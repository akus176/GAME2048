#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <conio.h>
#include <Windows.h>
#include <iomanip>
#include <string>
#include <fstream>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define NEW_GAME 110
#define EXIT 101
#define UNDO 117
#define REDO 114
#define ENTER 13
#define BACK 98
#define PLAY 112
#define REMOVE_BLOCK 105
#define ON_UNDO 97
#define ON_RED0 65
#define OFF_UNDO 100
#define OFF_REDO 68
#define ON_RANDOMCOLOR 113
#define OFF_RANDOMCOLOR 81
#define ON_REMOVEBLOCK 122
#define OFF_REMOVEBLOCK 90
#define RESIZE 99
#define YES 89
#define NO 78
#define WIN 2048
#define WIDTH 20
#define HEIGHT 4
#define WIDTH_CELL 8
#define HEIGHT_CELL 4
#define BLACK 232
#define BLUE 4
#define GREEN 2
#define YELLOW 3
#define RED 1
#define PURPLE 5
#define SKYBLUE 6 
#define WHITE 7
#define GREY 8
#define PINK 9
#define BRIGHT_GREEN 10
#define IVORY 11
#define AQUA 14
#define BRIGHT_BLUE 63
#define ORANGE 202

using namespace std;

struct Time {
	int start;
	int duration;
	int end;
};

struct Player {
	string name;
	int score;
	Time time;
	Player* next;

	Player() {
		name = "";
		score = 0;
		time.start = 0;
		time.duration = 0;
		time.end = 0;
		next = NULL;
	}
};

struct List {
	Player* head;
	int size;

	List() {
		head = NULL;
		size = 0;
	}
};

struct GamePlay2048 {
	int** Table;
	int TableSize;
	Player* player;
	int savedAccount;
	int bestScore;
	bool isMove;
	bool isChange;
	bool isUndo;
	bool isRedo;
	bool won;
	bool isResumeMode;
	bool randomColor;
	bool removeBlock;
	GamePlay2048* next;

	GamePlay2048() {
		Table = NULL;
		TableSize = 4;
		player = NULL;
		savedAccount = 0;
		bestScore = 0;
		isMove = true;
		isChange = true;
		isUndo = false;
		isRedo = false;
		won = false;
		isResumeMode = false;
		randomColor = false;
		removeBlock = false;
		next = NULL;
	}
};

struct Point {
	int x; 
	int y;
};  

struct Stack {
	GamePlay2048* topGamePlay = NULL;

	// Check if the stack is empty
	bool isEmpty(); 

	// Push an element onto the stack
	void push(GamePlay2048 otherGamePlay); 

	// Pop an element from the stack
	void pop(); 

	// Get the top element of the stack without removing it
	void top(GamePlay2048& otherGamePlay); 

	// Clear all old states stored in the stack	
	void clear();
};

// Function to handle the win condition
bool checkWin(int a);  

// Function to handle the lose condition
bool checkLose(GamePlay2048 GamePlay); 

// Function to handle player move up in the game board
void processMoveUp(GamePlay2048& GamePlay, Stack& UndoStack, List& list); 

// Function to handle player move down in the game board
void processMoveDown(GamePlay2048& GamePlay, Stack& UndoStack, List& list); 

// Function to handle player move left in the game board
void processMoveLeft(GamePlay2048& GamePlay, Stack& UndoStack, List& list); 

// Function to handle player move right in the game board
void processMoveRight(GamePlay2048& GamePlay, Stack& UndoStack, List& list); 

// Function to handle player undo action
void processUndo(GamePlay2048& GamePlay, Stack& UndoStack, Stack& RedoStack);

//  Function to handle player redo action
void processRedo(GamePlay2048& GamePlay, Stack& RedoStack);

// Function to handle player exit action
void processExit(GamePlay2048& GamePlay); 

// Function to handle player remove block action
void processRemoveBlock(GamePlay2048& GamePlay, Stack& UndoStack, List& list); 

// Function to handle player actions in the game board
void processGame(GamePlay2048& GamePlay, List& list); 

// Function to handle player actions in the game start interface
void processGameStart(Point& current, const Point initial, GamePlay2048& Gameplay, List& list); 

// Function to handle player move up in the game start interface
void processMoveUp(Point& current, const Point initial); 

// Function to handle player move down in the game start interface
void processMoveDown(Point& current, const Point initial); 

// Function to handle player move left in the game start interface
void processMoveLeft(Point& current, const Point initial); 

// Function to handle player move right in the game start interface
void processMoveRight(Point& current, const Point initial);

// Function to handle the Enter key press in the game start interface
void processEnter(Point current, const Point initial, GamePlay2048& GamePlay, List& list, char choice, int& flag, bool& is_YESNO);

// Function to handle the New Game feature
void processNewGame(GamePlay2048& GamePlay, Stack& UndoStack, Stack& RedoStack);

// Function to handle the exit feature
void processExit(GamePlay2048& GamePlay);

// Function to handle when the player presses the No key
void processNo(GamePlay2048& GamePlay);

// Function to handle when the player presses the Yes key
void processYes(GamePlay2048& GamePlay, List& list, Stack& UndoStack, Stack& RedoStack);

// "Function to handle pre-game tasks before entering the main game
void preGameSetUp(GamePlay2048& Gameplay, List& list); 

// Function to update the player's score
void updateScore(GamePlay2048& GamePlay, int a);

// Function to handle when the player clicks on "TopPlayer"
void TopPlayers(List& list); 

// Function to handle when the player clicks on "About"
void About(const Point initial); 

// Function to handle when the player clicks on "Setting"
void Setting(GamePlay2048& GamePlay, char& choice); 

// Function to handle when the player clicks on "Resume"
void Resume(GamePlay2048& GamePlay, List& list, char choice);

// Function to handle when the player clicks on "AdvancedFunction"
void AdvancedFunction(GamePlay2048& GamePlay, char& choice); 

//
void GiamDiem(List& list);



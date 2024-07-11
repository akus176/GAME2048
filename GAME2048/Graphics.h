#pragma once
#include "Functions.h"

// Function to set color
void setColor(int background_color, int text_color);

// Function to reset color for the next task
void resetColor();

// Function to move the cursor to the 
// coordinates x, y on the console window
void gotoxy(int x, int y);

// Function for the game start interface
void GameStartUI(const Point initial);

// Function for the main game interface during play
void GameUI(GamePlay2048& GamePlay);

// Function to draw a box
void drawBox(Point point, int b_color, int t_color, string content, int width, int height);

// Function to draw a cell for the game board
void drawTableCell(int x, int y);

// Function to draw the game board
void drawTable(GamePlay2048& GamePlay);

// Function to draw the game logo
void drawLogo();

// Function to draw the losing interface
void LoseUI(GamePlay2048& GamePlay, List& list);

// Function to draw the winning interface
void WinUI(GamePlay2048& GamePlay, List& list);

// Function to draw the leaderboard interface
void Top20PlayersUI(List& list);

// Function to hide the cursor in the console screen
void showCursorOnScr(bool boolean);


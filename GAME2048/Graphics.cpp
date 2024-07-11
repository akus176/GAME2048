#include "Graphics.h"
#include "Model.h"
#include <iostream>

using namespace std;

void setColor(int background_color, int text_color) {
    cout << "\033[48;5;" << background_color << "m";
    cout << "\033[38;5;" << text_color << "m";
}

void resetColor() {
	cout << "\033[0m";
}

void gotoxy(int x, int y) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(h, c);
}

void GameStartUI(const Point initial) {
	system("cls");

    gotoxy(45, 12);
    setColor(BLACK, GREY);
    cout << ">> Press e to terminate program << ";
    resetColor();

	drawLogo();

    // The first box will be highlighted when drawn
	drawBox(initial, WHITE, BLUE, "NEW GAME", WIDTH, HEIGHT);
	drawBox({ initial.x + WIDTH + 15, initial.y }, BLACK, GREEN, "TOP PLAYERS", WIDTH, HEIGHT);
    drawBox({ initial.x + (WIDTH + 15) * 2, initial.y }, BLACK, PINK, "RESUME", WIDTH, HEIGHT);
	drawBox({ initial.x, initial.y + 10 }, BLACK, YELLOW, "ABOUT", WIDTH, HEIGHT);
	drawBox({ initial.x + WIDTH + 15, initial.y + 10 }, BLACK, RED, "SETTINGS", WIDTH, HEIGHT);
    drawBox({ initial.x + (WIDTH + 15) * 2, initial.y + 10 }, BLACK, ORANGE, "ADVANCED FUNCTION", WIDTH, HEIGHT);
}

void drawTable(GamePlay2048& GamePlay) {

    // Draw grid lines
   for (int i = 0; i < GamePlay.TableSize; i++) {
       for (int j = 0; j < GamePlay.TableSize; j++) {
           drawTableCell(j * WIDTH_CELL, i * HEIGHT_CELL);
       }
   }

   for (int i = 1; i < GamePlay.TableSize; i++) {
       for (int j = 0; j <= GamePlay.TableSize; j++) {
           gotoxy(j * WIDTH_CELL, i * HEIGHT_CELL);
           cout << char(195);
       }
   }
}

void GameUI(GamePlay2048& GamePlay) {

    // Draw game table

    if (GamePlay.randomColor) {
        // Random to avoid getting black
        int color = rand() % 210 + 17;
        if (color == BLACK)
            color += 1;
        setColor(BLACK, color);
        drawTable(GamePlay);
        resetColor();
    }
    else {
        drawTable(GamePlay);
    }

    // Print values in cells 

    Point start, origin;
    for (int i = 0; i < GamePlay.TableSize; i++) {
        for (int j = 0; j < GamePlay.TableSize; j++) {
            string temp = to_string(GamePlay.Table[i][j]);

            origin.x = j * WIDTH_CELL;
            origin.y = i * HEIGHT_CELL;

            start.x = origin.x + (WIDTH_CELL - temp.size()) / 2;
            if (temp.size() % 2 == 1) {
                start.x += 1;
            }

            start.y = origin.y + HEIGHT_CELL / 2;

            gotoxy(start.x, start.y);
            if (GamePlay.Table[i][j] != 0) {
                switch (GamePlay.Table[i][j]) {
                case 2:
                    setColor(BLACK, BRIGHT_BLUE);
                    break;
                case 4: 
                    setColor(BLACK, YELLOW);
                    break;
                case 8: 
                    setColor(BLACK, AQUA);
                    break;
                case 16: 
                    setColor(BLACK, SKYBLUE);
                    break;
                case 32: 
                    setColor(BLACK, BLUE);
                    break;
                case 64: 
                    setColor(BLACK, GREY);
                    break;
                case 128:
                    setColor(BLACK, PINK);
                case 256: 
                    setColor(BLACK, PURPLE);
                    break;
                case 512:
                    setColor(BLACK, IVORY);
                    break;
                case 1024:
                    setColor(BLACK, GREEN);
                    break;
                case 2048: 
                    setColor(BLACK, RED);
                    break;
                case 4096: 
                    setColor(BLACK, RED + 100);
                    break;
                case 8192:
                    setColor(BLACK, RED + 105);
                    break;
                case 16384: 
                    setColor(BLACK, RED + 110);
                    break;
                case 32768: 
                    setColor(BLACK, RED + 115);
                    break;
                default:
                    break;
                }

                cout << GamePlay.Table[i][j];
                resetColor();
            }
        }
    }

    // Draw frame for score and best score
    drawBox({ WIDTH_CELL * GamePlay.TableSize + 2, 0 }, BLACK, WHITE, "SCORE", WIDTH - 4, HEIGHT + 1);
    drawBox({ WIDTH_CELL * GamePlay.TableSize + WIDTH + 3, 0 }, BLACK, YELLOW, " BEST SCORE", WIDTH - 4, HEIGHT + 1);
    
    int startX;
    string temp = to_string(GamePlay.player->score);

    startX = WIDTH_CELL * GamePlay.TableSize + 2 + (WIDTH - 4 - temp.size()) / 2;

    if (temp.size() % 2) {
        startX += 1;
    }

    gotoxy(startX, 3);
    cout << GamePlay.player->score;

    setColor(BLACK, YELLOW);
    gotoxy(startX + WIDTH, 3);

    if (GamePlay.player->score > GamePlay.bestScore) {
        cout << GamePlay.player->score;
    }
    else {
        cout << GamePlay.bestScore;
    }

    resetColor();

    setColor(BLACK, WHITE);
    for (int i = 0; i <= WIDTH_CELL * GamePlay.TableSize; i++) {
        gotoxy(i, HEIGHT_CELL * GamePlay.TableSize + 1);
        cout << "-";
    }

    // Print instruction

    gotoxy(0, HEIGHT_CELL * GamePlay.TableSize + 2);
    setColor(BLACK, IVORY);
    cout << "PRESS UP, DOWN, LEFT, RIGHT TO MOVE";
    resetColor();
    
    // Display game functions (depending on settings)

    // If not playing with Resume function, display New Game
    if (!GamePlay.isResumeMode) {
        drawBox({ WIDTH_CELL * GamePlay.TableSize + 2, 10 }, BLACK, BLUE, "n: NEW GAME", WIDTH - 4, HEIGHT - 2);
        drawBox({ WIDTH_CELL * GamePlay.TableSize + WIDTH + 3, 10 }, BLACK, GREEN, "e: EXIT", WIDTH - 4, HEIGHT - 2);
    }
    else {
        drawBox({ WIDTH_CELL * GamePlay.TableSize + 12, 10 }, BLACK, GREEN, "e: EXIT", WIDTH - 4, HEIGHT - 2);
    }

    if (GamePlay.isUndo && GamePlay.isRedo) {
        drawBox({ WIDTH_CELL * GamePlay.TableSize + 2, 13 }, BLACK, PINK, "u: UNDO", WIDTH - 4, HEIGHT - 2);
        drawBox({ WIDTH_CELL * GamePlay.TableSize + WIDTH + 3, 13 }, BLACK, AQUA, "r: REDO", WIDTH - 4, HEIGHT - 2);
    }
    else if (GamePlay.isUndo && !GamePlay.isRedo) 
        drawBox({ WIDTH_CELL * GamePlay.TableSize + 12, 13 }, BLACK, PINK, "u: UNDO", WIDTH - 4, HEIGHT - 2);

    if (GamePlay.removeBlock) {
        drawBox({WIDTH_CELL * GamePlay.TableSize + WIDTH - 9, 7}, BLACK, ORANGE, "i: REMOVE BLOCK", WIDTH - 2, HEIGHT - 2);
    }
}

void drawBox(Point point, int b_color, int t_color, string content, int width, int height) {

    setColor(b_color, t_color);

    // Draw color for the box
    for (int i = point.y + 1; i < point.y + height; i++) {
        for (int j = point.x + 1; j < point.x + width; j++) {
            gotoxy(j, i);
            cout << " ";
        }
    }

    // Find coordinates so that the content is centered inside the box
    int startX = point.x + ((width - content.size()) / 2);

    if (content.size() % 2 == 1) {
        startX += 1;
    }

    int srartY = point.y + height / 2;

    // Print content
    gotoxy(startX, srartY);
    cout << content;

    for (int ix = point.x; ix <= point.x + width; ix++) {

        // Draw the top line of the box
        gotoxy(ix, point.y);
        cout << char(205);

        // Draw the bottom line of the box
        gotoxy(ix, point.y + height);
        cout << char(205);
    }

    for (int iy = point.y; iy <= point.y + height; iy++) {

        // Draw the right line of the box
        gotoxy(point.x, iy);
        cout << char(186);

        // Draw the left line of the box
        gotoxy(point.x + width, iy);
        cout << char(186);
    }

    // Draw the square corners for the box

    gotoxy(point.x, point.y);
    cout << char(201);

    gotoxy(point.x + width, point.y);
    cout << char(187);

    gotoxy(point.x, point.y + height);
    cout << char(200);

    gotoxy(point.x + width, point.y + height);
    cout << char(188);

    resetColor();
}

void drawTableCell(int x, int y) {
    for (int i = x; i <= x + WIDTH_CELL; i++) {
        // Draw the top line of the cell
        gotoxy(i, y);
        cout << char(196);

        //  Draw the bottom line of the cell
        gotoxy(i, y + HEIGHT_CELL);
        cout << char(196);
    }

    for (int j = y; j <= y + HEIGHT_CELL; j++) {
        // Draw the left line of the cell
        gotoxy(x, j);
        cout << char(179);

        // Draw the right line of the cell
        gotoxy(x + WIDTH_CELL, j);
        cout << char(179);
    }

    // Draw the square corners for the cell

    gotoxy(x, y);
    cout << char(218);
    gotoxy(x + WIDTH_CELL, y);
    cout << char(191);
    gotoxy(x, y + HEIGHT_CELL);
    cout << char(192);
    gotoxy(x + WIDTH_CELL, y + HEIGHT_CELL);
    cout << char(217);
}


void drawLogo() {
    gotoxy(15, 1);
    cout << "  /222222   /000000  /44   /44  /888888         /GGGGGG   /AAAAAA  /MM      /MM /EEEEEEEE";
    gotoxy(15, 2);
    cout << " /22__  22 /00  \\ 00| 44  | 44 /88__  88       /GG__  GG /AA__  AA| MMM    /MMM| EE_____/";
    gotoxy(15, 3);
    cout << "|__/  \\ 22| 00  | 00| 44  | 44| 88  \\ 88      | GG  \\__/| AA  \\ AA| MMMM  /MMEE| EE      ";
    gotoxy(15, 4);
    cout << "  /222222/| 00  | 00| 44444444|  888888/      | GG /GGGG| AAAAAAAA| MM MM/MM EE| EEEEE   ";
    gotoxy(15, 5);
    cout << " /22____/ | 00  | 00|_____  44 >88__  88      | GG|_  GG| AA__  AA| MM  MMM| EE| EE__/   ";
    gotoxy(15, 6);
    cout << "| 22      | 00  | 00      | 44| 88  \\ 88      | GG  \\ GG| AA  | AA| MM\\  M | EE| EE      ";
    gotoxy(15, 7);
    cout << "| 22222222|  000000/      | 44|  888888/      |  GGGGGG/| AA  | AA| MM \\/  | EE| EEEEEEEE";
    gotoxy(15, 8);
    cout << "|________/ \\______/       |__/ \\______/        \\______/ |__/  |__/|__/     |__/|________/";
}

// Hàm giao diện thua
void LoseUI(GamePlay2048 &GamePlay, List& list) {
    system("cls");

    // Disable movement when displaying the losing message
    GamePlay.isMove = 0;

    setColor(BLACK, GREY);
    gotoxy(17, 3);
    cout << "  ______    ______   __       __  ________         ______   __     __  ________  _______  " << endl;
    gotoxy(17, 4);
    cout << " /      \\  /      \\ /  \\     /  |/        |       /      \\ /  |   /  |/        |/       \\ " << endl;
    gotoxy(17, 5);
    cout << "/222222  |/000000  |44  \\   /44 |88888888/       /222222  |00 |   00 |44444444/ 8888888  |" << endl;
    gotoxy(17, 6);
    cout << "22 | _22/ 00 |__00 |444  \\ /444 |88 |__          22 |  22 |00 |   00 |44 |__    88 |__88 |" << endl;
    gotoxy(17, 7);
    cout << "22 |/    |00    00 |4444  /4444 |88    |         22 |  22 |00  \\ /00/ 44    |   88    88<" << endl;
    gotoxy(17, 8);
    cout << "22 |2222 |00000000 |44 44 44/44 |88888/          22 |  22 | 00  /00/  44444/    8888888  |" << endl;
    gotoxy(17, 9);
    cout << "22 \\__22 |00 |  00 |44 |444/ 44 |88 |_____       22 \\__22 |  00 00/   44 |_____ 88 |  88 |" << endl;
    gotoxy(17, 10);
    cout << "22    22/ 00 |  00 |44 | 4/  44 |88       |      22    22/    000/    44       |88 |  88 |" << endl;
    gotoxy(17, 11);
    cout << " 222222/  00/   00/ 44/      44/ 88888888/        222222/      0/     44444444/ 88/   88/ " << endl;
    resetColor();

    string content = "YOU LOST! DO YOU WANT TO CONTINUE OR QUIT?";
    drawBox({ 30, 15 }, RED, WHITE, content, WIDTH * 3, HEIGHT * 3 - 1);
    drawBox({ 42, 22 }, RED, WHITE, "Y: QUIT", WIDTH - 4, HEIGHT / 2);
    drawBox({ 62, 22 }, RED, WHITE, "N: CONTINUE", WIDTH - 4, HEIGHT / 2);

    updateList(list, GamePlay.player);

    int rank = findPlayerRank(list, GamePlay.player->name);

    if (rank != 0) {
        gotoxy(46, 18);
        setColor(RED, WHITE);
        cout << "YOUR RANK IN LEADERBOARD: " << rank;
        resetColor();
    }
    else {
        gotoxy(43, 18);
        setColor(RED, WHITE);
        cout << "YOU AREN'T IN THE TOP 20 BEST PLAYERS!";
        resetColor();
    }

}

void WinUI(GamePlay2048 &GamePlay, List& list) {
    system("cls");

    // Disable movement when displaying the winning message
    GamePlay.isMove = 0;

    setColor(BLACK, YELLOW);
    gotoxy(25, 3);
    cout << " __      __  ______   __    __        __       __  ______  __    __ ";
    gotoxy(25, 4);
    cout << "/  \\    /  |/      \\ /  |  /  |      /  |  _  /  |/      |/  \\  /  |";
    gotoxy(25, 5);
    cout << "22  \\  /22//000000  |44 |  44 |      88 | / \\ 88 |222222/ 00  \\ 00 |";
    gotoxy(25, 6);
    cout << " 22  \\/22/ 00 |  00 |44 |  44 |      88 |/8  \\88 |  22 |  000   00 |";
    gotoxy(25, 7);
    cout << "  22  22/  00 |  00 |44 |  44 |      88 /888  88 |  22 |  0000  00 |";
    gotoxy(25, 8);
    cout << "   2222/   00 |  00 |44 |  44 |      88 88/88 88 |  22 |  00 00 00 |";
    gotoxy(25, 9);
    cout << "    22 |   00 \\__00 |44 \\__44 |      8888/  8888 | _22 |_ 00 |0000 |";
    gotoxy(25, 10);
    cout << "    22 |   00    00/ 44    44/       888/    888 |/ 22   |00 | 000 |";
    gotoxy(25, 11);
    cout << "    22/     000000/   444444/        88/      88/ 222222/ 00/   00/ ";
    resetColor();

    string content = "CONGRATULATIONS! DO YOU WANT TO CONTINUE OR QUIT?";
    drawBox({ 30, 15 }, RED, WHITE, content, WIDTH * 3, HEIGHT * 3 - 1);
    drawBox({ 42, 22 }, RED, WHITE, "Y: QUIT", WIDTH - 4,  HEIGHT / 2 );
    drawBox({ 62, 22 }, RED, WHITE, "N: CONTINUE", WIDTH - 4, HEIGHT / 2 );

    updateList(list, GamePlay.player);

    int rank = findPlayerRank(list, GamePlay.player->name);

    if (rank != 0) {
        gotoxy(46, 18);
        setColor(RED, WHITE);
        cout << "YOUR RANK IN LEADERBOARD: " << rank;
        resetColor();
    }
    else {
        gotoxy(43, 18);
        setColor(RED, WHITE);
        cout << "YOU AREN'T IN THE TOP 20 BEST PLAYERS!";
        resetColor();
    }

}

void Top20PlayersUI(List& list) {
    if (list.head == NULL) return;

    setColor(BLACK, RED);
    gotoxy(47, 0);
    cout << "TOP 20 BEST PLAYERS";
    resetColor();

    drawBox({ 4, 2 }, 254, BLUE, " ", WIDTH * 5 + 8, HEIGHT * 6 - 2);

    // Display the categories of the leaderboard
    gotoxy(13, 3);
    setColor(254, GREEN);
    cout << "RANK";
    resetColor();

    gotoxy(41, 3);
    setColor(254, YELLOW);
    cout << "NAME";
    resetColor();

    gotoxy(70, 3);
    setColor(254, PURPLE);
    cout << "SCORE";
    resetColor();

    gotoxy(96, 3);
    setColor(254, PINK);
    cout << "TIME";
    resetColor();

    setColor(254, BLUE);

    // Print separators between the categories in the leaderboard
    for (int i = 0; i < 21; i++) {
        gotoxy(25, 3 + i);
        cout << "|";
        gotoxy(60, 3 + i);
        cout << "|";
        gotoxy(85, 3 + i);
        cout << "|";
    }

    int count = 1;
    for (Player* current = list.head; current != NULL; current = current->next) {

        // Print the player's rank
        gotoxy(14, 3 + count);
        cout << count;

        // Print the player's name
        int startX = (34 - current->name.size()) / 2;
        gotoxy(26 + startX, 3 + count);
        cout << current->name;

        // Print the player's score
        string temp = to_string(current->score);
        startX = (23 - temp.size()) / 2;

        if (temp.size() % 2 == 0) {
            startX += 1;
        }

        gotoxy(61 + startX, 3 + count);
        cout << current->score;

        // Print the player's playing time
        temp = to_string(current->time.duration);
        startX = (32 - temp.size()) / 2;

        gotoxy(82 + startX, 3 + count);
        cout << current->time.duration << "s";

        count++;

        if (count == 21) break;
    }

    resetColor();

    drawBox({ 5, 25 }, BLACK, GREY, "b: BACK", WIDTH / 2, HEIGHT / 2);
}

void showCursorOnScr(bool boolean) {
    CONSOLE_CURSOR_INFO Info;
    if (!boolean) {
        Info.bVisible = FALSE;
    }
    else {
        Info.bVisible = TRUE;
    }
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
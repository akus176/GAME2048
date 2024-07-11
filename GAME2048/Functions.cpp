#include "functions.h"
#include "Graphics.h"
#include "Model.h"

bool checkWin(int a) {
    if (a == WIN) {
        return 1;
    }
    return 0;
}

bool checkLose(GamePlay2048 GamePlay) {

    // Check if there are any cells equal to 0
    for (int i = 0; i < GamePlay.TableSize; i++) {
        for (int j = 0; j < GamePlay.TableSize; j++) {
            if (GamePlay.Table[i][j] == 0)
                 return 0;
        }
    }
    
    // Check if there are any adjacent pairs of elements that can be merged
    for (int i = 0; i < GamePlay.TableSize; i++) {
        for (int j = 0; j < GamePlay.TableSize; j++) {
            if (i < GamePlay.TableSize - 1 && GamePlay.Table[i][j] == GamePlay.Table[i + 1][j]
                || j < GamePlay.TableSize - 1 && GamePlay.Table[i][j] == GamePlay.Table[i][j + 1])
                return 0;
        }
    }

    return 1;
}


void processMoveUp(GamePlay2048& GamePlay, Stack& UndoStack, List& list) {

    if (GamePlay.isChange && GamePlay.isUndo) {
        UndoStack.push(GamePlay);
    }

    if (GamePlay.isMove) {

        if (checkLose(GamePlay)) {
            LoseUI(GamePlay, list);
            return;
        }

        GamePlay.isChange = false;

        int  k, l;
        for (int j = 0; j < GamePlay.TableSize; j++) {
            k = 0, l = j;
            for (int i = 1; i < GamePlay.TableSize; i++) {
                if (GamePlay.Table[i][j] != 0) {
                    if (GamePlay.Table[i - 1][j] == 0 || GamePlay.Table[i - 1][j] == GamePlay.Table[i][j]) {
                        if (GamePlay.Table[k][l] == GamePlay.Table[i][j]) {
                            GamePlay.Table[k][l] *= 2;
                            GamePlay.Table[i][j] = 0;

                            if (checkWin(GamePlay.Table[k][l]) && !GamePlay.won) {
                                GamePlay.won = 1;
                                WinUI(GamePlay, list);
                            }

                            // Update ccore
                            updateScore(GamePlay, GamePlay.Table[k][l]);

                            // Update time
                            GamePlay.player->time.end = clock();
                            GamePlay.player->time.duration = int(GamePlay.player->time.end - GamePlay.player->time.start) / CLOCKS_PER_SEC;
                            
                            // Remove the case of merging between numbers
                            for (int p = k + 1; p < GamePlay.TableSize; p++) {
                                if (GamePlay.Table[p][l] == GamePlay.Table[k][l]) {
                                    k++;
                                    break;
                                }
                            }
                            GamePlay.isChange = true;
                        }
                        else {
                            if (GamePlay.Table[k][l] == 0) {
                                GamePlay.Table[k][l] = GamePlay.Table[i][j];
                                GamePlay.Table[i][j] = 0;
                                GamePlay.isChange = true;
                            }
                            else {
                                k++;
                                GamePlay.Table[k][l] = GamePlay.Table[i][j];
                                GamePlay.Table[i][j] = 0;
                                GamePlay.isChange = true;
                            }
                        }
                    }
                    else
                        k++;
                }
            }
        }

        if (GamePlay.isChange) {
            addToTable(GamePlay); 
        }
    }
}

void processMoveDown(GamePlay2048& GamePlay, Stack& UndoStack, List& list) {

    if (GamePlay.isChange && GamePlay.isUndo) {
        UndoStack.push(GamePlay);
    }

    if (GamePlay.isMove) {
        if (checkLose(GamePlay)) {
            LoseUI(GamePlay, list);
            return;
        }

        GamePlay.isChange = false;

        int k, l;
        for (int j = 0; j < GamePlay.TableSize; j++) {
            k = GamePlay.TableSize - 1; l = j;
            for (int i = GamePlay.TableSize - 2; i >= 0; i--) {
                if (GamePlay.Table[i][j] != 0) {
                    if (GamePlay.Table[i + 1][j] == 0 || GamePlay.Table[i + 1][j] == GamePlay.Table[i][j]) {
                        if (GamePlay.Table[k][l] == GamePlay.Table[i][j]) {
                            GamePlay.Table[k][l] *= 2;
                            GamePlay.Table[i][j] = 0;

                            // Update score
                            updateScore(GamePlay, GamePlay.Table[k][l]);

                            // Update time
                            GamePlay.player->time.end = clock();
                            GamePlay.player->time.duration = int(GamePlay.player->time.end - GamePlay.player->time.start) / CLOCKS_PER_SEC;

                            if (checkWin(GamePlay.Table[k][l]) && !GamePlay.won) {
                                GamePlay.won = 1;
                                WinUI(GamePlay, list);
                                return;
                            }

                            // Remove the case of merging between numbers
                            for (int p = k - 1; p >= 0; p--) {
                                if (GamePlay.Table[p][l] == GamePlay.Table[k][l]) {
                                    k--;
                                    break;
                                }
                            }

                            GamePlay.isChange = true;
                        }
                        else {
                            if (GamePlay.Table[k][l] == 0) {
                                GamePlay.Table[k][l] = GamePlay.Table[i][j];
                                GamePlay.Table[i][j] = 0;
                                GamePlay.isChange = true;
                            }
                            else {
                                k--;
                                GamePlay.Table[k][l] = GamePlay.Table[i][j];
                                GamePlay.Table[i][j] = 0;
                                GamePlay.isChange = true;
                            }
                        }
                    }
                    else
                        k--;
                }
            }
        }


        if (GamePlay.isChange) {
            addToTable(GamePlay);
        }
    }
}

void processMoveLeft(GamePlay2048& GamePlay, Stack& UndoStack, List& list) {
    if (GamePlay.isChange && GamePlay.isUndo) {
        UndoStack.push(GamePlay);
    }

    if (GamePlay.isMove) {
        if (checkLose(GamePlay)) {
            LoseUI(GamePlay, list);
            return;
        }

        GamePlay.isChange = false;

        int k, l;
        for (int i = 0; i < GamePlay.TableSize; i++) {
            k = i, l = 0;
            for (int j = 1; j < GamePlay.TableSize; j++) {
                if (GamePlay.Table[i][j] != 0) {
                    if (GamePlay.Table[i][j - 1] == 0 || GamePlay.Table[i][j - 1] == GamePlay.Table[i][j]) {
                        if (GamePlay.Table[k][l] == GamePlay.Table[i][j]) {
                            GamePlay.Table[k][l] *= 2;
                            GamePlay.Table[i][j] = 0;

                            // update score
                            updateScore(GamePlay, GamePlay.Table[k][l]);

                            // Update Time
                            GamePlay.player->time.end = clock();
                            GamePlay.player->time.duration = int(GamePlay.player->time.end - GamePlay.player->time.start) / CLOCKS_PER_SEC;

                            if (checkWin(GamePlay.Table[k][l]) && !GamePlay.won) {
                                GamePlay.won = 1;
                                WinUI(GamePlay, list);
                                return;
                            }

                            // Remove the case of merging between numbers
                            for (int p = l + 1; p < GamePlay.TableSize; p++) {
                                if (GamePlay.Table[k][p] == GamePlay.Table[k][l]) {
                                    l++;
                                    break;
                                }
                            }

                            GamePlay.isChange = true;
                        }
                        else {
                            if (GamePlay.Table[k][l] == 0) {
                                GamePlay.Table[k][l] = GamePlay.Table[i][j];
                                GamePlay.Table[i][j] = 0;
                                GamePlay.isChange = true;
                            }
                            else {
                                l++;
                                GamePlay.Table[k][l] = GamePlay.Table[i][j];
                                GamePlay.Table[i][j] = 0;
                                GamePlay.isChange = true;
                            }
                        }
                    }
                    else 
                        l++;
                    
                }
            }
        }
        if (GamePlay.isChange) {
            addToTable(GamePlay);
        }
    }
}

void processMoveRight(GamePlay2048& GamePlay, Stack& UndoStack, List& list) {
    if (GamePlay.isChange && GamePlay.isUndo) {
        UndoStack.push(GamePlay);
    }

    if (GamePlay.isMove) {

        if (checkLose(GamePlay)) {
            LoseUI(GamePlay, list);
            return;
        }

        GamePlay.isChange = false;

        int k, l;
        for (int i = 0; i < GamePlay.TableSize; i++) {
            k = i, l = GamePlay.TableSize - 1;
            for (int j = GamePlay.TableSize - 2; j >= 0; j--) {
                if (GamePlay.Table[i][j] != 0) {
                    if (GamePlay.Table[i][j + 1] == 0 || GamePlay.Table[i][j + 1] == GamePlay.Table[i][j]) {
                        if (GamePlay.Table[k][l] == GamePlay.Table[i][j]) {
                            GamePlay.Table[k][l] *= 2;
                            GamePlay.Table[i][j] = 0;

                            // update score
                            updateScore(GamePlay, GamePlay.Table[k][l]);

                            // Update time
                            GamePlay.player->time.end = clock();
                            GamePlay.player->time.duration = int(GamePlay.player->time.end - GamePlay.player->time.start) / CLOCKS_PER_SEC;


                            if (checkWin(GamePlay.Table[k][l]) && !GamePlay.won) {
                                GamePlay.won = 1;
                                WinUI(GamePlay, list);
                                return;
                            }

                            // Remove the case of merging between numbers
                            for (int p = l - 1; p >= 0; p--) {
                                if (GamePlay.Table[k][p] == GamePlay.Table[k][l]) {
                                    l--;
                                    break;
                                }
                            }

                            GamePlay.isChange = true;
                        }
                        else {
                            if (GamePlay.Table[k][l] == 0) {
                                GamePlay.Table[k][l] = GamePlay.Table[i][j];
                                GamePlay.Table[i][j] = 0;
                                GamePlay.isChange = true;
                            }
                            else {
                                l--;
                                GamePlay.Table[k][l] = GamePlay.Table[i][j];
                                GamePlay.Table[i][j] = 0;
                                GamePlay.isChange = true;
                            }
                        }
                    }
                    else l--;
                }
            }
        }
        if (GamePlay.isChange) {
            addToTable(GamePlay);
        }
    }
}

void processGame(GamePlay2048& GamePlay, List& list) {
    Stack UndoStack, RedoStack;

    // If playing in resume mode and enabling undo or redo mode,
    //  load data from the file into the stack
    if (GamePlay.isResumeMode) {
        if (GamePlay.isUndo) {
            string fileName = "Undo" + to_string(GamePlay.savedAccount) + ".bin";
            readStackFromFile(UndoStack, fileName);
        }
        if (GamePlay.isRedo) {
            string fileName = "Redo" + to_string(GamePlay.savedAccount) + ".bin";
            readStackFromFile(RedoStack, fileName);
        }
    }

    while (true) {
        char choice = _getch();

        if (int(choice) == NEW_GAME && GamePlay.isMove) {
            GamePlay.isMove = 0;
            processNewGame(GamePlay, UndoStack, RedoStack);
            break;
        }

        switch (int(choice)) {
        case UP: {
            processMoveUp(GamePlay, UndoStack, list);
            break;
        }
        case DOWN: {
            processMoveDown(GamePlay, UndoStack, list);
            break;
        }
        case LEFT: {
            processMoveLeft(GamePlay, UndoStack, list);
            break;
        }
        case RIGHT: {
            processMoveRight(GamePlay, UndoStack, list);
            break;
        }
        case EXIT: {
            if (GamePlay.isMove) {
                processExit(GamePlay);
            }
            break;
        }
        case NO: {
            if (!GamePlay.isMove) {
                processNo(GamePlay);
            }
            break;
        }
        case YES: {
            if (!GamePlay.isMove) {
                processYes(GamePlay, list, UndoStack, RedoStack);
            }
            break;
        }
        case UNDO: {
            if (GamePlay.isUndo) {
                processUndo(GamePlay, UndoStack, RedoStack);
            }
            break;
        }
        case REDO: {
            if (GamePlay.isRedo) {
                processRedo(GamePlay, RedoStack);
            }
            break;
        }
        case REMOVE_BLOCK: {
            if (GamePlay.removeBlock) {
                processRemoveBlock(GamePlay, UndoStack, list);
            }
            break;
        }
        default: 
            break;
        }
    }
}

void updateScore(GamePlay2048 & GamePlay, int a) {
    switch (a) {
    case 4: {
        GamePlay.player->score += 4;
        break;
    }
    case 8: {
        GamePlay.player->score += 8;
        break;
    }
    case 16: {
        GamePlay.player->score += 16;
        break;
    }
    case 32: {
        GamePlay.player->score += 32;
        break;
    }
    case 64: {
        GamePlay.player->score += 64;
        break;
    }
    case 128: {
        GamePlay.player->score += 128;
        break;
    }
    case 256: {
        GamePlay.player->score += 256;
        break;
    }
    case 512: {
        GamePlay.player->score += 512;
        break;
    }
    case 1024: {
        GamePlay.player->score += 1024;
        break;
    }
    case 2048: {
        GamePlay.player->score += 2048;
        break;
    }
    case 4096: {
        GamePlay.player->score += 4096;
    }
    case 8192: {
        GamePlay.player->score += 8192;
    }
    case 16384: {
        GamePlay.player->score += 16384;
    }
    case 32768: {
        GamePlay.player->score += 32768;
    }
    default:
        break;
    }
}


void processMoveUp(Point& current, const Point initial) {

    if (current.y != initial.y) {

        // Remove the highlight from the current box
        string content = findContent(current, initial);
        int text_color = findTextColor(current, initial);
        drawBox(current, BLACK, text_color, content, WIDTH, HEIGHT);

        // Highlight the next box when moving to it
        current.y = initial.y;
        content = findContent(current, initial);
        text_color = findTextColor(current, initial);
        drawBox(current, WHITE, text_color, content, WIDTH, HEIGHT);
    }
}

void processMoveDown(Point& current, const Point initial) {

    if (current.y == initial.y) {

        string content = findContent(current, initial);
        int text_color = findTextColor(current, initial);
        drawBox(current, BLACK, text_color, content, WIDTH, HEIGHT);

        current.y += 10;
        content = findContent(current, initial);
        text_color = findTextColor(current, initial);
        drawBox(current, WHITE, text_color, content, WIDTH, HEIGHT);
    }
}

void processMoveLeft(Point& current, const Point initial) {

    if (current.x != initial.x) {

        string content = findContent(current, initial);
        int text_color = findTextColor(current, initial);
        drawBox(current, BLACK, text_color, content, WIDTH, HEIGHT);

        current.x = current.x - WIDTH - 15;
        content = findContent(current, initial);
        text_color = findTextColor(current, initial);
        drawBox(current, WHITE, text_color, content, WIDTH, HEIGHT);
    }
}

void processMoveRight(Point& current, const Point initial) {

    if (current.x != initial.x + ( WIDTH + 15 ) * 2) {

        string content = findContent(current, initial);
        int text_color = findTextColor(current, initial);
        drawBox(current, BLACK, text_color, content, WIDTH, HEIGHT);

        current.x = current.x + WIDTH + 15;
        text_color = findTextColor(current, initial);
        content = findContent(current, initial);
        drawBox(current, WHITE, text_color, content, WIDTH, HEIGHT);
    }
}

void About(const Point initial) {
    system("cls");

    drawLogo();

    setColor(BLACK, WHITE);
    resetColor();

    drawBox({ 17,12 }, BLACK, WHITE, "", WIDTH * 4 + 3, HEIGHT * 3 - 2);

    gotoxy(19, 13);
    cout << "  Welcome to ";
    setColor(BLACK, YELLOW);
    cout << "2048 GAME ";
    resetColor();
    cout << "this is project of ";
    setColor(BLACK, PINK);
    cout << "Tran Anh Khoa. ";
    resetColor();
    cout << "This assignment is my";
    gotoxy(19, 14);
    cout << "own work except where I have acknowledged the use of the works of other people.";
    gotoxy(19, 16);
    cout << "  In this game, players use arrow keys to move tiles on a grid with size options.";
    gotoxy(19, 17);
    cout << "Each move creates an empty tile and randomly adds either a 2 or 4. The objective";
    gotoxy(19, 18);
    cout << "is to combine tiles to create higher values, aiming for ";
    setColor(BLACK, YELLOW);
    cout << "2048. ";
    resetColor();
    cout << "Merge identical";
    gotoxy(19, 19);
    cout << "tiles  to double their value and increase your score. The game ends when no more";
    gotoxy(19, 20);
    cout << "moves are possible.";
    setColor(BLACK, BRIGHT_BLUE);
    cout << "Enjoy the strategic challenge and aim for a high score!";
    resetColor();

    drawBox({ 5, 25 }, BLACK, GREY, "b: BACK", WIDTH / 2, HEIGHT / 2);

}
 
void processEnter(Point current, const Point initial, GamePlay2048& GamePlay, List& list, char choice, int& flag, bool& is_YESNO) {
    if (current.x == initial.x) {
        if (current.y == initial.y) {
            flag = 4;
            preGameSetUp(GamePlay, list);
        }
        else {
            About(initial);
        }
    }

    else if (current.x == initial.x + WIDTH + 15) {
        if (current.y == initial.y) {
            TopPlayers(list);
        }
        else {
            flag = 2;
            Setting(GamePlay, choice);
        }
    }

    else {
        if (current.y == initial.y) {
            is_YESNO = 1;
            flag = 3;
            Resume(GamePlay, list, choice);
        }

        else {
            flag = 5;
            AdvancedFunction(GamePlay, choice);
        }
    }
}

void processGameStart(Point& current, const Point initial, GamePlay2048& GamePlay, List& list) {

    // Set a flag to determine whether pressing yes or no is allowed
    bool is_YESNO = 0;

    // Set flags to avoid conflicts between keys when executing functions
    int flag = 0;

    /*
    flag = 0: Not pressed enter yet;
    flag = 1: Pressed enter;
    flag = 2: Pressed "Setting";
    flag = 3: Pressed "Resume";
    flag = 4: Pressed "Play";
    flag = 5: Pressed "AdvancedFunction".
    */

    while (true) {

        char choice = _getch();

        switch (int(choice))
        {
        case UP: {
            if (flag == 0) {
                processMoveUp(current, initial);
            }
            break;
        }
        case DOWN: {
            if (flag == 0) {
                processMoveDown(current, initial);
            }
            break;
        }
        case LEFT: {
            if (flag == 0) {
                processMoveLeft(current, initial);
            }
            break;
        }
        case RIGHT: {
            if (flag == 0) {
                processMoveRight(current, initial);
            }
            break;
        }
        case ENTER: {
            if (flag == 0) {
                flag = 1;
                processEnter(current, initial, GamePlay, list, choice, flag, is_YESNO);
            }
            break;
        }
        case PLAY: {
            if (flag == 4) {

                is_YESNO = 1;
                NewGame(GamePlay, list);
            }
            break;
        }
        case BACK: {
            if (flag != 0) {
                flag = 0;
                GameStartUI(initial);
                current = initial;
            }
            break;
        }
        case NO: {
            if (flag == 4 && is_YESNO == 1) {
                NewGame(GamePlay, list);
            }
            break;
        }
        case YES: {
            if (flag == 4 && is_YESNO == 1) {
                flag = 0; is_YESNO = 0;

                delete GamePlay.player;
                GameStartUI(initial);
                current = initial;
            }
            break;
        }
        case ON_UNDO: {
            if (flag == 2 && !GamePlay.isUndo) {
                Setting(GamePlay, choice);
            }
            break;
        }
        case ON_RED0: {
            if (flag == 2 && GamePlay.isUndo && !GamePlay.isRedo) {
                Setting(GamePlay, choice);
            }
            break;
        }
        case OFF_UNDO: {
            if (flag == 2 && GamePlay.isUndo) {
                Setting(GamePlay, choice);
            }
            break;
        }
        case OFF_REDO: {
            if (flag == 2 && GamePlay.isRedo) {
                Setting(GamePlay, choice);
            }
            break;
        }
        case RESIZE: {
            if (flag == 2) {
                Setting(GamePlay, choice);
            }
            break;
        }
        case ON_RANDOMCOLOR: {
            if (flag == 5 && !GamePlay.randomColor) {
                AdvancedFunction(GamePlay, choice);
            }
            break;
        }
        case OFF_RANDOMCOLOR: {
            if (flag == 5 && GamePlay.randomColor) {
                AdvancedFunction(GamePlay, choice);
            }
            break;
        }
        case ON_REMOVEBLOCK: {
            if (flag == 5 && !GamePlay.removeBlock) {
                AdvancedFunction(GamePlay, choice);
            }
            break;
        }
        case OFF_REMOVEBLOCK: {
            if (flag == 5 && GamePlay.removeBlock) {
                AdvancedFunction(GamePlay, choice);
            }
            break;
        }
        case EXIT: {
            releaseList(list);
            system("cls");
            exit(0);
            break;
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5': {
            if (flag == 3) {
                Resume(GamePlay, list, choice);
            }
            break;
        }
        default:
            break;
        }
    }
}



void preGameSetUp(GamePlay2048& GamePlay, List& list) {

    system("cls");

    // Show cursor
    showCursorOnScr(true);

    drawLogo();

    drawBox({ 5, 25 }, BLACK, GREY, "b: BACK", WIDTH / 2, HEIGHT / 2);
    drawBox({ 105, 25 }, BLACK, YELLOW, "p: PLAY", WIDTH / 2, HEIGHT / 2);

    // Allow the player to input their name
    string name = "";
    bool validName = false;
    List listAccount; // List to store the accounts that have been resumed
    readPlayerFromFileBin(listAccount, "listAccount.bin"); 

    do {
        gotoxy(30, 15);
        cout << "Enter player's name (maximum length is 20 characters): ";
        getline(cin, name);
        reduceName(name);

        // Player name must not be the same in the top 20 and any previously resumed accounts
        validName = !(name == "") && !checkNameExist(name, list) && !checkNameExist(name, listAccount) && !(name.size() > 20);

        gotoxy(30, 16);
        if (!validName) {
            setColor(BLACK, RED);
            cout << "This name is not valid or already exists. Please enter another name!";
            resetColor();

            gotoxy(30, 15);
        }
        else {
            setColor(BLACK, GREEN);
            cout << "This name is valid. You can play the game or back to Game Start now!";
            resetColor();
            gotoxy(30, 15);
            cout << "Your name in this game is: " << name << string(80 - name.size(), ' ');
            gotoxy(30, 17);
        }
    } while (!validName);

    // Find the number of accounts resumed
    string fileName;
    for (int i = 1; i <= 6; i++) {
        fileName = "Account" + to_string(i) + ".bin";
        fstream input(fileName, ios::in | ios::out);

        // If there are fewer than 5 accounts, resume it to the next account
        if (!input.is_open() && i != 6) {
            GamePlay.savedAccount = i;
            break;
        }

        // If there are already 5 accounts, ask the user to
        //  input the account they want to replace

        if (i == 6) {
            int nameAccount = 0;
            bool validNameAccount = false;

            do {
                gotoxy(30, 18);
                cout << "The number of resumed accounts is full! (max is 5 accounts)";
                gotoxy(30, 19);
                cout << "Please enter the account you want to replace (1-->5): ";
                cin >> nameAccount;
                validNameAccount = (nameAccount <= 5) && (nameAccount >= 1) && !cin.fail();

                gotoxy(30, 20);
                if (!validNameAccount) {

                    cin.clear();
                    while (cin.get() != '\n') {
                        continue;
                    }

                    setColor(BLACK, RED);
                    cout << "This account you want to replaced not exist!";
                    resetColor();
                    cout << string(20, ' ');
                }
                else {
                    setColor(BLACK, GREEN);
                    cout << "This name account is valid. Your game will be saved in account " << nameAccount;
                    resetColor();
                }
            } while (!validNameAccount);

            GamePlay.savedAccount = nameAccount;

            break;
        }

        input.close();
    }

    createPlayer(name, GamePlay);
    
    // Release the list of resumed accounts
    releaseList(listAccount);

    // Hide cursor 
    showCursorOnScr(false);
}

void TopPlayers(List& list) {

    system("cls");

    Top20PlayersUI(list);
}


bool Stack::isEmpty() {
    return (topGamePlay == NULL);
}

void Stack::push(GamePlay2048 otherGamePlay) {
    GamePlay2048* NewGamePlay = new GamePlay2048;


    NewGamePlay->TableSize = otherGamePlay.TableSize;

    NewGamePlay->Table = new int* [NewGamePlay->TableSize];
    for (int i = 0; i < NewGamePlay->TableSize; i++) {
        NewGamePlay->Table[i] = new int[NewGamePlay->TableSize];
    }

    for (int i = 0; i < otherGamePlay.TableSize; i++) {
        for (int j = 0; j < otherGamePlay.TableSize; j++) {
            NewGamePlay->Table[i][j] = otherGamePlay.Table[i][j];
        }
    }

    NewGamePlay->player = new Player;

    NewGamePlay->player->score = otherGamePlay.player->score;
    NewGamePlay->isMove = otherGamePlay.isMove;
    NewGamePlay->won = otherGamePlay.won;
    NewGamePlay->isResumeMode = otherGamePlay.isResumeMode;
    NewGamePlay->removeBlock = otherGamePlay.removeBlock;

    NewGamePlay->next = topGamePlay;
    topGamePlay = NewGamePlay;
}

void Stack::pop() {
    if (!isEmpty()) {

        GamePlay2048* current = topGamePlay;
        topGamePlay = current->next;

        delete current->player;

        for (int i = 0; i < current->TableSize; i++) {
            delete[]  current->Table[i];
        }
        delete[] current->Table;

        delete current;

        current = NULL;
    }
}

void Stack::top(GamePlay2048 &otherGamePlay) {
    if (!isEmpty()) {

        for (int i = 0; i < otherGamePlay.TableSize; i++) {
            for (int j = 0; j < otherGamePlay.TableSize; j++) {
                otherGamePlay.Table[i][j] = topGamePlay->Table[i][j];
            }
        }
        
        otherGamePlay.player->score = topGamePlay->player->score;
        otherGamePlay.isMove = topGamePlay->isMove;
        otherGamePlay.won = topGamePlay->won;
        otherGamePlay.isResumeMode = topGamePlay->isResumeMode;
        otherGamePlay.removeBlock = topGamePlay->removeBlock;
    }
}

void Stack::clear() {
    while (!isEmpty()) {
        pop();
    }
    topGamePlay = NULL;
}

void processUndo(GamePlay2048& GamePlay, Stack& UndoStack, Stack& RedoStack) {
    if (GamePlay.isMove) {
        if (UndoStack.isEmpty()) {
            gotoxy(WIDTH_CELL * GamePlay.TableSize + 44, 1);
            setColor(BLACK, RED);
            cout << "You can't UNDO anymore!";
            resetColor();
        }

        if (!UndoStack.isEmpty()) {
            RedoStack.push(GamePlay);
            UndoStack.top(GamePlay);
            UndoStack.pop();

            system("cls");

            // Update time
            GamePlay.player->time.end = clock();
            GamePlay.player->time.duration = int(GamePlay.player->time.end - 
                GamePlay.player->time.start) / CLOCKS_PER_SEC;

            GameUI(GamePlay);
        }
    }
}

void processRedo(GamePlay2048& GamePlay, Stack& RedoStack) {
    if (GamePlay.isMove) {
        if (RedoStack.isEmpty()) {
            gotoxy(WIDTH_CELL * GamePlay.TableSize + 44, 2);
            setColor(BLACK, RED);
            cout << "You can't REDO anymore!";
            resetColor();
        }

        if (!RedoStack.isEmpty()) {
            RedoStack.top(GamePlay);
            RedoStack.pop();

            system("cls");

            // Update time
            GamePlay.player->time.end = clock();
            GamePlay.player->time.duration = int(GamePlay.player->time.end - 
                GamePlay.player->time.start) / CLOCKS_PER_SEC;

            GameUI(GamePlay);
        }
    }
}

void Setting(GamePlay2048& GamePlay, char& choice) {

    // Display current state settings of the game

    system("cls");
    drawLogo();

    gotoxy(32, 15);
    setColor(BLACK, BLUE);
    cout << "RESIZE ";
    resetColor();
    cout << "game table: ";
    setColor(BLACK, BLUE);
    cout << "c ";
    resetColor();
    cout << "(default size is 4)";

    gotoxy(32, 17);
    setColor(BLACK, PURPLE);
    cout << "UNDO ";
    resetColor();
    cout << "mode: ";
    setColor(BLACK, PURPLE);
    cout << "a";
    resetColor();
    cout << "(activate)/";
    setColor(BLACK, PURPLE);
    cout << "d";
    resetColor();
    cout << "(deactivate) (REDO mode also deactive)";

    gotoxy(32, 19);
    setColor(BLACK, PINK);
    cout << "REDO ";
    resetColor();
    cout << "mode: ";
    setColor(BLACK, PINK);
    cout << "A";
    resetColor();
    cout << "(activate)/";
    setColor(BLACK, PINK);
    cout << "D";
    resetColor();
    cout << "(deactivate) (After activate UNDO mode)";

    drawBox({ 5, 25 }, BLACK, GREY, "b: BACK", WIDTH / 2, HEIGHT / 2);

    if (GamePlay.isUndo == false) {
        gotoxy(32, 21);
        cout << "UNDO mode: ";
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();
        gotoxy(52, 21);
        cout << "REDO mode: ";
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();
    }
    else {
        gotoxy(32, 21);
        cout << "UNDO mode: ";
        setColor(BLACK, GREEN);
        cout << "ON ";
        resetColor();

        if (GamePlay.isRedo == false) {
            gotoxy(52, 21);
            cout << "REDO mode: ";
            setColor(BLACK, RED);
            cout << "OFF";
            resetColor();
        }
        else {
            gotoxy(52, 21);
            cout << "REDO mode: ";
            setColor(BLACK, GREEN);
            cout << "ON ";
            resetColor();
        }
        
    }

    gotoxy(32, 24);
    cout << "This size's valid. The size of your game table is " << GamePlay.TableSize << "x" << GamePlay.TableSize;

    // Process player choice
    if (choice == char(ON_UNDO)) {
        GamePlay.isUndo = true;
        gotoxy(32, 21);
        cout << "UNDO mode: ";
        setColor(BLACK, GREEN);
        cout << "ON ";
        resetColor();
    }
    else if (choice == char(ON_RED0)) {
        GamePlay.isRedo = true;
        gotoxy(52, 21);
        cout << "REDO mode: ";
        setColor(BLACK, GREEN);
        cout << "ON ";
        resetColor();
    }
    else if(choice == char(OFF_UNDO)) {
        GamePlay.isUndo = false;
        GamePlay.isRedo = false;

        gotoxy(32, 21);
        cout << "UNDO mode: ";
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();

        gotoxy(52, 21);
        cout << "REDO mode: ";
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();
    }
    else if (choice == char(OFF_REDO)) {
        GamePlay.isRedo = false;

        gotoxy(52, 21);
        cout << "REDO mode: ";
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();
    }

    else if (choice == char(RESIZE)) {

        showCursorOnScr(true);

        int size;
        bool validChoice = false;

        do {
            gotoxy(32, 23);
            cout << "Enter table size (size >= 4 and size <= 10): ";
            cin >> size;

            validChoice = (size >= 4) && (size <= 10) && !cin.fail();

            if (!validChoice) {

                cin.clear();
                while (cin.get() != '\n') {
                    continue;
                }

                setColor(BLACK, RED);
                gotoxy(32, 24);
                cout << "This size isn't valid. Please enter a nummeric value between 4 and 10";
                resetColor();
            }
        } while (!validChoice);

        // Update the game table size
        GamePlay.TableSize = size;

        setColor(BLACK, GREEN);
        gotoxy(32, 24);
        cout << "This size's valid. The size of your game table is " << GamePlay.TableSize << "x" << GamePlay.TableSize
            << string(20, ' ');
        resetColor();
        gotoxy(32, 24);

        showCursorOnScr(false);
    }
}

void Resume(GamePlay2048& GamePlay, List& list, char choice) {
    system("cls");

    // Initialize the interface
    gotoxy(42, 5);
    cout << "PRESS ";
    for (int i = 1, color = 202; i <= 5; i++, color++) {
        setColor(BLACK, color);
        cout << to_string(i) + " ";
        resetColor();
        if (i == 5) continue;
        cout << "| ";
    }
    cout << "TO CHOOSE";

    gotoxy(42, 6);
    cout << "ACCOUNT YOU WANT TO PLAY CONTINUE: ";

    drawBox({ 5, 25 }, BLACK, GREY, "b: BACK", 10, 2);

    for (int i = 1, line = 8; i <= 5; i++, line+=3) {
        string content = "ACCOUNT " + to_string(i);
        drawBox({ 48, line }, BLACK, GREY, content, WIDTH, HEIGHT / 2);
    }

    // Find the number of accounts that have been resumed
    int count = 1;
    while (true) {
        string fileName = "Account" + to_string(count) + ".bin";
        fstream input(fileName, ios::in | ios::out);

        if (count == 6) break;

        else if (!input.is_open()) break;

        count++;
    }

    // Color the accounts that have data
    for (int i = 1, line = 8, color = 202; i < count; i++, line+=3, color++) {
        string content = "ACCOUNT " + to_string(i);
        drawBox({ 48,line }, BLACK, color, content, WIDTH, HEIGHT / 2);
    };

    // Process player choice
    if (choice == '1' && count > 1) {
        loadGameFromFile(GamePlay, "Account1.bin");

        GamePlay.savedAccount = 1;

        system("cls");

        string content = "YOU WILL CONTINUE PLAYING GAME WITH THE STATE OF ACCOUNT 1";
        drawBox({ 30, 8 }, RED, WHITE, content, WIDTH * 3 + 2, HEIGHT * 3 - 1);

    }
    else if (choice == '2' && count > 2) {

        loadGameFromFile(GamePlay, "Account2.bin");

        GamePlay.savedAccount = 2;

        system("cls");

        string content = "YOU WILL CONTINUE PLAYING GAME WITH THE STATE OF ACCOUNT 2";
        drawBox({ 30, 8 }, RED, WHITE, content, WIDTH * 3 + 2, HEIGHT * 3 - 1);
    }

    else if (choice == '3' && count > 3) {
        loadGameFromFile(GamePlay, "Account3.bin");

        GamePlay.savedAccount = 3;

        system("cls");

        string content = "YOU WILL CONTINUE PLAYING GAME WITH THE STATE OF ACCOUNT 3";
        drawBox({ 30, 8 }, RED, WHITE, content, WIDTH * 3 + 2, HEIGHT * 3 - 1);
    }

    else if (choice == '4' && count > 4) {
        loadGameFromFile(GamePlay, "Account4.bin");

        GamePlay.savedAccount = 4;

        system("cls");

        string content = "YOU WILL CONTINUE PLAYING GAME WITH THE STATE OF ACCOUNT 4";
        drawBox({ 30, 8 }, RED, WHITE, content, WIDTH * 3 + 2, HEIGHT * 3 - 1);
    }

    else if (choice == '5' && count > 5) {
        loadGameFromFile(GamePlay, "Account5.bin");

        GamePlay.savedAccount = 5;

        system("cls");

        string content = "YOU WILL CONTINUE PLAYING GAME WITH THE STATE OF ACCOUNT 5";
        drawBox({ 30, 8 }, RED, WHITE, content, WIDTH * 3 + 2, HEIGHT * 3 - 1);
    }

    else {
        return;
    }

    GamePlay.isResumeMode = 1;

    setColor(RED, WHITE);
    gotoxy(55, 15);
    cout << "LOADING...";
    resetColor();

    Sleep(3000);

    system("cls");

    GameUI(GamePlay);

    processGame(GamePlay, list);


}

void AdvancedFunction(GamePlay2048& GamePlay, char& choice) {
    system("cls");
    drawLogo();

    // Print the message and instructions 

    gotoxy(50, 12);
    setColor(BLACK, BLUE);
    cout << "RANDOM COLOR: ";
    resetColor();
    if (!GamePlay.randomColor) {
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();
    }
    else {
        setColor(BLACK, GREEN);
        cout << "ON";
        resetColor();
    }

    gotoxy(25, 14);
    cout << "This mode allows game board to ";
    setColor(BLACK, BLUE);
    cout << "change colors ";
    resetColor();
    cout << "after each move, undo, redo";

    gotoxy(42, 16);
    cout << "OPTIONS: ";
    setColor(BLACK, BLUE);
    cout << "q";
    resetColor();
    cout << "(activate) / ";
    setColor(BLACK, BLUE);
    cout << "Q";
    resetColor();
    cout << "(deactivate)";

    gotoxy(50, 19);
    setColor(BLACK, ORANGE);
    cout << "REMOVE BLOCK: ";
    resetColor();
    if (!GamePlay.removeBlock) {
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();
    }

    else {
        setColor(BLACK, GREEN);
        cout << "ON";
        resetColor();
    }
    gotoxy(28, 21);
    cout << "This function allows you to ";
    setColor(BLACK, ORANGE);
    cout << "remove ";
    resetColor();
    cout << "any tile from the game board.";

    gotoxy(28, 22);
    cout << "However, each removal will ";
    setColor(BLACK, ORANGE);
    cout << "deduct 100 points ";
    resetColor();
    cout << "from your score.";

    gotoxy(28, 23);
    cout << "If your score's ";
    setColor(BLACK, ORANGE);
    cout << "below 100";
    resetColor();
    cout << ", you won't be able to use this function!";

    gotoxy(42, 25);
    cout << "OPTIONS: ";
    setColor(BLACK, ORANGE);
    cout << "z";
    resetColor();
    cout << "(activate) / ";
    setColor(BLACK, ORANGE);
    cout << "Z";
    resetColor();
    cout << "(deactivate)";

    drawBox({ 5, 25 }, BLACK, GREY, "b: BACK", WIDTH / 2, HEIGHT / 2);

    // Process player's choice

    if (choice == char(ON_RANDOMCOLOR)) {
        GamePlay.randomColor = true;
        gotoxy(50, 12);
        setColor(BLACK, BLUE);
        cout << "RANDOM COLOR: ";
        resetColor();
        setColor(BLACK, GREEN);
        cout << "ON ";
        resetColor();
    }
    else if (choice == char(OFF_RANDOMCOLOR)) {
        GamePlay.randomColor = false;
        gotoxy(50, 12);
        setColor(BLACK, BLUE);
        cout << "RANDOM COLOR: ";
        resetColor();
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();
    }
    else if (choice == char(ON_REMOVEBLOCK)) {
        GamePlay.removeBlock = true;
        gotoxy(50, 19);
        setColor(BLACK, ORANGE);
        cout << "REMOVE BLOCK: ";
        resetColor();
        setColor(BLACK, GREEN);
        cout << "ON ";
        resetColor();
    }
    else if (choice == char(OFF_REMOVEBLOCK)) {
        GamePlay.removeBlock = false;
        gotoxy(50, 19);
        setColor(BLACK, ORANGE);
        cout << "REMOVE BLOCK: ";
        resetColor();
        setColor(BLACK, RED);
        cout << "OFF";
        resetColor();
    }
}

void processRemoveBlock(GamePlay2048& GamePlay, Stack& UndoStack, List& list) {
    if (GamePlay.isMove) {
        if (GamePlay.player->score < 100) {
            gotoxy(WIDTH_CELL * GamePlay.TableSize + WIDTH * 2 + 4, 3);
            setColor(BLACK, RED);
            cout << "Can't use REMOVE BLOCK";
            resetColor();
        }

        else {
            setColor(BLACK, ORANGE);
            gotoxy(0, HEIGHT_CELL * GamePlay.TableSize + 4);
            cout << "Enter tile position to remove: ";

            Point A;
            bool validPos = false;

            showCursorOnScr(true);

            // Handle the operation when the player inputs the coordinate

            do {
                gotoxy(0, HEIGHT_CELL * GamePlay.TableSize + 5);
                cout << "row (0 <= row <= " << GamePlay.TableSize - 1 << "): ";
                cin >> A.x;
                validPos = (A.x >= 0) && (A.x <= GamePlay.TableSize - 1) && !cin.fail();

                gotoxy(0, HEIGHT_CELL * GamePlay.TableSize + 6);
                if (!validPos) {

                    cin.clear();
                    while (cin.get() != '\n') {
                        continue;
                    }

                    setColor(BLACK, RED);
                    cout << "Position invalid!";
                    setColor(BLACK, ORANGE);

                }
                else {
                    setColor(BLACK, GREEN);
                    cout << "Postition valid!  ";
                    setColor(BLACK, ORANGE);
                }

            } while (!validPos);

            do {
                gotoxy(0, HEIGHT_CELL * GamePlay.TableSize + 7);
                cout << "colum (0 <= colum <= " << GamePlay.TableSize - 1 << "): ";
                cin >> A.y;
                validPos = (A.y >= 0) && (A.y <= GamePlay.TableSize - 1) && !cin.fail();

                gotoxy(0, HEIGHT_CELL * GamePlay.TableSize + 8);
                if (!validPos) {

                    cin.clear();
                    while (cin.get() != '\n') {
                        continue;
                    }

                    setColor(BLACK, RED);
                    cout << "Position invalid!";
                    setColor(BLACK, ORANGE);

                }
                else {
                    setColor(BLACK, GREEN);
                    cout << "Postition valid!  ";
                    setColor(BLACK, ORANGE);
                }

            } while (!validPos);


            if (GamePlay.isUndo) {
                UndoStack.push(GamePlay);
            }

            GamePlay.Table[A.x][A.y] = 0;
            GamePlay.player->score -= 100;

            resetColor();
            system("cls");

            // Update time
            GamePlay.player->time.end = clock();
            GamePlay.player->time.duration = int(GamePlay.player->time.end - 
                GamePlay.player->time.start) / CLOCKS_PER_SEC;

            GameUI(GamePlay);
        }

        showCursorOnScr(false);
    }
}

void releaseList(List& list) {
    if (list.head == NULL) return;

    Player* current = list.head;
    Player* next = NULL;

    while (current != NULL) {
        next = current->next;

        delete current;
        current = next;
    }

    list.head = NULL;
    list.size = 0;
}

void processNewGame(GamePlay2048& GamePlay, Stack& UndoStack, Stack& RedoStack) {
   
    // Delete the data from the previous game
    deleteMemories(GamePlay.Table, GamePlay.TableSize);
    UndoStack.clear();
    RedoStack.clear();
    system("cls");

    // Ask the player if they want to exit to the Start menu
    setColor(BLACK, YELLOW);
    drawLogo();
    resetColor();
    gotoxy(18, 15);
    drawBox({ 30, 14 }, GREY, RED, "", WIDTH * 3, HEIGHT * 3);
    gotoxy(37, 18);
    setColor(GREY, RED);
    cout << "DO YOU WANT TO GO BACK TO THE START DISPLAY TO";
    gotoxy(37, 19);
    cout << "RE-ENTER YOUR NAME OR DO ANOTHER FUNCTIONS?";
    resetColor();

    drawBox({ 47,21 }, GREY, RED, "Y : YES", WIDTH / 2, HEIGHT / 2);
    drawBox({ 62,21 }, GREY, RED, "N : NO", WIDTH / 2, HEIGHT / 2);
}

void processExit(GamePlay2048& GamePlay) {
    system("cls");
    GamePlay.isMove = 0;

    string content = "DO YOU REALLY WANT TO EXIT THE GAME?";
    drawBox({ 30, 8 }, RED, WHITE, content, WIDTH * 3, HEIGHT * 3);
    drawBox({ 47, 15 }, RED, WHITE, "Y : YES", WIDTH / 2, HEIGHT / 2);
    drawBox({ 62, 15 }, RED, WHITE, "N : NO", WIDTH / 2, HEIGHT / 2);
}

void processNo(GamePlay2048& GamePlay) {
    GamePlay.isMove = 1;

    system("cls");
    GameUI(GamePlay);
}

void processYes(GamePlay2048& GamePlay, List& list, Stack& UndoStack, Stack& RedoStack) {

    GamePlay.isMove = 1;
    GamePlay.player->time.end = clock();
    GamePlay.player->time.duration = int(GamePlay.player->time.end - GamePlay.player->time.start) / CLOCKS_PER_SEC;

    // Update the list with the new player
    updateList(list, GamePlay.player);
    writeListToFileBin(list, "player.bin");

    // Save the game state
    string fileName = "Account" + to_string(GamePlay.savedAccount) + ".bin";
    saveGameToFile(GamePlay, fileName);

    // Save the Undo and Redo stack if available 

    if (GamePlay.isUndo) {
        string fileNameUndo = "Undo" + to_string(GamePlay.savedAccount) + ".bin";
        saveStackToFile(UndoStack, fileNameUndo);
    }

    if (GamePlay.isRedo) {
        string fileNameRedo = "Redo" + to_string(GamePlay.savedAccount) + ".bin";
        saveStackToFile(RedoStack, fileNameRedo);
    }

    // Update the list of resumed accounts

    List listAccount;

    readPlayerFromFileBin(listAccount, "listAccount.bin");
    updateListAccount(listAccount, GamePlay.player, GamePlay.savedAccount);
    writeListToFileBin(listAccount, "listAccount.bin");

    releaseList(listAccount);

    // Release the list
    releaseList(list);

    delete GamePlay.player;

    deleteMemories(GamePlay.Table, GamePlay.TableSize);

    UndoStack.clear();
    RedoStack.clear();

    system("cls");

    playGame();
}

void GiamDiem(List& list) {
    Player* current = list.head;

    while (current->next != NULL) {

        //if (current->next == NULL || current->next->next == NULL) {
        //    current->score -= 100;
        //}

        current = current->next;
    }

    //current->name = "Tom";
    current->score -= 100;
}
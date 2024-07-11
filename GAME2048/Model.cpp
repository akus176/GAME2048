#include "Model.h"
#include "Graphics.h"
#include <iostream>

using namespace std;

void initializeGame(GamePlay2048& GamePlay) {

    srand(time(NULL));

    GamePlay.Table = new int* [GamePlay.TableSize];

    for (int i = 0; i < GamePlay.TableSize; i++) {
        GamePlay.Table[i] = new int[GamePlay.TableSize];
    }

    for (int i = 0; i < GamePlay.TableSize; i++) {
        for (int j = 0; j < GamePlay.TableSize; j++) {
            GamePlay.Table[i][j] = 0;
        }
    }

    int row = 0, col = 0, count = 0;

    while (count != 2) {

        row = rand() % GamePlay.TableSize;
        col = rand() % GamePlay.TableSize;

        if (GamePlay.Table[row][col] == 0) {

            // Generate randomly with a distribution of 70% for 2 and 30% for 4
            if ((rand() % 10) < 7) {
                GamePlay.Table[row][col] = 2;
            }
            else {
                GamePlay.Table[row][col] = 4;
            }
            count++;
        }
    }

    // Adjust the flags that manage the game state

    GamePlay.player->score = 0;

    GamePlay.isMove = true;

    GamePlay.isChange = true;

    GamePlay.won = false;

    GamePlay.isResumeMode = false;
}

void NewGame(GamePlay2048& GamePlay, List& list) {

    resetColor();

    initializeGame(GamePlay);

    GamePlay.player->time.start = clock();

    system("cls");

    GameUI(GamePlay);

    processGame(GamePlay, list);
}

void addToTable(GamePlay2048& GamePlay) {
    int row, col, count = 0;

    while (count == 0) {
        row = rand() % GamePlay.TableSize;
        col = rand() % GamePlay.TableSize;

        // Generate randomly with a distribution of 70% for 2 and 30% for 4
        if (GamePlay.Table[row][col] == 0) {
            if ((rand() % 100) < 7) {
                GamePlay.Table[row][col] = 2;
            }
            else GamePlay.Table[row][col] = 4;

            count++;
        }
    }

    if (count == 1) {
        system("cls");
        GameUI(GamePlay);
    }
}

void createPlayer(string name, GamePlay2048& GamePlay) {
    GamePlay.player = new Player;

    GamePlay.player->name = name;
    GamePlay.player->score = 0;
    GamePlay.player->next = NULL;
}

void reduceName(string& name) {
    if (name == "") return;

    // Remove excess spaces at the beginning and the end of string
    int first = name.find_first_not_of(' ');
    int last = name.find_last_not_of(' ');

    if (first == string::npos) {
        name = "";
        return;
    }

    name = name.substr(first, last - first + 1);

    // Remove excess whitespace between words
    int pos = 0;
    while (pos = name.find("  ", pos)) {
        if (pos == string::npos)
            break;
        else
            name.erase(pos, 1);
    }
}

int findBestScore(List list) {
    if (list.head == NULL) return EXIT_FAILURE;

    int maxScore = 0;
    for (Player* current = list.head; current != NULL; current = current->next) {
        if (current->score > maxScore)
            maxScore = current->score;
    }
    return maxScore;
}

int findPlayerRank(List list, string name) {
    if (list.head == NULL) return 0;

    int count = 1;

    Player* current = list.head;
    while (current != NULL) {
        if (current->name == name) {
            return count;
        }
        count++;
        current = current->next;
    }

    return 0;
}

string findContent(Point current, const Point initial) {
    if (current.x == initial.x) {
        if (current.y == initial.y) {
            return "NEW GAME";
        }
        else {
            return "ABOUT";
        }
    }
    else if (current.x == initial.x + WIDTH + 15) {
        if (current.y == initial.y) {
            return "TOP PLAYERS";
        }
        else {
            return "SETTINGS";
        }
    }
    else {
        if (current.y == initial.y) {
            return "RESUME";
        }
        else {
            return "ADVANCED FUNCTION";
        }
    }
}

int findTextColor(Point current, const Point initial) {
    if (current.x == initial.x) {
        if (current.y == initial.y) {
            return BLUE;
        }
        else {
            return YELLOW;
        }
    }
    else if (current.x == initial.x + WIDTH + 15) {
        if (current.y == initial.y) {
            return GREEN;
        }
        else {
            return RED;
        }
    }
    else {
        if (current.y == initial.y) {
            return PINK;
        }
        else {
            return ORANGE;
        }
    }
}

bool checkNameExist(string name, List list) {
    if (list.head == NULL) return 0;

    for (Player* current = list.head; current != NULL; current = current->next) {
        if (current->name == name) return 1;
    }
    return 0;
}

void sortByScoreAndTime(List& list) {
    if (list.head == NULL || list.head->next == NULL) return;

    for (Player* curr1 = list.head; curr1->next != NULL; curr1 = curr1->next) {
        Player* max = curr1;
        for (Player* curr2 = curr1->next; curr2 != NULL; curr2 = curr2->next) {
            if (curr2->score > max->score)
                max = curr2;
            else if (curr2->score == max->score) {
                if (curr2->time.duration < max->time.duration) {
                    max = curr2;
                }
            }
        }

        if (max == curr1) continue;

        swap(max->name, curr1->name);
        swap(max->score, curr1->score);
        swap(max->time, curr1->time);
    }
}

void deleteMemories(int** Matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[] Matrix[i];
    }
    delete[] Matrix;
}

void writeListToFileBin(List list, string fileName) {
    if (list.head == NULL) return;

    fstream output;
    output.open(fileName, ios::out | ios::binary);

    if (!output.is_open()) {
        cout << "Failed to open " << fileName;
    }

    output.write((char*)&list.size, sizeof(int));

    Player* current = list.head;

    while (current != NULL) {
        char* buffer = new char[21];

        for (int i = 0; i < current->name.size(); i++) {
            buffer[i] = current->name[i];
        }

        buffer[current->name.size()] = '\0';

        output.write(buffer, 21 * sizeof(char));

        output.write((char*)&current->score, sizeof(int));

        output.write((char*)&current->time.duration, sizeof(int));

        delete[] buffer;

        current = current->next;
    }

    output.close();
}

void addTailList(List& list, Player* player) {
    if (list.head == NULL)
        list.head = player;
    else {
        Player* current = list.head;
        while (current->next != NULL) {
            current = current->next;
        }

        current->next = player;
    }
}

void readPlayerFromFileBin(List& list, string fileName) {
    fstream input;
    input.open(fileName, ios::in | ios::binary);

    if (!input.is_open()) {
        return;
    }

    input.read((char*)&list.size, sizeof(int));

    for (int i = 0; i < list.size; i++) {
        Player* player = createPlayerFromFileBin(input);
        addTailList(list, player);
    }

    input.close();
}

Player* createPlayerFromFileBin(fstream& input) {
    Player* player = new Player;

    char* buffer = new char[21];

    input.read(buffer, 21 * sizeof(char));

    player->name = string(buffer);

    input.read((char*)&player->score, sizeof(int));

    input.read((char*)&player->time.duration, sizeof(int));

    delete[] buffer;

    return player;
}

void updateList(List& list, Player* newPlayer) {
    Player* copyPlayer = new Player;

    copyPlayer->name = newPlayer->name;
    copyPlayer->score = newPlayer->score;
    copyPlayer->time.duration = newPlayer->time.duration;

    if (list.head == NULL || list.size == 0) {
        list.head = copyPlayer;
        list.size++;

        return;
    }

    if (checkNameExist(newPlayer->name, list)) {
        Player* current = list.head;
        while (current != NULL) {
            if (current->name == newPlayer->name) {
                current->score = newPlayer->score;
                current->time = newPlayer->time;
                break;
            }
            current = current->next;
        }
        delete copyPlayer;

        sortByScoreAndTime(list);
        return;
    }

    // Add tail

    Player* current = list.head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = copyPlayer;
    list.size++;

    // Sort list
    sortByScoreAndTime(list);

    if (list.size == 21) {
        Player* current2 = list.head;
        while (current2->next->next != NULL) {
            current2 = current2->next;
        }

        Player* temp = current2->next;
        current2->next = NULL;

        delete temp;
        temp = NULL;
        list.size--;
    }
}

void updateListAccount(List& listAccount, Player* account, int pos) {
    // If the list already contains 5 accounts
    if (listAccount.size == 5) {
        Player* current = listAccount.head;
        int count = 1; 

        while (current != NULL && count< pos) {
            current = current->next;
            count++;
        }

        if (current != NULL && count == pos) {

            // update information
            current->name = account->name;
            current->score = account->score;
            current->time.duration = account->time.duration;
        }
        return;
    }

    // Create a copy 
    Player* copyAccount = new Player;

    copyAccount->name = account->name;
    copyAccount->score = account->score;
    copyAccount->time.duration = account->time.duration;
    copyAccount->next = NULL;

    // Add a copy tail of the list
    if (listAccount.head == NULL || listAccount.size == 0) {
        listAccount.head = copyAccount;
        listAccount.size++;
    }
    else if (listAccount.size < 5) {
        Player* current = listAccount.head;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = copyAccount;
        listAccount.size++;
    }
}

void saveGameToFile(GamePlay2048 GamePlay, string fileName) {
    fstream output;

    output.open(fileName, ios::out | ios::binary);

    if (!output.is_open()) {
        cout << "Failed to open " << fileName;
        return;
    }

    output.write((char*)&GamePlay.TableSize, sizeof(int));

    for (int i = 0; i < GamePlay.TableSize; i++) {
        for (int j = 0; j < GamePlay.TableSize; j++) {
            output.write((char*)&GamePlay.Table[i][j], sizeof(int));
        }
    }

    char* buffer = new char[21];

    for (int i = 0; i < GamePlay.player->name.size(); i++) {
        buffer[i] = GamePlay.player->name[i];
    }

    buffer[GamePlay.player->name.size()] = '\0';

    output.write(buffer, 21 * sizeof(char));

    delete[] buffer;

    output.write((char*)&GamePlay.player->score, sizeof(int));

    output.write((char*)&GamePlay.bestScore, sizeof(int));

    output.write((char*)&GamePlay.player->time.duration, sizeof(int));

    output.write((char*)&GamePlay.isMove, sizeof(bool));
    output.write((char*)&GamePlay.isChange, sizeof(bool));
    output.write((char*)&GamePlay.isUndo, sizeof(bool));
    output.write((char*)&GamePlay.isRedo, sizeof(bool));
    output.write((char*)&GamePlay.won, sizeof(bool));
    output.write((char*)&GamePlay.randomColor, sizeof(bool));
    output.write((char*)&GamePlay.removeBlock, sizeof(bool));

    output.close();
}

void loadGameFromFile(GamePlay2048& GamePlay, string fileName) {
    fstream input;
    input.open(fileName, ios::in | ios::binary);

    if (!input.is_open()) {
        cout << "Failed to open " << fileName;
        return;
    }

    input.read((char*)&GamePlay.TableSize, sizeof(int));

    GamePlay.Table = new int* [GamePlay.TableSize];

    for (int i = 0; i < GamePlay.TableSize; i++) {
        GamePlay.Table[i] = new int[GamePlay.TableSize];
    }

    for (int i = 0; i < GamePlay.TableSize; i++) {
        for (int j = 0; j < GamePlay.TableSize; j++) {
            input.read((char*)&GamePlay.Table[i][j], sizeof(int));
        }
    }

    GamePlay.player = new Player;

    char* buffer = new char[21];

    input.read(buffer, 21 * sizeof(char));

    GamePlay.player->name = string(buffer);

    delete[] buffer;

    input.read((char*)&GamePlay.player->score, sizeof(int));
    input.read((char*)&GamePlay.bestScore, sizeof(int));
    input.read((char*)&GamePlay.player->time.duration, sizeof(int));

    // Include the total play time from before
    GamePlay.player->time.start -= GamePlay.player->time.duration;

    input.read((char*)&GamePlay.isMove, sizeof(bool));
    input.read((char*)&GamePlay.isChange, sizeof(bool));
    input.read((char*)&GamePlay.isUndo, sizeof(bool));
    input.read((char*)&GamePlay.isRedo, sizeof(bool));
    input.read((char*)&GamePlay.won, sizeof(bool));
    input.read((char*)&GamePlay.randomColor, sizeof(bool));
    input.read((char*)&GamePlay.removeBlock, sizeof(bool));

    input.close();
}

void saveStackToFile(Stack stack, string fileName) {
    if (stack.isEmpty()) return;

    fstream output;
    output.open(fileName, ios::out | ios::binary);


    if (!output.is_open()) {
        cout << "Failed to open " << fileName;
        return;
    }

    GamePlay2048* current = stack.topGamePlay;

    while (current != NULL) {
        output.write((char*)&current->TableSize, sizeof(int));

        for (int i = 0; i < current->TableSize; i++) {
            for (int j = 0; j < current->TableSize; j++) {
                output.write((char*)&current->Table[i][j], sizeof(int));
            }
        }

        output.write((char*)&current->player->score, sizeof(int));
        output.write((char*)&current->isMove, sizeof(bool));
        output.write((char*)&current->won, sizeof(bool));
        output.write((char*)&current->isResumeMode, sizeof(bool));
        output.write((char*)&current->removeBlock, sizeof(bool));

        current = current->next;
    }

    output.close();
}

GamePlay2048* createGamePlayFromFile(fstream& input) {
    GamePlay2048* newGamePlay = new GamePlay2048;

    input.read((char*)&newGamePlay->TableSize, sizeof(int));

    newGamePlay->Table = new int* [newGamePlay->TableSize];
    for (int i = 0; i < newGamePlay->TableSize; i++) {
        newGamePlay->Table[i] = new int[newGamePlay->TableSize];
    }

    for (int i = 0; i < newGamePlay->TableSize; i++) {
        for (int j = 0; j < newGamePlay->TableSize; j++) {
            input.read((char*)&newGamePlay->Table[i][j], sizeof(int));
        }
    }

    newGamePlay->player = new Player;

    input.read((char*)&newGamePlay->player->score, sizeof(int));
    input.read((char*)&newGamePlay->isMove, sizeof(bool));
    input.read((char*)&newGamePlay->won, sizeof(bool));
    input.read((char*)&newGamePlay->isResumeMode, sizeof(bool));
    input.read((char*)&newGamePlay->removeBlock, sizeof(bool));

    newGamePlay->next = NULL;

    return newGamePlay;
}

void addTailStack(Stack& stack, GamePlay2048* GamePlay) {
    if (stack.isEmpty()) {
        stack.topGamePlay = GamePlay;
        return;
    }

    GamePlay2048* current = stack.topGamePlay;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = GamePlay;
}

void readStackFromFile(Stack& stack, string fileName) {
    fstream input;
    input.open(fileName, ios::in | ios::binary);

    if (!input.is_open()) {
        return;
    }

    while (input.peek() != EOF) {
        GamePlay2048* newGamePlay = createGamePlayFromFile(input);
        addTailStack(stack, newGamePlay);
    }

    input.close();
}

void playGame() {
    GamePlay2048 GamePlay;

    List list;

    // Hide the cursor in the console screen
    showCursorOnScr(false);

    // Initialize coordinates used as fixed 
    // markers when drawing in GameStartUI
    const Point initial = { 16,15 };

    // Initialize coordinates to mark the current
    //  position when drawing in GameStartUI
    Point current = { initial.x, initial.y };

    // Load from file to list player
    readPlayerFromFileBin(list, "player.bin");

    //GiamDiem(list);

    //writeListToFileBin(list, "player.bin");

    //return;

    // Find best score
    GamePlay.bestScore = findBestScore(list);

    //GamePlay.bestScore = 50;

    // Print the starting interface
    GameStartUI(initial);

    // Process actions in the starting interface
    processGameStart(current, initial, GamePlay, list);

}
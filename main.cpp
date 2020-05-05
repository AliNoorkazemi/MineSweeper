#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Table {
public:
    int row, column;
    char **table;
public:
    Table(int row, int column) : row(row), column(column) {
        table = new char *[row];
        for (int i = 0; i < row; i++)
            table[i] = new char[column];
    }

    void showTable();
};

class GameTable : public Table {

    int bombs;
public:
    GameTable(int row, int column, int bombs) : Table(row, column), bombs(bombs) {}

    void putBombs(int, int);

    void putNumbers();

    void menu();
};

class Player : public Table {

public:
    int flags;
    time_t startTime;
    int endTime;

    Player(int row, int column) : Table(row, column) {
        flags = 0;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < column; j++)
                table[i][j] = '.';
    }

    void clickElement(int, int, GameTable);

    void putAndPickUpFlag(int, int, int);

    void putAndPickUpQuestionMark(int, int);

    bool gameOver(int, int);

    bool win(GameTable);

    int getTime();

    void start();

    void getRecords();

private:
    void saveRecord();

    void reveal(int, int, GameTable);

    void saveToTopTen();
};

int main() {
    int n, m;//n is the number of rows and m is the number of column
    cin >> n >> m;
    int bombs;
    while (true) {
        cin >> bombs;
        if (bombs < n * m)
            break;
        cout << "Invalid" << endl;
    }
    Player player(n, m);
    GameTable gameTable(n, m, bombs);
    int xPosition, yPosition;
    player.showTable();
    cin >> xPosition;
    cin >> yPosition;
    gameTable.putBombs(xPosition, yPosition);
    gameTable.putNumbers();
    player.clickElement(xPosition, yPosition, gameTable);
    player.start();
    do {
        gameTable.menu();
        int input;
        cin >> input;
        switch (input) {
            case 1:
                player.showTable();
                break;
            case 2:
                cin >> xPosition;
                cin >> yPosition;
                player.clickElement(xPosition, yPosition, gameTable);
                break;
            case 3:
                cin >> xPosition;
                cin >> yPosition;
                player.putAndPickUpFlag(xPosition, yPosition, bombs);
                break;
            case 4:
                cin >> xPosition;
                cin >> yPosition;
                player.putAndPickUpQuestionMark(xPosition, yPosition);
                break;
            case 5:
                cout << bombs << endl;
                break;
            case 6:
                cout << bombs - player.flags << endl;
                break;
            case 7:
                cout << player.getTime() << endl;
                break;
            case 8:
                player.getRecords();
                break;
        }
        if (player.gameOver(xPosition, yPosition) || player.win(gameTable))
            break;
    } while (true);

    return 0;
}

void Table::showTable() {
    cout << " ";
    for (int i = 1; i <= this->column; i++) {
        if (i / 10 > 0)
            cout << " " << i;
        else
            cout << "  " << i;
    }
    cout << endl;
    for (int i = 0; i < this->row; i++) {
        if ((i + 1) / 10 > 0)
            cout << i + 1 << " ";
        else
            cout << i + 1 << "  ";
        for (int j = 0; j < this->column; j++)
            cout << this->table[i][j] << "  ";
        cout << endl;
    }
}

void GameTable::putBombs(int xPosition, int yPosition) {
    srand(time(NULL));
    int xRandom;
    int yRandom;
    for (int i = 0; i < bombs; i++) {
        while (true) {
            xRandom = rand() % row;
            yRandom = rand() % column;
            if (this->table[xRandom][yRandom] != 'O' && ((xPosition - 1) != xRandom || (yPosition - 1) != yRandom))
                break;
        }
        this->table[xRandom][yRandom] = 'O';
    }
}

void GameTable::putNumbers() {
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->column; j++) {
            if (this->table[i][j] == 'O')
                continue;
            int bombsForEachElement = 0;
            if (i > 0 && j > 0)
                if (this->table[i - 1][j - 1] == 'O')
                    ++bombsForEachElement;
            if (i > 0)
                if (this->table[i - 1][j] == 'O')
                    ++bombsForEachElement;
            if (j > 0)
                if (this->table[i][j - 1] == 'O')
                    ++bombsForEachElement;
            if (i + 1 < this->row && j + 1 < this->column)
                if (this->table[i + 1][j + 1] == 'O')
                    ++bombsForEachElement;
            if (j + 1 < this->column)
                if (this->table[i][j + 1] == 'O')
                    ++bombsForEachElement;
            if (i + 1 < this->row && j > 0)
                if (this->table[i + 1][j - 1] == 'O')
                    ++bombsForEachElement;
            if (j + 1 < this->column && i > 0)
                if (this->table[i - 1][j + 1] == 'O')
                    ++bombsForEachElement;
            if (i + 1 < this->row)
                if (this->table[i + 1][j] == 'O')
                    ++bombsForEachElement;
            this->table[i][j] = bombsForEachElement + 48;
        }
    }
}

void GameTable::menu() {
    cout
            << "1.Print  2.Click Element  3.Flag  4.Question Mark  5.The Number Of Bombs  6.Undiscovered Bombs  7.Time  8.Get Records\n";
}

bool Player::gameOver(int xPosition, int yPosition) {
    if (this->table[xPosition - 1][yPosition - 1] == 'O') {
        cout << "    GAME OVER !!!!!";
        return true;
    } else
        return false;
}

void Player::clickElement(int xPosition, int yPosition, GameTable gameTable) {
    if (this->table[xPosition - 1][yPosition - 1] != '.') {
        cout << "Invalid" << endl;
        return;
    }
    this->reveal(xPosition - 1, yPosition - 1, gameTable);
}

void Player::putAndPickUpFlag(int xPosition, int yPosition, int bombs) {
    if (this->table[xPosition - 1][yPosition - 1] == '.' && this->flags < bombs) {
        this->table[xPosition - 1][yPosition - 1] = '<';
        this->flags++;
    } else if (this->table[xPosition - 1][yPosition - 1] == '<') {
        this->table[xPosition - 1][yPosition - 1] = '.';
        this->flags--;
    } else
        cout << "Invalid" << endl;
}

void Player::putAndPickUpQuestionMark(int xPosition, int yPosition) {
    if (this->table[xPosition - 1][yPosition - 1] == '.') {
        this->table[xPosition - 1][yPosition - 1] = '?';
    } else if (this->table[xPosition - 1][yPosition - 1] == '?') {
        this->table[xPosition - 1][yPosition - 1] = '.';
    } else
        cout << "Invalid" << endl;
}

bool Player::win(GameTable gameTable) {
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->column; j++) {
            if (gameTable.table[i][j] == 'O')
                continue;
            if (this->table[i][j] != gameTable.table[i][j]) {
                return false;
            }
        }
    }
    time_t time1 = std::time(0);
    this->endTime = time1 - this->startTime;
    this->saveRecord();
    this->saveToTopTen();
    cout << "    YOU WIN !!!!!!";
    return true;
}

void Player::reveal(int i, int j, GameTable gameTable) {
    if (i >= 0 && i < gameTable.row && j >= 0 && j < gameTable.column && this->table[i][j] == '.') {
        this->table[i][j] = gameTable.table[i][j];

        if (gameTable.table[i][j] == '0') {
            reveal(i, j - 1, gameTable);
            reveal(i, j + 1, gameTable);
            reveal(i - 1, j - 1, gameTable);
            reveal(i + 1, j - 1, gameTable);
            reveal(i + 1, j + 1, gameTable);
            reveal(i - 1, j + 1, gameTable);
            reveal(i - 1, j, gameTable);
            reveal(i + 1, j, gameTable);
        }
    }
}

int Player::getTime() {
    time_t now = std::time(0);
    int second = now - this->startTime;
    return second;
}

void Player::start() {
    this->startTime = time(0);
}

void Player::saveRecord() {
    ifstream reader("file.txt", ios::in);
    if (!reader.is_open()) {
        cout << "Can not open file.txt!" << endl;
        return;
    }
    int record;
    reader >> record;
    reader.close();
    ofstream writer;
    if (record > this->endTime) {
        writer.open("file.txt", ios::out);
        writer << this->endTime << endl;
    }
    writer.close();
}

void Player::getRecords() {
    ifstream reader("records.txt", ios::in);
    if (!reader.is_open()) {
        cout << "Can not open records.txt!" << endl;
        return;
    }
    string userName;
    int time_record;
    while (reader >> userName >> time_record) {
        cout << userName << " : " << time_record << endl;
    }
    reader.close();
}

void Player::saveToTopTen() {
    ifstream reader("records.txt", ios::in);
    if (!reader.is_open()) {
        cout << "Can not open records.txt!" << endl;
        return;
    }
    vector<pair<int, string>> vector;
    int time_record;
    string name;
    while (reader >> name >> time_record) {
        vector.push_back(make_pair(time_record, name));
    }
    reader.close();
    sort(vector.begin(), vector.end());
    if (this->endTime > (vector.end()-1)->first)
        return;
    cout << "You hit the new record\nPlease write your name:" << endl;
    cin >> name;
    vector.push_back(make_pair(this->endTime, name));
    sort(vector.begin(), vector.end());
    auto iterator = vector.begin();
    ofstream writer("records.txt", ios::out);
    for (; iterator != vector.end() - 1; iterator++) {
        writer << iterator->second << endl << iterator->first << endl;
    }
    writer.close();
}
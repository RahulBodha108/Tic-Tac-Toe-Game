// Tic-Tac-Toe with a threaded win/draw check.
// Build: g++ -std=c++11 -pthread tictactoe.cpp -o tictactoe

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <mutex>
using namespace std;

const int ROWS = 3;
const int COLS = 3;
const string EMPTY = " ";

void runGame();
void initializeGameBoard(string gameBoard[ROWS][COLS]);
void printCurrentBoard(string gameBoard[ROWS][COLS]);
void getUserInput(bool xTurn, string gameBoard[ROWS][COLS]);
string getWinner(string gameBoard[ROWS][COLS]);
bool isBoardFull(string gameBoard[ROWS][COLS]);

// Workers used by getWinner()
void checkRows(string gameBoard[ROWS][COLS], string &result, mutex &m);
void checkCols(string gameBoard[ROWS][COLS], string &result, mutex &m);
void checkDiagonals(string gameBoard[ROWS][COLS], string &result, mutex &m);

int main()
{
    runGame();
    return 0;
}

void runGame()
{
    string gameBoard[ROWS][COLS];
    initializeGameBoard(gameBoard);

    // Coin flip decides who starts.
    srand(static_cast<unsigned>(time(0)));
    bool xTurn = (rand() % 2 == 0);
    cout << "Tic-Tac-Toe\n";
    cout << (xTurn ? "X" : "O") << " goes first.\n";

    string winner = EMPTY;
    bool full = false;

    while (true)
    {
        printCurrentBoard(gameBoard);
        getUserInput(xTurn, gameBoard);

        // Run the two end-of-game checks side by side.
        thread winnerThread([&]() { winner = getWinner(gameBoard); });
        thread fullThread([&]() { full = isBoardFull(gameBoard); });
        winnerThread.join();
        fullThread.join();

        if (winner != EMPTY)
        {
            printCurrentBoard(gameBoard);
            cout << "Player " << winner << " wins!\n";
            break;
        }
        if (full)
        {
            printCurrentBoard(gameBoard);
            cout << "It's a draw.\n";
            break;
        }

        xTurn = !xTurn;
    }
}

void initializeGameBoard(string gameBoard[ROWS][COLS])
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            gameBoard[r][c] = EMPTY;
}

void printCurrentBoard(string gameBoard[ROWS][COLS])
{
    cout << "\n    1   2   3\n";
    for (int r = 0; r < ROWS; r++)
    {
        cout << r + 1 << " ";
        for (int c = 0; c < COLS; c++)
        {
            cout << " " << gameBoard[r][c] << " ";
            if (c < COLS - 1) cout << "|";
        }
        cout << "\n";
        if (r < ROWS - 1) cout << "  ---+---+---\n";
    }
    cout << "\n";
}

void getUserInput(bool xTurn, string gameBoard[ROWS][COLS])
{
    string mark = xTurn ? "X" : "O";
    int row = 0, col = 0;

    while (true)
    {
        cout << "Player " << mark << " - enter row and column (1-3 1-3): ";
        if (!(cin >> row >> col))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Numbers only, please.\n";
            continue;
        }
        if (row < 1 || row > ROWS || col < 1 || col > COLS)
        {
            cout << "Out of range. Use values from 1 to 3.\n";
            continue;
        }
        if (gameBoard[row - 1][col - 1] != EMPTY)
        {
            cout << "That square is taken.\n";
            continue;
        }
        break;
    }

    gameBoard[row - 1][col - 1] = mark;
}

void checkRows(string gameBoard[ROWS][COLS], string &result, mutex &m)
{
    for (int r = 0; r < ROWS; r++)
    {
        if (gameBoard[r][0] != EMPTY &&
            gameBoard[r][0] == gameBoard[r][1] &&
            gameBoard[r][1] == gameBoard[r][2])
        {
            lock_guard<mutex> lock(m);
            result = gameBoard[r][0];
            return;
        }
    }
}

void checkCols(string gameBoard[ROWS][COLS], string &result, mutex &m)
{
    for (int c = 0; c < COLS; c++)
    {
        if (gameBoard[0][c] != EMPTY &&
            gameBoard[0][c] == gameBoard[1][c] &&
            gameBoard[1][c] == gameBoard[2][c])
        {
            lock_guard<mutex> lock(m);
            result = gameBoard[0][c];
            return;
        }
    }
}

void checkDiagonals(string gameBoard[ROWS][COLS], string &result, mutex &m)
{
    if (gameBoard[1][1] == EMPTY) return;

    if ((gameBoard[0][0] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][2]) ||
        (gameBoard[0][2] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][0]))
    {
        lock_guard<mutex> lock(m);
        result = gameBoard[1][1];
    }
}

string getWinner(string gameBoard[ROWS][COLS])
{
    string result = EMPTY;
    mutex m;

    thread rowThread(checkRows, gameBoard, ref(result), ref(m));
    thread colThread(checkCols, gameBoard, ref(result), ref(m));
    thread diagThread(checkDiagonals, gameBoard, ref(result), ref(m));

    rowThread.join();
    colThread.join();
    diagThread.join();

    return result;
}

bool isBoardFull(string gameBoard[ROWS][COLS])
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (gameBoard[r][c] == EMPTY)
                return false;
    return true;
}

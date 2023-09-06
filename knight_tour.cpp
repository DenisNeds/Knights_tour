#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

class KnightTour {
public:
    KnightTour(int boardSizeX, int boardSizeY);
    void findClosedTour();
    void printBoard();
    void printToFile();
    bool isClosedTour();

private:
    int boardSizeX;
    int boardSizeY;
    vector<vector<int>> board;
    array<int, 8> moveX;
    array<int, 8> moveY;
    int start_x;
    int start_y;

    bool limits(int x, int y);
    bool isEmpty(int x, int y);
    int getDegree(int x, int y);
    bool nextMove(int& x, int& y);
    bool isSolved();
};

KnightTour::KnightTour(int boardSizeX, int boardSizeY) : boardSizeX(boardSizeX), boardSizeY(boardSizeY), start_x(0), start_y(0) {
    // Ustvarimo šahovnico
    board.resize(boardSizeY, vector<int>(boardSizeX, -1));

    // Možni premiki
    moveX = {1, 1, 2, 2, -1, -1, -2, -2};
    moveY = {2, -2, 1, -1, 2, -2, 1, -1};

    // Naključno začetno mesto
    srand(time(nullptr));
    start_x = rand() % boardSizeX;
    start_y = rand() % boardSizeY;
    board[start_y][start_x] = 1;
}

bool KnightTour::limits(int x, int y) {
    return (x >= 0 && y >= 0 && x < boardSizeX && y < boardSizeY);
}

bool KnightTour::isEmpty(int x, int y) {
    return limits(x, y) && board[y][x] == -1;
}

int KnightTour::getDegree(int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; ++i) {
        int next_x = x + moveX[i];
        int next_y = y + moveY[i];
        if (isEmpty(next_x, next_y)) {
            count++;
        }
    }
    return count;
}

bool KnightTour::nextMove(int& x, int& y) {
    int min_deg_idx = -1, c, min_deg = 9, nx, ny;

    int start = rand() % 8;
    for (int count = 0; count < 8; ++count) {
        int i = (start + count) % 8;
        nx = x + moveX[i];
        ny = y + moveY[i];
        if (isEmpty(nx, ny) && (c = getDegree(nx, ny)) < min_deg) {
            min_deg_idx = i;
            min_deg = c;
        }
    }

    if (min_deg_idx == -1) {
        return false;
    }

    nx = x + moveX[min_deg_idx];
    ny = y + moveY[min_deg_idx];
    board[ny][nx] = board[y][x] + 1;
    x = nx;
    y = ny;

    return true;
}

bool KnightTour::isSolved() {
    for (int i = 0; i < boardSizeY; ++i) {
        for (int j = 0; j < boardSizeX; ++j) {
            if (board[i][j] == -1) {
                return false;
            }
        }
    }
    return true;
}

bool KnightTour::isClosedTour() {
    int x = start_x, y = start_y;
    vector<pair<int, int>> possible_moves;

    for (int i = 1; i < boardSizeX * boardSizeY; ++i) {
        if (!nextMove(x, y)) {
            return false;
        }
        possible_moves.clear();

        for (int j = 0; j < 8; ++j) {
            int nx = x + moveX[j];
            int ny = y + moveY[j];
            if (limits(nx, ny) && isEmpty(nx, ny)) {
                possible_moves.emplace_back(nx, ny);
            }
        }

        // Preverimo, ali je možen premik slučajno začetno mesto
        for (const auto& move : possible_moves) {
            if (move.first == start_x && move.second == start_y) {
                int next_x = x;
                int next_y = y;
                if (nextMove(next_x, next_y) && next_x == start_x && next_y == start_y) {
                    return true;
                }
            }
        }
    }

    return false; // Konj se ne vrne na začetno mesto
}


void KnightTour::findClosedTour() {
    int x = start_x, y = start_y;
    for (int i = 1; i < boardSizeX * boardSizeY; ++i) {
        if (!nextMove(x, y)) {
            cout << "Ni Knight's Toura." << endl;
            return;
        }
    }
    if (!isSolved()) {
        cout << "Ni Knight's Toura." << endl;
        return;
    }
}

void KnightTour::printBoard() {
    for (int i = 0; i < boardSizeY; ++i) {
        for (int j = 0; j < boardSizeX; ++j) {
            cout << board[i][j] << "\t";
        }
        cout << endl;
    }
}

void KnightTour::printToFile() {
    ofstream outputFile("output_data.txt");

    for (int i = 0; i < boardSizeY; ++i) {
        for (int j = 0; j < boardSizeX; ++j) {
            outputFile << board[i][j] << "\t";
        }
        outputFile << endl;
    }

    outputFile.close();
}

int main() {
    int boardSizeX, boardSizeY;
    cout << "Vnesi dimenzijo šahovnice (M x N): ";
    cin >> boardSizeX >> boardSizeY;

    KnightTour tour(boardSizeX, boardSizeY);

    auto start = high_resolution_clock::now();

    tour.findClosedTour();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Knight's tour:" << endl;
    tour.printBoard();
    tour.printToFile();

    if (tour.isClosedTour()) {
        cout << "Knight's tour je sklenjen." << endl;
    } else {
        cout << "Knight's tour ni sklenjen." << endl;
    }

    cout << "Čas izvajanja: " << duration.count() << " mikrosekund" << endl;

    return 0;
}

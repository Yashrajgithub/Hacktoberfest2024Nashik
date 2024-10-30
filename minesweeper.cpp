#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 10;
const int MINES = 10;

enum CellState {
    HIDDEN,
    REVEALED,
    FLAGGED
};

struct Cell {
    bool hasMine;
    int adjacentMines;
    CellState state;
};

class Minesweeper {
private:
    vector<vector<Cell>> grid;
    bool gameOver;
    bool gameWon;

public:
    Minesweeper() {
        grid.resize(HEIGHT, vector<Cell>(WIDTH, {false, 0, HIDDEN}));
        gameOver = false;
        gameWon = false;
        placeMines();
        calculateAdjacentMines();
    }

    void placeMines() {
        srand(static_cast<unsigned int>(time(0)));
        int minesPlaced = 0;
        while (minesPlaced < MINES) {
            int x = rand() % WIDTH;
            int y = rand() % HEIGHT;
            if (!grid[y][x].hasMine) {
                grid[y][x].hasMine = true;
                minesPlaced++;
            }
        }
    }

    void calculateAdjacentMines() {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (grid[y][x].hasMine) {
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            int ny = y + dy;
                            int nx = x + dx;
                            if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH && !grid[ny][nx].hasMine) {
                                grid[ny][nx].adjacentMines++;
                            }
                        }
                    }
                }
            }
        }
    }

    void displayGrid(bool revealMines = false) {
        cout << "   ";
        for (int x = 0; x < WIDTH; x++) {
            cout << setw(2) << x << " ";
        }
        cout << endl;

        for (int y = 0; y < HEIGHT; y++) {
            cout << setw(2) << y << " ";
            for (int x = 0; x < WIDTH; x++) {
                if (grid[y][x].state == HIDDEN) {
                    cout << " * ";
                } else if (grid[y][x].state == FLAGGED) {
                    cout << " F ";
                } else if (grid[y][x].hasMine && revealMines) {
                    cout << " M ";
                } else {
                    cout << " " << grid[y][x].adjacentMines << " ";
                }
            }
            cout << endl;
        }
    }

    void revealCell(int x, int y) {
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || grid[y][x].state != HIDDEN) {
            return;
        }

        grid[y][x].state = REVEALED;

        if (grid[y][x].hasMine) {
            gameOver = true;
        } else if (grid[y][x].adjacentMines == 0) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    revealCell(x + dx, y + dy);
                }
            }
        }
    }

    void flagCell(int x, int y) {
        if (grid[y][x].state == HIDDEN) {
            grid[y][x].state = FLAGGED;
        } else if (grid[y][x].state == FLAGGED) {
            grid[y][x].state = HIDDEN;
        }
    }

    bool checkWin() {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (!grid[y][x].hasMine && grid[y][x].state == HIDDEN) {
                    return false;
                }
            }
        }
        gameWon = true;
        return true;
    }

    void playGame() {
        while (!gameOver && !gameWon) {
            displayGrid();
            int x, y;
            char action;

            cout << "Enter coordinates (x y) and action (r for reveal, f for flag): ";
            cin >> x >> y >> action;

            if (action == 'r') {
                revealCell(x, y);
            } else if (action == 'f') {
                flagCell(x, y);
            }

            checkWin();
        }

        if (gameOver) {
            cout << "Game Over! You hit a mine." << endl;
            displayGrid(true);
        } else if (gameWon) {
            cout << "Congratulations! You've cleared the minefield." << endl;
        }
    }
};

int main() {
    Minesweeper game;
    game.playGame();
    return 0;
}

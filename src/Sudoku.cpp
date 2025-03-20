#include "Sudoku.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <random>
#include <chrono>

Sudoku::Sudoku(int blockRows, int blockCols)
        : blockRows(blockRows), blockCols(blockCols) {
    size = blockRows * blockCols;
    board.assign(size, std::vector<int>(size, 0));
}

bool Sudoku::isValid(int row, int col, int num) {
    // Check row/col
    for (int i = 0; i < size; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }
    // Check sub-grid
    int startRow = (row / blockRows) * blockRows;
    int startCol = (col / blockCols) * blockCols;
    for (int r = 0; r < blockRows; r++) {
        for (int c = 0; c < blockCols; c++) {
            if (board[startRow + r][startCol + c] == num) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::findEmpty(int &row, int &col) {
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            if (board[row][col] == 0)
                return true;
        }
    }
    return false;
}

bool Sudoku::solve() {
    int row, col;
    if (!findEmpty(row, col)) {
        return true;
    }

    // Shuffle possible numbers
    static std::mt19937 rng(
            (unsigned)std::chrono::steady_clock::now().time_since_epoch().count()
    );
    std::vector<int> nums(size);
    for (int i = 0; i < size; i++) {
        nums[i] = i + 1;
    }
    std::shuffle(nums.begin(), nums.end(), rng);

    for (int num : nums) {
        if (isValid(row, col, num)) {
            board[row][col] = num;
            if (solve()) {
                return true;
            }
            board[row][col] = 0; // backtrack
        }
    }
    return false;
}

void Sudoku::generateFullBoard() {
    // Clear board first
    for (auto &row : board) {
        std::fill(row.begin(), row.end(), 0);
    }
    solve();
}

void Sudoku::removeNumbers(int clues) {
    int totalCells = size * size;
    if (clues <= 0) {
        // keep ~60% cells by default
        clues = totalCells - static_cast<int>(0.4 * totalCells);
    }

    // Gather all cells
    std::vector<std::pair<int,int>> cells;
    cells.reserve(totalCells);
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            cells.push_back({r, c});
        }
    }

    // Shuffle
    static std::mt19937 rng(
            (unsigned)std::chrono::steady_clock::now().time_since_epoch().count()
    );
    std::shuffle(cells.begin(), cells.end(), rng);

    int removed = 0;
    while ((totalCells - removed) > clues && !cells.empty()) {
        auto cell = cells.back();
        cells.pop_back();
        board[cell.first][cell.second] = 0;
        removed++;
    }
}

bool Sudoku::loadFromFile(const std::string &filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }

    // TODO: this is very minimal rn
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (!(fin >> board[r][c])) {
                std::cerr << "Error: invalid puzzle format in file.\n";
                return false;
            }
        }
    }

    fin.close();
    return true;
}

void Sudoku::printBoard(int cursorRow, int cursorCol) {
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            // Mark cursor position in brackets for demonstration
            if (r == cursorRow && c == cursorCol) {
                if (board[r][c] == 0) {
                    std::cout << "[.] ";
                } else {
                    std::cout << "[" << board[r][c] << "] ";
                }
            } else {
                if (board[r][c] == 0) {
                    std::cout << ".  ";
                } else {
                    std::cout << board[r][c] << "  ";
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool Sudoku::checkSolved() {
    // Quick check: no zeros and isValid for each filled cell
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            int val = board[r][c];
            if (val == 0) return false;
            // Temporarily empty this cell and check validity
            board[r][c] = 0;
            bool valid = isValid(r, c, val);
            // Restore
            board[r][c] = val;
            if (!valid) {
                return false;
            }
        }
    }
    return true;
}

void Sudoku::placeNumber(int row, int col, int value) {
    if (row < 0 || row >= size || col < 0 || col >= size) return;

    if (value >= 0 && value <= 9) {
        board[row][col] = value;
    }
}

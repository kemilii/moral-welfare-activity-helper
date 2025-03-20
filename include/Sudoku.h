#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <string>

class Sudoku {
public:
    Sudoku(int blockRows, int blockCols);

    void generateFullBoard();

    /**
     * Remove numbers to create a puzzle with a certain number of clues.
     * If clues <= 0, a default puzzle is created (keeping ~60% filled).
     */
    void removeNumbers(int clues = 0);

    bool loadFromFile(const std::string &filename);

    /// Print the board.
    void printBoard(int cursorRow = -1, int cursorCol = -1);

    /// Check if the puzzle is solved (simple check).
    bool checkSolved();

    /// Place a number on the board (1–9) or 0 to clear, if valid.
    void placeNumber(int row, int col, int value);

    /// Board dimension = blockRows * blockCols
    int getSize() const { return size; }

private:
    int blockRows;
    int blockCols;
    int size;  // size = blockRows * blockCols
    std::vector<std::vector<int>> board;

    // Internal helper methods:
    bool isValid(int row, int col, int num);
    bool findEmpty(int &row, int &col);
    bool solve();

};

#endif // SUDOKU_H

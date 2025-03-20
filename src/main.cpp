#include "Sudoku.h"
#include <iostream>
#include <string>
#include <cstdlib>


void printHelp() {
    std::cout << "Usage:\n"
              << "  ./sudoku             # Direct start\n"
              << "  ./sudoku -l <file>   # Load game progress from file\n"
              << "  ./sudoku -h          # Get help\n\n"
              << "Controls:\n"
              << "  w,a,s,d  - Move cursor up/left/down/right\n"
              << "  0        - Remove the digit in current cell\n"
              << "  1..9     - Place digit 1..9 in current cell\n"
              << "  enter    - Attempt to check if puzzle is solved\n"
              << "  esc      - Exit the game\n\n";
}

int main(int argc, char* argv[]) {
    // Default puzzle config
    int blockRows = 3;
    int blockCols = 3;
    int clues     = 5;
    bool loadFromFile = false;
    std::string filename;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h") {
            printHelp();
            return 0;
        } else if (arg == "-l" && (i + 1) < argc) {
            loadFromFile = true;
            filename = argv[++i];
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 1;
        }
    }

    // Ask for Sudoku sub-grid dimensions
    std::cout << "Enter number of block rows: ";
    std::cin >> blockRows;
    std::cout << "Enter number of block columns: ";
    std::cin >> blockCols;

    // Create Sudoku object
    Sudoku sudoku(blockRows, blockCols);

    if (loadFromFile) {
        // Attempt to load puzzle from file
        if (!sudoku.loadFromFile(filename)) {
            std::cerr << "Failed to load puzzle from file. Exiting.\n";
            return 1;
        }
        std::cout << "Puzzle loaded from file: " << filename << "\n";
    } else {
        // Otherwise, generate a new puzzle
        sudoku.generateFullBoard();
        sudoku.removeNumbers(clues);
    }

    std::cout << "Starting Sudoku (size = " << sudoku.getSize() << "x"
              << sudoku.getSize() << ")\n";
    printHelp(); // remind

    int cursorRow = 0;
    int cursorCol = 0;
    sudoku.printBoard(cursorRow, cursorCol);

    while (true) {
        std::cout << "Input command (w/a/s/d, 0..9, enter, esc): ";
        std::string cmd;
        if (!(std::cin >> cmd)) {
            std::cout << "Exiting...\n";
            break;
        }

        if (cmd == "esc") {
            std::cout << "Exiting game.\n";
            break;
        } else if (cmd == "w") {
            cursorRow = (cursorRow > 0) ? cursorRow - 1 : cursorRow;
        } else if (cmd == "s") {
            cursorRow = (cursorRow < sudoku.getSize() - 1) ? cursorRow + 1 : cursorRow;
        } else if (cmd == "a") {
            cursorCol = (cursorCol > 0) ? cursorCol - 1 : cursorCol;
        } else if (cmd == "d") {
            cursorCol = (cursorCol < sudoku.getSize() - 1) ? cursorCol + 1 : cursorCol;
        } else if (cmd == "enter") {
            // Attempt to check if puzzle is solved
            if (sudoku.checkSolved()) {
                std::cout << "Congratulations! Puzzle solved.\n";
                sudoku.printBoard(cursorRow, cursorCol);
                break;
            } else {
                std::cout << "Not solved yet. Keep trying!\n";
            }
        } else if (cmd.size() == 1 && (cmd[0] >= '0' && cmd[0] <= '9')) {
            int val = cmd[0] - '0';
            sudoku.placeNumber(cursorRow, cursorCol, val);
        } else {
            std::cout << "Unrecognized command.\n";
        }

        // Print updated board
        sudoku.printBoard(cursorRow, cursorCol);
    }

    return 0;
}

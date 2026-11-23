#pragma once
#include <cstdlib>

// Task 3: Randomly select a piece (0-6) and its starting rotation (0-3)
void spawnNewPiece(int& currentPiece, int& currentRotation, int& pieceX, int& pieceY, int cols) {
    currentPiece = rand() % 7;
    currentRotation = rand() % 4;
    pieceX = (cols / 2) - 2;
    pieceY = 0;
}

// Task 3: Check if a piece can move/rotate to (testX, testY) with testRotation
bool canMove(int board[20][10], char pieces[7][4][4][4], int rows, int cols,
    int pieceIdx, int testRotation, int testX, int testY) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            char cellChar = pieces[pieceIdx][testRotation][r][c];
            if (cellChar != '.' && cellChar != '0') {
                int boardX = testX + c;
                int boardY = testY + r;

                if (boardX < 0 || boardX >= cols || boardY >= rows) {
                    return false;
                }
                if (boardY >= 0 && board[boardY][boardX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Task 4: Lock the active piece permanently onto the board
void lockPiece(int board[20][10], char pieces[7][4][4][4], int rows, int cols,
    int pieceIdx, int rotation, int pieceX, int pieceY) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            char cellChar = pieces[pieceIdx][rotation][r][c];
            if (cellChar != '.' && cellChar != '0') {
                int boardX = pieceX + c;
                int boardY = pieceY + r;
                if (boardY >= 0 && boardY < rows && boardX >= 0 && boardX < cols) {
                    board[boardY][boardX] = pieceIdx + 1;
                }
            }
        }
    }
}

// Task 4: Clear full horizontal rows, shift upper rows down, and update score
int clearFullLines(int board[20][10], int rows, int cols, int& score) {
    int linesCleared = 0;

    for (int r = rows - 1; r >= 0; r--) {
        bool isFull = true;
        for (int c = 0; c < cols; c++) {
            if (board[r][c] == 0) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            linesCleared++;
            for (int above = r; above > 0; above--) {
                for (int c = 0; c < cols; c++) {
                    board[above][c] = board[above - 1][c];
                }
            }
            for (int c = 0; c < cols; c++) {
                board[0][c] = 0;
            }
            r++;
        }
    }

    if (linesCleared == 1) score += 100;
    else if (linesCleared == 2) score += 300;
    else if (linesCleared == 3) score += 500;
    else if (linesCleared >= 4) score += 800;

    return linesCleared;
}

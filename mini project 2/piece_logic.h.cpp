#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
using namespace std;

// Active falling piece variables (Task 3)
int currentPiece = 0;
char currentShape;
int pieceRow = 0;
int pieceCol = 3; // Starts at row 0, col 3 (from Section 4)
int fallTimer = 0;

// Copy a 4x4 piece from pieces[p] into currentShape
void spawnPiece(int p, char pieces) {
    currentPiece = p;
    pieceRow = 0;
    pieceCol = 3;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            currentShape[r][c] = pieces[p][r][c];
        }
    }
}

// Check if 'shape' fits at (targetRow, targetCol) without hitting walls or blocks (Task 3)
bool canPlace(char shape, int targetRow, int targetCol, int board) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (shape[r][c] == '#') {
                int boardR = targetRow + r;
                int boardC = targetCol + c;

                // Hit left wall, right wall, or floor?
                if (boardC < 0 || boardC >= 10 || boardR >= 20) {
                    return false;
                }
                // Hit a landed block on the board?
                if (boardR >= 0 && board[boardR][boardC] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Rotate the piece clockwise into a copy first (Task 3)
void rotatePiece(int board) {
    char temp;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            temp[r][c] = currentShape[3 - c][r];
        }
    }

    // Only keep the rotation if it fits!
    if (canPlace(temp, pieceRow, pieceCol, board)) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                currentShape[r][c] = temp[r][c];
            }
        }
    }
}

// Lock the landed piece into the board and spawn a new random one (Task 4)
void lockPiece(int board, char pieces) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (currentShape[r][c] == '#') {
                // Store (currentPiece + 1) so colors match 1 to 7
                board[pieceRow + r][pieceCol + c] = currentPiece + 1;
            }
        }
    }
    spawnPiece(rand() % 7, pieces);
}

// Handle Left, Right, Down, Rotate (Up/W), and Hard Drop (Space) (Tasks 3 & 4)
void handlePieceKeys(sf::Keyboard::Key code, int board, char pieces) {
    // Move Left (Left Arrow or A)
    if (code == sf::Keyboard::Key::Left || code == sf::Keyboard::Key::A) {
        if (canPlace(currentShape, pieceRow, pieceCol - 1, board)) pieceCol--;
    }
    // Move Right (Right Arrow or D)
    if (code == sf::Keyboard::Key::Right || code == sf::Keyboard::Key::D) {
        if (canPlace(currentShape, pieceRow, pieceCol + 1, board)) pieceCol++;
    }
    // Move Down (Down Arrow or S)
    if (code == sf::Keyboard::Key::Down || code == sf::Keyboard::Key::S) {
        if (canPlace(currentShape, pieceRow + 1, pieceCol, board)) pieceRow++;
    }
    // Rotate (Up Arrow or W)
    if (code == sf::Keyboard::Key::Up || code == sf::Keyboard::Key::W) {
        rotatePiece(board);
    }
    // Hard Drop (Spacebar) - Task 4
    if (code == sf::Keyboard::Key::Space) {
        while (canPlace(currentShape, pieceRow + 1, pieceCol, board)) {
            pieceRow++;
        }
        lockPiece(board, pieces);
    }
}

// Automatic gravity every 30 frames (Task 4)
void updateFallingPiece(int board, char pieces) {
    fallTimer++;
    if (fallTimer >= 30) {
        fallTimer = 0;
        if (canPlace(currentShape, pieceRow + 1, pieceCol, board)) {
            pieceRow++;
        }
        else {
            lockPiece(board, pieces);
        }
    }
}

// Draw the active falling 4x4 piece on screen
void drawActivePiece(sf::RenderWindow& window, sf::Color colors, float boardX, float boardY, float cell) {
    sf::RectangleShape block({ cell - 2, cell - 2 });
    block.setFillColor(colors[currentPiece + 1]);

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (currentShape[r][c] == '#') {
                block.setPosition({ boardX + (pieceCol + c) * cell + 1, boardY + (pieceRow + r) * cell + 1 });
                window.draw(block);
            }
        }
    }
}

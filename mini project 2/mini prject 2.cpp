#include "piece_logic.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

sf::Color getPieceColor(int id) {
    switch (id) {
    case 1: return sf::Color::Cyan;
    case 2: return sf::Color::Yellow;
    case 3: return sf::Color::Magenta;
    case 4: return sf::Color::Green;
    case 5: return sf::Color::Red;
    case 6: return sf::Color::Blue;
    case 7: return sf::Color(255, 165, 0);
    default: return sf::Color(30, 30, 30);
    }
}

int main() {
    srand((unsigned int)time(0));

    char pieces[7][4][4][4];
    ifstream fin("pieces.txt");
    if (!fin) {
        cout << "Error: Could not open pieces.txt!" << endl;
        return 1;
    }

    for (int p = 0; p < 7; p++) {
        for (int rot = 0; rot < 4; rot++) {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    fin >> pieces[p][rot][r][c];
                }
            }
        }
    }
    fin.close();

    const int ROWS = 20;
    const int COLS = 10;
    const float CELL_SIZE = 30.0f;
    const float OFFSET_X = 20.0f;
    const float OFFSET_Y = 20.0f;

    int board[ROWS][COLS] = { 0 };
    int score = 0;
    bool gameOver = false;

    int currentPiece = 0;
    int currentRotation = 0;
    int pieceX = 3;
    int pieceY = 0;

    spawnNewPiece(currentPiece, currentRotation, pieceX, pieceY, COLS);

    sf::RenderWindow window(sf::VideoMode({ 520, 640 }), "Tetris - Mini Project 2");
    window.setFramerateLimit(60);

    sf::Clock fallClock;
    float fallDelay = 0.5f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (!gameOver) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Left) {
                        if (canMove(board, pieces, ROWS, COLS, currentPiece, currentRotation, pieceX - 1, pieceY)) {
                            pieceX--;
                        }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Right) {
                        if (canMove(board, pieces, ROWS, COLS, currentPiece, currentRotation, pieceX + 1, pieceY)) {
                            pieceX++;
                        }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Down) {
                        if (canMove(board, pieces, ROWS, COLS, currentPiece, currentRotation, pieceX, pieceY + 1)) {
                            pieceY++;
                        }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::Up) {
                        int nextRot = (currentRotation + 1) % 4;
                        if (canMove(board, pieces, ROWS, COLS, currentPiece, nextRot, pieceX, pieceY)) {
                            currentRotation = nextRot;
                        }
                    }
                }
            }
        }

        if (!gameOver && fallClock.getElapsedTime().asSeconds() >= fallDelay) {
            if (canMove(board, pieces, ROWS, COLS, currentPiece, currentRotation, pieceX, pieceY + 1)) {
                pieceY++;
            }
            else {
                lockPiece(board, pieces, ROWS, COLS, currentPiece, currentRotation, pieceX, pieceY);
                clearFullLines(board, ROWS, COLS, score);
                cout << "Score: " << score << endl;

                spawnNewPiece(currentPiece, currentRotation, pieceX, pieceY, COLS);

                if (!canMove(board, pieces, ROWS, COLS, currentPiece, currentRotation, pieceX, pieceY)) {
                    gameOver = true;
                    cout << "GAME OVER! Final Score: " << score << endl;
                }
            }
            fallClock.restart();
        }

        window.clear(sf::Color(15, 15, 20));

        sf::RectangleShape cell({ CELL_SIZE - 2.0f, CELL_SIZE - 2.0f });

        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                cell.setPosition({ OFFSET_X + c * CELL_SIZE, OFFSET_Y + r * CELL_SIZE });
                cell.setFillColor(getPieceColor(board[r][c]));
                window.draw(cell);
            }
        }

        if (!gameOver) {
            cell.setFillColor(getPieceColor(currentPiece + 1));
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    char cellChar = pieces[currentPiece][currentRotation][r][c];
                    if (cellChar != '.' && cellChar != '0') {
                        int drawX = pieceX + c;
                        int drawY = pieceY + r;
                        if (drawY >= 0 && drawY < ROWS && drawX >= 0 && drawX < COLS) {
                            cell.setPosition({ OFFSET_X + drawX * CELL_SIZE, OFFSET_Y + drawY * CELL_SIZE });
                            window.draw(cell);
                        }
                    }
                }
            }
        }

        window.display();
    }

    return 0;
}

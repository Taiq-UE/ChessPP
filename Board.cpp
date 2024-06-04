#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Board.h"
#include "Piece.h"

Board::Board(bool isPvP) : isPvP(isPvP) {

    whiteKingMoved = false;
    blackKingMoved = false;
    whiteRookLeftMoved = false;
    whiteRookRightMoved = false;
    blackRookLeftMoved = false;
    blackRookRightMoved = false;
    game_over = false;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = nullptr;
        }
    }

    board[0][0] = new Rook(false);
    board[0][1] = new Knight(false);
    board[0][2] = new Bishop(false);
    board[0][3] = new Queen(false);
    board[0][4] = new King(false);
    board[0][5] = new Bishop(false);
    board[0][6] = new Knight(false);
    board[0][7] = new Rook(false);
    for (int i = 0; i < 8; ++i) {
        board[1][i] = new Pawn(false);
    }

    board[7][0] = new Rook(true);
    board[7][1] = new Knight(true);
    board[7][2] = new Bishop(true);
    board[7][3] = new Queen(true);
    board[7][4] = new King(true);
    board[7][5] = new Bishop(true);
    board[7][6] = new Knight(true);
    board[7][7] = new Rook(true);
    for (int i = 0; i < 8; ++i) {
        board[6][i] = new Pawn(true);
    }

    isWhiteTurn = true;
}

Piece* Board::getPiece(int x, int y) {
    return board[y][x];
}

void Board::setPiece(int x, int y, Piece* piece) {
    board[y][x] = piece;
}

bool Board::movePiece(int startX, int startY, int endX, int endY) {
    std::cout << "Attempting to move piece from (" << startX << ", " << startY << ") to (" << endX << ", " << endY << ")" << std::endl;

    Piece* piece = getPiece(startX, startY);

    if (piece == nullptr || piece->getIsWhite() != isWhiteTurn) {
        std::cout << "Invalid move: No piece at start or not the correct turn." << std::endl;
        return false;
    }

    if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) {
        if (abs(startY - endY) == 1 && abs(startX - endX) == 1 && getPiece(endX, endY) == nullptr) {
            Piece* capturedPawn = getPiece(endX, startY);
            if (Pawn* capturedPawnCast = dynamic_cast<Pawn*>(capturedPawn)) {
                if (capturedPawnCast->getJustDoubleJumped()) {
                    setPiece(endX, startY, nullptr);
                    setPiece(endX, endY, pawn);
                    setPiece(startX, startY, nullptr);
                    std::cout << "En passant move: Captured pawn at (" << endX << ", " << startY << ")" << std::endl;
                    switchTurn();
                    return true;
                }
            }
        }
    }

    if (dynamic_cast<King*>(piece)) {
        if (isWhiteTurn) {
            if (startX == 4 && startY == 7 && endX == 6 && endY == 7) {
                if (canCastle(true, true)) {
                    setPiece(6, 7, piece);
                    setPiece(4, 7, nullptr);
                    setPiece(5, 7, getPiece(7, 7));
                    setPiece(7, 7, nullptr);
                    whiteKingMoved = true;
                    whiteRookRightMoved = true;
                    switchTurn();
                    return true;
                } else {
                    return false;
                }
            } else if (startX == 4 && startY == 7 && endX == 2 && endY == 7) {
                if (canCastle(true, false)) {
                    setPiece(2, 7, piece);
                    setPiece(4, 7, nullptr);
                    setPiece(3, 7, getPiece(0, 7));
                    setPiece(0, 7, nullptr);
                    whiteKingMoved = true;
                    whiteRookLeftMoved = true;
                    switchTurn();
                    return true;
                } else {
                    return false;
                }
            }
        } else {
            if (startX == 4 && startY == 0 && endX == 6 && endY == 0) { // Roszada krótka
                if (canCastle(false, true)) {
                    setPiece(6, 0, piece);
                    setPiece(4, 0, nullptr);
                    setPiece(5, 0, getPiece(7, 0));
                    setPiece(7, 0, nullptr);
                    blackKingMoved = true;
                    blackRookRightMoved = true;
                    switchTurn();
                    return true;
                } else {
                    return false;
                }
            } else if (startX == 4 && startY == 0 && endX == 2 && endY == 0) { // Roszada długa
                if (canCastle(false, false)) {
                    setPiece(2, 0, piece);
                    setPiece(4, 0, nullptr);
                    setPiece(3, 0, getPiece(0, 0));
                    setPiece(0, 0, nullptr);
                    blackKingMoved = true;
                    blackRookLeftMoved = true;
                    switchTurn();
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    if (!piece->isValidMove(*this, startX, startY, endX, endY)) {
        std::cout << "Invalid move for the piece." << std::endl;
        return false;
    }

    if (endX < 0 || endX >= 8 || endY < 0 || endY >= 8) {
        std::cout << "Invalid move: End position is out of bounds." << std::endl;
        return false;
    }

    Piece* endPiece = getPiece(endX, endY);
    if (endPiece != nullptr && endPiece->getIsWhite() == piece->getIsWhite()) {
        std::cout << "Invalid move: End position occupied by same color." << std::endl;
        return false;
    }

    setPiece(endX, endY, piece);
    setPiece(startX, startY, nullptr);

    if (isCheck(isWhiteTurn)) {
        std::cout << "Invalid move: Move puts own king in check." << std::endl;
        setPiece(startX, startY, piece);
        setPiece(endX, endY, endPiece);
        return false;
    }

    bool opponentInCheck = isCheck(!isWhiteTurn);
    std::cout << "Opponent in check: " << (opponentInCheck ? "Yes" : "No") << std::endl;

    if (opponentInCheck) {
        if (!findMoveToRemoveCheck(!isWhiteTurn)) {
            std::cout << "Checkmate detected." << std::endl;
            game_over = true;
            QMessageBox::information(nullptr, "Checkmate", "Checkmate!");
        } else {
            std::cout << "Check detected." << std::endl;
            QMessageBox::information(nullptr, "Check", "Check!");
        }
    } else {
        if (!isAnyMovePossible(!isWhiteTurn)) {
            std::cout << "Stalemate detected." << std::endl;
            game_over = true;
            QMessageBox::information(nullptr, "Stalemate", "Stalemate!");
        }
    }

    if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) {
        pawn->move();
        if ((pawn->getIsWhite() && endY == 0) || (!pawn->getIsWhite() && endY == 7)) {
            promotePawn(endX, endY);
        }
    }

    if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) {
        if (abs(startY - endY) == 2) {
            pawn->setJustDoubleJumped(true);
        } else {
            pawn->setJustDoubleJumped(false);
        }
    }

    resetPawnsJustDoubleJumped(dynamic_cast<Pawn*>(piece));

    switchTurn();

    std::cout << "Successful move from (" << startX << ", " << startY << ") to (" << endX << ", " << endY << ")" << std::endl;

    if (isOnlyKingsLeft()) {
        std::cout << "Draw!" << std::endl;
        QMessageBox::information(nullptr, "Draw!", "Draw! Only kings left.");
        return false; // Stop further moves
    }

    if (dynamic_cast<King*>(piece)) {
        if (isWhiteTurn) whiteKingMoved = true;
        else blackKingMoved = true;
    } else if (dynamic_cast<Rook*>(piece)) {
        if (isWhiteTurn) {
            if (startX == 0 && startY == 7) whiteRookLeftMoved = true;
            else if (startX == 7 && startY == 7) whiteRookRightMoved = true;
        } else {
            if (startX == 0 && startY == 0) blackRookLeftMoved = true;
            else if (startX == 7 && startY == 0) blackRookRightMoved = true;
        }
    }

    return true;
}

void Board::switchTurn() {
    isWhiteTurn = !isWhiteTurn;
}

void Board::promotePawn(int x, int y) {
    Piece* pawn = getPiece(x, y);
    bool isWhite = pawn->getIsWhite();
    delete pawn;

    QMessageBox msgBox;
    msgBox.setText("Pawn Promotion");
    msgBox.setInformativeText("Choose the piece to promote your pawn to:");
    QAbstractButton *queenButton = (QAbstractButton*)msgBox.addButton("Queen", QMessageBox::ActionRole);
    QAbstractButton *rookButton = (QAbstractButton*)msgBox.addButton("Rook", QMessageBox::ActionRole);
    QAbstractButton *bishopButton = (QAbstractButton*)msgBox.addButton("Bishop", QMessageBox::ActionRole);
    QAbstractButton *knightButton = (QAbstractButton*)msgBox.addButton("Knight", QMessageBox::ActionRole);

    msgBox.exec();

    QAbstractButton* clickedButton = msgBox.clickedButton();

    if (clickedButton == queenButton) {
        setPiece(x, y, new Queen(isWhite));
    } else if (clickedButton == rookButton) {
        setPiece(x, y, new Rook(isWhite));
    } else if (clickedButton == bishopButton) {
        setPiece(x, y, new Bishop(isWhite));
    } else if (clickedButton == knightButton) {
        setPiece(x, y, new Knight(isWhite));
    } else {
        setPiece(x, y, new Queen(isWhite));
    }
}

bool Board::isCheck(bool isWhite) {
    int kingX = -1;
    int kingY = -1;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = getPiece(x, y);
            if (piece != nullptr && piece->getIsWhite() == isWhite && dynamic_cast<King*>(piece)) {
                kingX = x;
                kingY = y;
                std::cout << (isWhite ? "White" : "Black") << " King found at (" << kingX << ", " << kingY << ")" << std::endl;
                break;
            }
        }
        if (kingX != -1) {
            break;
        }
    }

    if (kingX == -1 || kingY == -1) {
        std::cerr << "Error: King not found!" << std::endl;
        return false;
    }

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = getPiece(x, y);
            if (piece != nullptr && piece->getIsWhite() != isWhite) {
                bool validMove = piece->isValidMove(*this, x, y, kingX, kingY);
//                std::cout << "Checking piece at (" << x << ", " << y << "), can move to king's position: " << validMove << std::endl;
                if (validMove) {
//                    std::cout << "Check detected by piece at (" << x << ", " << y << ")" << std::endl;
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::isAnyMovePossible(bool isWhite) {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = getPiece(x, y);
            if (piece != nullptr && piece->getIsWhite() == isWhite) {
                for (int newX = 0; newX < 8; ++newX) {
                    for (int newY = 0; newY < 8; ++newY) {
                        if (newX != x || newY != y) {
                            if (piece->isValidMove(*this, x, y, newX, newY)) {
                                Piece* displacedPiece = getPiece(newX, newY);
                                setPiece(newX, newY, piece);
                                setPiece(x, y, nullptr);

                                bool kingInCheck = isCheck(isWhite);

                                setPiece(x, y, piece);
                                setPiece(newX, newY, displacedPiece);

                                if (!kingInCheck) {
                                    std::cout << "Possible move: (" << x << ", " << y << ") to (" << newX << ", " << newY << ")" << std::endl;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Board::isOnlyKingsLeft() {
    int pieceCount = 0;

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = getPiece(x, y);
            if (piece != nullptr) {
                if (++pieceCount > 2 || dynamic_cast<King*>(piece) == nullptr) {
                    return false;
                }
            }
        }
    }

    return pieceCount == 2;
}

bool Board::findMoveToRemoveCheck(bool isWhite) {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = getPiece(x, y);
            if (piece != nullptr && piece->getIsWhite() == isWhite) {
                for (int newX = 0; newX < 8; ++newX) {
                    for (int newY = 0; newY < 8; ++newY) {
                        if (piece->isValidMove(*this, x, y, newX, newY)) {
                            Piece* displacedPiece = getPiece(newX, newY);
                            setPiece(newX, newY, piece);
                            setPiece(x, y, nullptr);

                            bool kingInCheck = isCheck(isWhite);

                            setPiece(x, y, piece);
                            setPiece(newX, newY, displacedPiece);

                            if (!kingInCheck) {
                                std::cout << "Move to remove check: (" << x << ", " << y << ") to (" << newX << ", " << newY << ")" << std::endl;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Board::canCastle(bool isWhite, bool isKingSide) {
    if (isWhite) {
        if (isKingSide) {
            if (whiteKingMoved || whiteRookRightMoved) return false;
            if (getPiece(5, 7) != nullptr || getPiece(6, 7) != nullptr) return false;
            if (isCheck(true) || isMovePuttingOwnKingInCheck(true, 4, 7, 5, 7) || isMovePuttingOwnKingInCheck(true, 4, 7, 6, 7)) return false;
        } else {
            if (whiteKingMoved || whiteRookLeftMoved) return false;
            if (getPiece(1, 7) != nullptr || getPiece(2, 7) != nullptr || getPiece(3, 7) != nullptr) return false;
            if (isCheck(true) || isMovePuttingOwnKingInCheck(true, 4, 7, 3, 7) || isMovePuttingOwnKingInCheck(true, 4, 7, 2, 7)) return false;
        }
    } else {
        if (isKingSide) {
            if (blackKingMoved || blackRookRightMoved) return false;
            if (getPiece(5, 0) != nullptr || getPiece(6, 0) != nullptr) return false;
            if (isCheck(false) || isMovePuttingOwnKingInCheck(false, 4, 0, 5, 0) || isMovePuttingOwnKingInCheck(false, 4, 0, 6, 0)) return false;
        } else {
            if (blackKingMoved || blackRookLeftMoved) return false;
            if (getPiece(1, 0) != nullptr || getPiece(2, 0) != nullptr || getPiece(3, 0) != nullptr) return false;
            if (isCheck(false) || isMovePuttingOwnKingInCheck(false, 4, 0, 3, 0) || isMovePuttingOwnKingInCheck(false, 4, 0, 2, 0)) return false;
        }
    }
    return true;
}


bool Board::isMovePuttingOwnKingInCheck(bool isWhite, int startX, int startY, int endX, int endY) {
    Piece* displacedPiece = getPiece(endX, endY);
    setPiece(endX, endY, getPiece(startX, startY));
    setPiece(startX, startY, nullptr);

    bool kingInCheck = isCheck(isWhite);

    setPiece(startX, startY, getPiece(endX, endY));
    setPiece(endX, endY, displacedPiece);

    return kingInCheck;
}

void Board::resetPawnsJustDoubleJumped(Pawn* justMovedPawn) {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = getPiece(x, y);
            if (piece != nullptr) {
                Pawn* pawn = dynamic_cast<Pawn*>(piece);
                if (pawn != nullptr && pawn != justMovedPawn) {
                    pawn->resetJustDoubleJumped();
                }
            }
        }
    }
}



void Board::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    file << serialize();
    file.close();
}

void Board::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    deserialize(data);
    file.close();
}

bool Board::getIsWhiteTurn() {
    return isWhiteTurn;
}

std::string Board::serialize() {
    std::ostringstream oss;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = getPiece(x, y);
            if (piece == nullptr) {
                oss << "-";
            } else {
                oss << typeid(*piece).name() << "_" << (piece->getIsWhite() ? "White" : "Black");
                if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) {
                    std::cout << pawn->getHasMoved() << std::endl;
                    oss << "_" << (pawn->getHasMoved() ? "1" : "0");
                }
            }
            if (x < 7) {
                oss << ",";
            }
        }
        oss << "\n";
    }

    oss << (isWhiteTurn ? "White" : "Black") << "\n";

    return oss.str();
}


void Board::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string line;

    for (int y = 0; y < 8; ++y) {
        if (!std::getline(iss, line)) {
            std::cerr << "Error during deserialization: Not enough rows in the data." << std::endl;
            return;
        }

        std::istringstream lineStream(line);
        std::string cell;

        for (int x = 0; x < 8; ++x) {
            if (!std::getline(lineStream, cell, ',')) {
                std::cerr << "Error during deserialization: Not enough cells in the row." << std::endl;
                return;
            }

            if (cell == "-") {
                board[y][x] = nullptr;
            } else {
                size_t firstUnderscorePos = cell.find('_');
                size_t secondUnderscorePos = cell.find('_', firstUnderscorePos + 1);
                std::string pieceType = cell.substr(1, firstUnderscorePos - 1);
                bool isWhite = (cell.substr(firstUnderscorePos + 1, secondUnderscorePos - firstUnderscorePos - 1) == "White");

                Piece* piece;
                if (pieceType == "Pawn") {
                    piece = new Pawn(isWhite);
                    if (cell.substr(secondUnderscorePos + 1) == "1") {
                        static_cast<Pawn*>(piece)->move();
                    }
                } else if (pieceType == "Rook") {
                    piece = new Rook(isWhite);
                } else if (pieceType == "Knight") {
                    piece = new Knight(isWhite);
                } else if (pieceType == "Bishop") {
                    piece = new Bishop(isWhite);
                } else if (pieceType == "Queen") {
                    piece = new Queen(isWhite);
                } else if (pieceType == "King") {
                    piece = new King(isWhite);
                }
                board[y][x] = piece;
            }
        }
    }

    std::string turn;
    if (iss >> turn) {
        isWhiteTurn = (turn == "White");
    } else {
        std::cerr << "Error during deserialization: Could not read the turn." << std::endl;
    }
}

#include <vector>
#include <random>

void Board::makeMove() {
    if (isPvP) {
        return;
    }

    while (true) {
        std::vector<std::tuple<int, int, int, int>> possibleMoves;

        for (int startX = 0; startX < 8; ++startX) {
            for (int startY = 0; startY < 8; ++startY) {
                Piece* piece = getPiece(startX, startY);
                if (piece != nullptr && piece->getIsWhite() == isWhiteTurn) {
                    for (int endX = 0; endX < 8; ++endX) {
                        for (int endY = 0; endY < 8; ++endY) {
                            if (piece->isValidMove(*this, startX, startY, endX, endY)) {
                                Piece* displacedPiece = getPiece(endX, endY);
                                setPiece(endX, endY, piece);
                                setPiece(startX, startY, nullptr);

                                bool kingInCheck = isCheck(isWhiteTurn);

                                setPiece(startX, startY, piece);
                                setPiece(endX, endY, displacedPiece);

                                if (!kingInCheck) {
                                    possibleMoves.push_back(std::make_tuple(startX, startY, endX, endY));
                                }
                            }
                        }
                    }
                }
            }
        }

        if (possibleMoves.empty()) {
            break;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, possibleMoves.size() - 1);

        int randomIndex = dis(gen);
        auto [startX, startY, endX, endY] = possibleMoves[randomIndex];

        if (movePiece(startX, startY, endX, endY)) {
            break;
        }

        if (isGameOver()) {
            break;
        }
    }
}

bool Board::isGameOver() const {
    return game_over;
}


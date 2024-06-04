#include "Piece.h"
#include "cmath"
#include "Board.h"

bool Piece::isValidMove(Board& board, int startX, int startY, int endX, int endY) {
    return false;
}

bool King::isValidMove(Board& board, int startX, int startY, int endX, int endY) {
    Piece *endPiece = board.getPiece(endX, endY);
    if (endPiece != nullptr && endPiece->getIsWhite() == this->getIsWhite()) {
        return false;
    }

    int deltaX = abs(startX - endX);
    int deltaY = abs(startY - endY);
    return (deltaX <= 1 && deltaY <= 1);
}

bool Queen::isValidMove(Board& board, int startX, int startY, int endX, int endY) {
    if (startX != endX && startY != endY && abs(startX - endX) != abs(startY - endY)) {
        return false;
    }

    int xDirection = (endX - startX) ? (endX - startX) / abs(endX - startX) : 0;
    int yDirection = (endY - startY) ? (endY - startY) / abs(endY - startY) : 0;

    int x = startX + xDirection;
    int y = startY + yDirection;

    while (x != endX || y != endY) {
        if (board.getPiece(x, y) != nullptr) {
            return false;
        }
        x += xDirection;
        y += yDirection;
    }

    Piece* endPiece = board.getPiece(endX, endY);
    if (endPiece != nullptr && endPiece->getIsWhite() == getIsWhite()) {
        return false;
    }

    return true;
}

bool Rook::isValidMove(Board& board, int startX, int startY, int endX, int endY) {
    if (startX != endX && startY != endY) {
        return false;
    }

    if (startX == endX && startY == endY) {
        return false;
    }

    int xDirection = (endX - startX) ? (endX - startX) / abs(endX - startX) : 0;
    int yDirection = (endY - startY) ? (endY - startY) / abs(endY - startY) : 0;

    int x = startX + xDirection;
    int y = startY + yDirection;

    while (x != endX || y != endY) {
        if (board.getPiece(x, y) != nullptr) {
            return false;
        }
        x += xDirection;
        y += yDirection;
    }

    Piece *endPiece = board.getPiece(endX, endY);
    if (endPiece != nullptr && endPiece->getIsWhite() == this->getIsWhite()) {
        return false;
    }

    return true;
}

bool Bishop::isValidMove(Board& board, int startX, int startY, int endX, int endY) {
    int deltaX = abs(startX - endX);
    int deltaY = abs(startY - endY);
    if (deltaX != deltaY) {
        return false;
    }

    // Check for pieces in the way
    int xDirection;
    if (deltaX != 0) {
        xDirection = (endX - startX) / deltaX;
    } else {
        xDirection = 0;
    }
    int yDirection;
    if (deltaY != 0) {
        yDirection = (endY - startY) / deltaY;
    } else {
        yDirection = 0;
    }

    int x = startX + xDirection;
    int y = startY + yDirection;

    while (x != endX || y != endY) {
        if (board.getPiece(x, y) != nullptr) {
            return false;
        }
        x += xDirection;
        y += yDirection;
    }

    return true;
}

bool Knight::isValidMove(Board& board, int startX, int startY, int endX, int endY) {
    // Check if the end square is occupied by a piece of the same color
    Piece *endPiece = board.getPiece(endX, endY);
    if (endPiece != nullptr && endPiece->getIsWhite() == this->getIsWhite()) {
        return false;
    }

    // Implement knight's move validation
    // Knight can move to any square not on the same rank, file, or diagonal
    int deltaX = abs(startX - endX);
    int deltaY = abs(startY - endY);
    return (deltaX == 2 && deltaY == 1) || (deltaX == 1 && deltaY == 2);
}

bool Pawn::isValidMove(Board& board, int startX, int startY, int endX, int endY) {
    int direction = isWhite ? -1 : 1;
    if (endY - startY == direction) {
        if (endX == startX) {
            // Normal move forward
            return board.getPiece(endX, endY) == nullptr;
        } else if (abs(endX - startX) == 1) {
            // Diagonal move - capture or en passant
            Piece* endPiece = board.getPiece(endX, endY);
            if (endPiece != nullptr && endPiece->getIsWhite() != isWhite) {
                // Capture
                return true;
            } else {
                // En passant
                Piece* sidePiece = board.getPiece(endX, startY);
                if (sidePiece != nullptr && sidePiece->getIsWhite() != isWhite) {
                    if (Pawn* sidePawn = dynamic_cast<Pawn*>(sidePiece)) {
                        return sidePawn->getJustDoubleJumped();
                    }
                }
            }
        }
    } else if (!hasMoved && endY - startY == 2 * direction && endX == startX) {
        // Double jump
        return board.getPiece(startX, startY + direction) == nullptr && board.getPiece(endX, endY) == nullptr;
    }
    return false;
}

void Pawn::move() {
    hasMoved = true;
}

bool Pawn::getJustDoubleJumped() const {
    return justDoubleJumped;
}

void Pawn::resetJustDoubleJumped() {
    justDoubleJumped = false;
}

void Pawn::setJustDoubleJumped(bool b) {
    justDoubleJumped = true;
}

bool Pawn::getHasMoved() {
    return hasMoved;
}


// Piece.h
#ifndef CHESSCPP_PIECE_H
#define CHESSCPP_PIECE_H
#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsPixmapItem>
#include "Board.h"

class Board;

class Piece : public QGraphicsPixmapItem {

protected:
    bool isWhite;

public:
    Piece(bool isWhite, const QPixmap& pixmap, QGraphicsItem* parent = nullptr)
            : QGraphicsPixmapItem(pixmap.scaled(50, 50), parent), isWhite(isWhite) {}
    bool getIsWhite() const { return isWhite; }
    virtual bool isValidMove(Board& board, int startX, int startY, int endX, int endY) = 0;
    void setPos(int x, int y);
};

class King : public Piece {
public:
    King(bool isWhite)
            : Piece(isWhite, QPixmap(isWhite
            ? "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\white-king.png"
            : "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\black-king.png")) {}
    bool isValidMove(Board& board, int startX, int startY, int endX, int endY) override;
};

class Queen : public Piece {
public:
    Queen(bool isWhite)
            : Piece(isWhite, QPixmap(isWhite
            ? "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\white-queen.png"
            : "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\black-queen.png")) {}
    bool isValidMove(Board& board, int startX, int startY, int endX, int endY) override;
};

class Rook : public Piece {
public:
    Rook(bool isWhite)
            : Piece(isWhite, QPixmap(isWhite
            ? "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\white-rook.png"
            : "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\black-rook.png")) {}
    bool isValidMove(Board& board, int startX, int startY, int endX, int endY) override;
};

class Bishop : public Piece {
public:
    Bishop(bool isWhite)
            : Piece(isWhite, QPixmap(isWhite
            ? "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\white-bishop.png"
            : "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\black-bishop.png")) {}
    bool isValidMove(Board& board, int startX, int startY, int endX, int endY) override;
};

class Knight : public Piece {
public:
    Knight(bool isWhite)
            : Piece(isWhite, QPixmap(isWhite
            ? "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\white-knight.png"
            : "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\black-knight.png")) {}
    bool isValidMove(Board& board, int startX, int startY, int endX, int endY) override;
};

class Pawn : public Piece {
public:
    Pawn(bool isWhite)
            : Piece(isWhite, QPixmap(isWhite
            ? "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\white-pawn.png"
            : "C:\\Users\\kamil\\CLionProjects\\ChessCpp\\images\\black-pawn.png")),
            hasMoved(false), justDoubleJumped(false){}
    bool isValidMove(Board& board, int startX, int startY, int endX, int endY) override;
    void move();
    bool justDoubleJumped;
    bool getJustDoubleJumped() const;

    void resetJustDoubleJumped();

    void setJustDoubleJumped(bool b);

    bool hasMoved;


    bool getHasMoved();
};

#endif //CHESSCPP_PIECE_H
#endif // PIECE_H
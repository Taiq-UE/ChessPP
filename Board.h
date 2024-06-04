#ifndef CHESSCPP_BOARD_H
#define CHESSCPP_BOARD_H

#include "Piece.h"

class Pawn;
class Piece;

class Board {
private:

    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteRookLeftMoved;
    bool whiteRookRightMoved;
    bool blackRookLeftMoved;
    bool blackRookRightMoved;
    Piece* board[8][8]; // 8x8 chess board
    bool isWhiteTurn;
    bool game_over;
    Piece* previousBoardState[8][8];
    bool isPvP;

public:
    Board();
    bool movePiece(int startX, int startY, int endX, int endY);
    void setPiece(int x, int y, Piece* piece);
    Piece *getPiece(int x, int y);
    void switchTurn();
    void promotePawn(int x, int y);

    bool isCheck(bool isWhite);

    void undoLastMove();

    bool isCheckMate(bool isWhite);

    bool isStalemate(bool isWhite);

    bool isAnyMovePossible(bool isWhite);

    bool isMovePuttingOwnKingInCheck(bool isWhite, int startX, int startY, int endX, int endY);

    bool isOnlyKingsLeft();

    bool findMoveToRemoveCheck(bool isWhite);

    bool canCastle(bool isWhite, bool isKingSide);

    void resetPawnsJustDoubleJumped(Pawn *justMovedPawn);

    void saveToFile(const std::string &filename);

    void loadFromFile(const std::string &filename);

    bool getIsWhiteTurn();

    std::string serialize();

    void deserialize(const std::string &data);

    int evaluate();

    int minimax(Board &board, int depth, bool maximizingPlayer);

    int minimax(int depth, bool maximizingPlayer);

    void makeMove();

    void undoMove(int startX, int startY, int endX, int endY);

    bool isGameOver() const;

    Board(bool isPvP);
};

#endif //CHESSCPP_BOARD_H
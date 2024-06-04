#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "Board.h"

class ChessBoard : public QGraphicsView {
public:
    explicit ChessBoard(bool isPvP, QWidget* parent = nullptr);

    void updateBoard();
    void switchTurn();
    bool getIsWhiteTurn();

    bool movePiece(int startX, int startY, int endX, int endY) {
        return board.movePiece(startX, startY, endX, endY);
    }
    bool isStalemate(bool isWhite) {
        return board.isStalemate(isWhite);
    }

    void saveToFile(const std::string& filename) {
        board.saveToFile(filename);
    }

    void loadFromFile(const std::string& filename) {
        board.loadFromFile(filename);
        isWhiteTurn = getIsWhiteTurn();
        updateBoard();
    }

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QGraphicsScene* scene;
    Board board;
    int startX, startY;
    bool isWhiteTurn;
    bool isPvP;

};

#endif // CHESSBOARD_H
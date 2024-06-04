#include "ChessBoard.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

ChessBoard::ChessBoard(bool isPvP, QWidget* parent) : QGraphicsView(parent), isPvP(isPvP), board(isPvP) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(500, 500);
    scene->setSceneRect(0, 0, 500, 500);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto rect = scene->addRect(i * 50, j * 50, 50, 50);
            if ((i + j) % 2 == 0) {
                rect->setBrush(Qt::lightGray);
            } else {
                rect->setBrush(Qt::darkGray);
            }
        }
    }

    for (int i = 0; i < 8; ++i) {
        auto rowLabel = scene->addText(QString(QChar('A' + i)));
        rowLabel->setPos(i * 50, 400);
        auto colLabel = scene->addText(QString::number(i + 1));
        colLabel->setPos(400, i * 50);
    }

    updateBoard();
}

void ChessBoard::mousePressEvent(QMouseEvent* event) {
    startX = event->x() / 50;
    startY = event->y() / 50;
}

void ChessBoard::mouseReleaseEvent(QMouseEvent* event) {
    int endX = event->x() / 50;
    int endY = event->y() / 50;

    if (board.movePiece(startX, startY, endX, endY)) {
        updateBoard();

        if (!isPvP && !board.getIsWhiteTurn() && !board.isGameOver()) {
            board.makeMove();
            updateBoard();
        }
    }
}

void ChessBoard::updateBoard() {
    scene->clear();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto rect = scene->addRect(i * 50, j * 50, 50, 50);
            if ((i + j) % 2 == 0) {
                rect->setBrush(Qt::lightGray);
            } else {
                rect->setBrush(Qt::darkGray);
            }
        }
    }

    for (int i = 0; i < 8; ++i) {
        auto rowLabel = scene->addText(QString(QChar('A' + i)));
        rowLabel->setPos(i * 50, 400);
        auto colLabel = scene->addText(QString::number(8 - i));
        colLabel->setPos(400, i * 50);
    }

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board.getPiece(i, j);
            if (piece != nullptr) {
                auto pixmapItem = scene->addPixmap(piece->pixmap());
                pixmapItem->setPos(i * 50, j * 50);
            }
        }
    }
}

bool ChessBoard::getIsWhiteTurn() {
    return board.getIsWhiteTurn();
}


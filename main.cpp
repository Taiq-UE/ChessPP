#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "ChessBoard.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMessageBox msgBox;
    msgBox.setText("Choose the game mode");
    QPushButton* pvpButton = msgBox.addButton("Player vs Player", QMessageBox::ActionRole);
    QPushButton* pvbButton = msgBox.addButton("Player vs Bot", QMessageBox::ActionRole);
    msgBox.exec();

    bool isPvP = false;
    if (msgBox.clickedButton() == pvpButton) {
        isPvP = true;
    } else if (msgBox.clickedButton() == pvbButton) {
        isPvP = false;
    }

    ChessBoard chessBoard(isPvP);

    QMessageBox msgBox2;
    msgBox2.setText("Would you like to load a game or start a new one?");
    QPushButton *loadButton = msgBox2.addButton("Load Game", QMessageBox::ActionRole);
    QPushButton *newButton = msgBox2.addButton("New Game", QMessageBox::ActionRole);

    msgBox2.exec();

    if (msgBox2.clickedButton() == loadButton) {
        chessBoard.loadFromFile("C:\\Users\\kamil\\CLionProjects\\ChessCpp\\save.txt");
    } else if (msgBox2.clickedButton() == newButton) {
        chessBoard.show();
    }

    QVBoxLayout* layout = new QVBoxLayout;

    QPushButton* saveButton = new QPushButton("Save");
    layout->addWidget(saveButton);

    QObject::connect(saveButton, &QPushButton::clicked, [&chessBoard]() {
        chessBoard.saveToFile("C:\\Users\\kamil\\CLionProjects\\ChessCpp\\save.txt");
    });

    layout->addWidget(&chessBoard);

    QWidget window;
    window.setLayout(layout);
    window.show();

    return a.exec();
}
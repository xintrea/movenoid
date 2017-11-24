#include <QGraphicsScene>
#include <QLinearGradient>
#include <QBrush>

#include "GameWindow.h"
#include "ui_GameWindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    this->showFullScreen();

    QLinearGradient gradient(0, 0, 0, ui->graphicsView->size().height());
    gradient.setColorAt(0, QColor::fromRgbF(0.87, 0.89, 0.94, 1));
    gradient.setColorAt(1, QColor::fromRgbF(0.69, 0.79, 0.97, 1));
    QBrush brush(gradient);
    ui->graphicsView->setBackgroundBrush(brush);

    gameField=new GameField();
    gameField->addText("Start game");
    ui->graphicsView->setScene(gameField);
}

GameWindow::~GameWindow()
{
    delete gameField;
    delete ui;
}

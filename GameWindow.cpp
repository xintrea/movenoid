#include <QGraphicsScene>
#include <QLinearGradient>
#include <QBrush>
#include <QDebug>

#include "GameWindow.h"
#include "ui_GameWindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    this->showFullScreen();

    // Устанавливается градиент заднего плана
    QLinearGradient gradient(0, 0, 0, ui->graphicsView->size().height());
    gradient.setColorAt(0, QColor::fromRgbF(0.87, 0.89, 0.94, 1));
    gradient.setColorAt(1, QColor::fromRgbF(0.69, 0.79, 0.97, 1));
    QBrush brush(gradient);
    ui->graphicsView->setBackgroundBrush(brush);

    // Задается сцена в виде игрового поля
    gameField=new GameField();
    gameField->setSceneRect(0.0, 0.0, 10.0, 10.0);
    // gameField->addText("Start game");

    ui->graphicsView->setScene(gameField);
    // ui->graphicsView->ensureVisible(0.0, 0.0, 10.0, 10.0, 0, 0);

    // ui->graphicsView->centerOn(2.5, 2.5);

    // ui->graphicsView->ensureVisible(0.0, 0.0, 10.0, 10.0, 0, 0);
    // ui->graphicsView->centerOn(5.0, 5.0);

    ui->graphicsView->fitInView(0.0, 0.0, 5.0, 5.0, Qt::KeepAspectRatio);
    // ui->graphicsView->fitInView(0.0, 0.0, 1.0, 1.0, Qt::KeepAspectRatio);

    connect(gameField, SIGNAL(setScore(int)), this, SLOT(onSetScore(int)));
    connect(gameField, SIGNAL(scoreUp(int)), this, SLOT(onScoreUp(int)));

    connect(gameField, SIGNAL(setLives(int)), this, SLOT(onSetLives(int)));
    connect(gameField, SIGNAL(livesUp()), this, SLOT(onLivesUp()));
    connect(gameField, SIGNAL(livesDn()), this, SLOT(onLivesDn()));

    gameField->runGame();
}

GameWindow::~GameWindow()
{
    delete gameField;
    delete ui;
}


void GameWindow::reject()
{
    qDebug() << "Reject game window";
    QDialog::reject();
}


void GameWindow::onSetScore(const int iScore)
{
    ui->lcdScreenScore->display(iScore);
}


void GameWindow::onScoreUp(const int iScore)
{
    int currScore=ui->lcdScreenScore->intValue();
    ui->lcdScreenScore->display(currScore+iScore);
}


void GameWindow::onSetLives(const int iLives)
{
    ui->lcdScreenLives->display(iLives);
}


void GameWindow::onLivesUp()
{
    int currLives=ui->lcdScreenLives->intValue();
    ui->lcdScreenLives->display(currLives+1);
}


void GameWindow::onLivesDn()
{
    int currLives=ui->lcdScreenLives->intValue();
    ui->lcdScreenLives->display(currLives-1);
}

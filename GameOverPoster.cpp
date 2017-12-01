#include "GameOverPoster.h"
#include "ui_GameOverPoster.h"

GameOverPoster::GameOverPoster(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameOverPoster)
{
    ui->setupUi(this);

    // Убирается рамка окна
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

GameOverPoster::~GameOverPoster()
{
    delete ui;
}

void GameOverPoster::setScore(const int score)
{
    ui->scoreLabel->setText(QString::number(score));
}

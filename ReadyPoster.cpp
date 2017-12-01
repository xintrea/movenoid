#include "ReadyPoster.h"
#include "ui_ReadyPoster.h"

ReadyPoster::ReadyPoster(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadyPoster)
{
    ui->setupUi(this);

    // Убирается рамка окна
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    count=5;
    ui->countDownLabel->setText(QString::number(count));

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateCountDown()));

    runCountDown();
}

ReadyPoster::~ReadyPoster()
{
    delete ui;
}

void ReadyPoster::runCountDown()
{
    updateTimer.start(1000);
}


// Слот, срабатывающий по таймеру updateWorldTimer
void ReadyPoster::updateCountDown()
{
    count--;

    if(count==0)
        this->close();
    else
        ui->countDownLabel->setText(QString::number(count));
}

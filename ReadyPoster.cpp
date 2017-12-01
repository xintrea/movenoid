#include "ReadyPoster.h"
#include "ui_ReadyPoster.h"

ReadyPoster::ReadyPoster(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadyPoster)
{
    ui->setupUi(this);

    // Убирается рамка окна
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    ui->line1Label->hide();
    ui->line2Label->hide();

    count=3;
    ui->countDownLabel->setText(QString::number(count));

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateCountDown()));

    runCountDown();
}


ReadyPoster::~ReadyPoster()
{
    delete ui;
}


void ReadyPoster::setLine1Text(const QString iText)
{
    ui->line1Label->setText(iText);
    ui->line1Label->show();
}


void ReadyPoster::setLine2Text(const QString iText)
{
    ui->line2Label->setText(iText);
    ui->line2Label->show();
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

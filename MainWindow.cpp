#include <QDebug>
#include <iostream>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SettingsWindow.h"
#include "GameWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->gameVersion->setText("v."+
                             QString::number(APPLICATION_RELEASE_VERSION)+"."+
                             QString::number(APPLICATION_RELEASE_SUBVERSION)+"."+
                             QString::number(APPLICATION_RELEASE_MICROVERSION));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onSettingsButtonClicked() const
{
    SettingsWindow settingsWindow;
    settingsWindow.exec();
}


void MainWindow::onPlayButtonClicked() const
{
    GameWindow gameWindow;
    gameWindow.exec();
}

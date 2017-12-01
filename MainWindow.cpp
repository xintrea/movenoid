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

#include <QDebug>
#include <iostream>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SettingsWindow.h"

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

void MainWindow::onSettingsButtonClicked()
{
    SettingsWindow settingsWindow;
    settingsWindow.exec();
}

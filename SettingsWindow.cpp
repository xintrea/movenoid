#include <QDebug>
#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"
#include "AppConfig.h"
#include "main.h"

extern AppConfig appConfig;

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    init();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}


void SettingsWindow::init(void)
{
    ui->captureDeviceFileName->setText( appConfig.getParameter("captureDeviceFileName") );

}


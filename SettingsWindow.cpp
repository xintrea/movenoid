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
    // На экране устанавливается значение, считанное из конфига
    ui->captureDeviceFileName->setText(appConfig.getParameter("captureDeviceFileName"));

    ui->brigthnessTrashholdSlider->setMinimum(0);
    ui->brigthnessTrashholdSlider->setMaximum(100);
    ui->brigthnessTrashholdSlider->setSliderPosition(appConfig.getParameter("brigthnessTrashhold").toInt());

}


// Клик по кнопке Reset to default
void SettingsWindow::onCaptureDeviceDefaultButtonClicked()
{
    ui->captureDeviceFileName->setText("default");
    appConfig.setParameter("captureDeviceFileName", "default");

}


// Клик по кнопке Apply
void SettingsWindow::onCaptureDeviceApplyButtonClicked()
{

}

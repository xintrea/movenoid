#include "CaptureWindow.h"
#include "ui_CaptureWindow.h"

CaptureWindow::CaptureWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptureWindow)
{
    ui->setupUi(this);
}


CaptureWindow::~CaptureWindow()
{
    delete ui;
}


void CaptureWindow::setImage(QImage *img)
{
    int enabledWidth=this->ui->capturePixmapLabel->sizeHint().width();
    this->ui->capturePixmapLabel->setPixmap(QPixmap::fromImage( *img ).scaledToWidth(enabledWidth, Qt::FastTransformation));
}


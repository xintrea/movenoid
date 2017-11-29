#ifndef CAPTUREWINDOW_H
#define CAPTUREWINDOW_H

#include <QDialog>
#include <QImage>

namespace Ui {
class CaptureWindow;
}

class CaptureWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CaptureWindow(QWidget *parent = 0);
    ~CaptureWindow();

    void setImage(QImage *img);

private:
    Ui::CaptureWindow *ui;
};

#endif // CAPTUREWINDOW_H

#ifndef READYPOSTER_H
#define READYPOSTER_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class ReadyPoster;
}

class ReadyPoster : public QDialog
{
    Q_OBJECT

public:
    explicit ReadyPoster(QWidget *parent = 0);
    ~ReadyPoster();

private:
    Ui::ReadyPoster *ui;

private slots:
    void updateCountDown();

protected:

    void runCountDown();

    int count;
    QTimer updateTimer;
};

#endif // READYPOSTER_H

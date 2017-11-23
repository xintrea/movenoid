#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QGraphicsScene>
#include <QDialog>

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

private:
    Ui::GameWindow *ui;

    QGraphicsScene scene;
};

#endif // GAMEWINDOW_H

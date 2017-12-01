#ifndef GAMEOVERPOSTER_H
#define GAMEOVERPOSTER_H

#include <QDialog>

namespace Ui {
class GameOverPoster;
}

class GameOverPoster : public QDialog
{
    Q_OBJECT

public:
    explicit GameOverPoster(QWidget *parent = 0);
    ~GameOverPoster();

    void setScore(const int score);

private:
    Ui::GameOverPoster *ui;
};

#endif // GAMEOVERPOSTER_H

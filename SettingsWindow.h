#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;
    void init(void);

protected:
    QString deviceFileName;

private slots:
    void onCaptureDeviceDefaultButtonClicked();
    void onCaptureDeviceApplyButtonClicked();
};

#endif // SETTINGSWINDOW_H

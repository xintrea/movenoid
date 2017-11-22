#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__

#include <QObject>
#include <QSettings>
#include <QString>


class AppConfig : public QObject
{
    Q_OBJECT

public:
    AppConfig(QObject *pobj=0);
    ~AppConfig();

    QString getParameter(QString name);
    void setParameter(QString name, QString value);

protected:
    QSettings *conf;

};

#endif // __APPCONFIG_H__

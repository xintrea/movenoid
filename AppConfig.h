#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__

#include <QObject>
#include <QSettings>
#include <QString>


// Конфигурация программы (переделать на синглтон)
class AppConfig : public QObject
{
    Q_OBJECT

public:
    AppConfig(QObject *pobj=0);
    ~AppConfig();

    QString getParameter(const QString name) const;
    void setParameter(const QString name, const QString value) const;

protected:
    QSettings *conf;

};

#endif // __APPCONFIG_H__

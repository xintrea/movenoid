#include <QFile>
#include <QDebug>

#include "main.h"
#include "AppConfig.h"


AppConfig::AppConfig(QObject *pobj)
{
    Q_UNUSED(pobj);

    // Создается имя файла конфигурации
    QString configFileName="./config.ini";

    // Проверяется, есть ли файл конфигурации
    QFile configFile(configFileName);
    if(!configFile.open(QIODevice::ReadWrite))
        criticalError("Can't using file "+configFileName);
    configFile.close();

    // Создается указатель на объект хранилища конфигурации
    conf=new QSettings(configFileName, QSettings::IniFormat);
    conf->sync();
}


AppConfig::~AppConfig()
{
    qDebug() << "Save config file before exit";
    conf->sync();
    delete conf;
}


// Получение параметра по имени в виде строки с проверкой его существования
QString AppConfig::getParameter(QString name)
{
    // Если в конфиге есть запрашиваемый параметр
    if(conf->contains(name))
        return conf->value(name).toString();
    else { // Иначе в файле конфигурации небыло нужного параметра
        QString paramName;
        QString paramValue;

        if( name=="captureDeviceFileName" ) {
            paramName=name;
            paramValue="";
        }
        if( name=="brigthnessTrashhold" ) {
            paramName=name;
            paramValue="50";
        }

        // Если запрашиваемый параметр имеет допустимое имя, его надо проинициализировать в файле конфигурации
        if(paramName.length()>0) {
            conf->setValue(paramName, paramValue);
            conf->sync();

            return paramValue;
        } else {
            criticalError("Get unknown parameter: "+name);
        }

    }

    return QString();
}


void AppConfig::setParameter(QString name, QString value)
{
    conf->setValue(name, value);
    conf->sync();
}



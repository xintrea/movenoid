#include <QPoint>
#include <QDebug>

#include "MoveDetector.h"
#include "AppConfig.h"

using namespace std;

extern AppConfig appConfig;

MoveDetector::MoveDetector()
{
    enableBoxMinArea=0.0008333;
    enableBoxMaxArea=0.25;
    enableDispersionBoxBtwContour=0.15;

    enableAspectRatio << 1.0/1.0 << 2.0/1.0 << 10.0/3.0; // Прямоугольники 3x3, 6x3, 10x3
    enableAspectRatioDispersion=0.15;

    // Инициализируется устройство захвата изображения
    captureDevice.init( appConfig.getParameter("captureDeviceFileName") );
    captureDevice.setBrigthnessThreshold( appConfig.getParameter("brigthnessThreshold").toInt() );
}


Marker MoveDetector::detectMarker()
{
    // Этап 1 - Получение упрощенных данных о контурах на картинке, дальше работа только с этими данными
    QVector<ContourData> contoursData=getSimplificatedContourData();
    qDebug() << "Detect object etap1: " << contoursData.size();

    // Этап 2 - удаление контуров со слишком большим или слишком маленьким ограничивающим прямоугольником
    // и контуров у которых разница с ограничивающим прямоугольником слишком большая
    contoursData=removeTooSmallBigCrookedContour(contoursData);
    qDebug() << "Detect object etap2: " << contoursData.size();

    // Этап 3 - удаление контуров с недопустимыми пропорциями
    contoursData=removeBadAspectRatioContour(contoursData);
    qDebug() << "Detect object etap3: " << contoursData.size();

    // Этап 4 - в массиве должны остаться только два самых больших контура, остальные отбрасываются как помехи
    contoursData=removeNoiseContour(contoursData);
    qDebug() << "Detect object etap4: " << contoursData.size();

    // показываем картинки
    // cv::imshow("Binary", bin);
    // cv::imshow("Detect", image);

    Marker marker;
    return marker;
}


// Этап 1
// Метод обрабатывает контуры и извлекает обобщенную информацию - минимальный прямоугольник и площадь контура
QVector<ContourData> MoveDetector::getSimplificatedContourData()
{
    cv::Mat* currentBwFrame=captureDevice.getBwFrame();

    vector<vector<cv::Point> > contours; // Массив для нахождения вершин минимальных прямоугольников
    vector<cv::Vec4i> hierarchy; // Массив иерархии, передается как формальный параметр потому что иерархия в данном случае не используется

    // Поиск контуров
    cv::findContours(currentBwFrame->clone(),
                     contours,
                     hierarchy,
                     CV_RETR_LIST,
                     CV_CHAIN_APPROX_SIMPLE);

    QVector<ContourData> contoursData;

    // Найденные контуры нужно обработать - получить минимальные прямоугольники и площадь под контурами
    // Все контуры верхнего уровня (без иерархии), поэтому обходится только верхний уровень
    // for(unsigned int idx = 0; idx >= 0; idx = hierarchy[idx][0] ) - это код для иерархических контуров
    for(unsigned int idx = 0; idx < contours.size(); idx++)
    {
        cv::Scalar color( rand()&255, rand()&255, rand()&255 );

        // cv::drawContours( image, contours, idx, color, CV_FILLED, 8, hierarchy );

        ContourData data;

        // Для каждого контура определяется минимальный прямоугольник
        vector<cv::Point> points=contours[idx]; // Очередной контур "помещается" в массив points
        data.box=minAreaRect(points); // Определяется минимальный прямоугольник для контура
        data.area=contourArea(points); // Площадь текущего произвольного контура

        contoursData.append(data);
    }

    return contoursData;
}


// Этап 2
QVector<ContourData> MoveDetector::removeTooSmallBigCrookedContour(QVector<ContourData> contoursData)
{
    QMutableVectorIterator<ContourData> iterator(contoursData);
    while(iterator.hasNext()) {
        ContourData currentData=iterator.next();

        // Относительная площадь ограничивающего прямоугольника
        qreal relationArea=( (qreal)(currentData.box.size.width*currentData.box.size.height) )/( (qreal)(captureDevice.getFrameSize().width()*captureDevice.getFrameSize().height()) );

        // Разница между площадью контура и площадью ограничивающего прямоугольника
        qreal dispersion=qAbs(1.0 - ((qreal) currentData.area)/((qreal)(currentData.box.size.width*currentData.box.size.height)));

        if( relationArea<enableBoxMinArea ) {
            qDebug() << "Etap 2. Remove if relationArea" << relationArea << "less than" << "enableBoxMinArea" << enableBoxMinArea;
            iterator.remove(); // Текущий элемент уничтожается
        } else if( relationArea>enableBoxMaxArea ) {
            qDebug() << "Etap 2. Remove if relationArea" << relationArea << "more than" << "enableBoxMaxArea" << enableBoxMaxArea;
            iterator.remove();
        } else if( dispersion > enableDispersionBoxBtwContour ) {
            qDebug() << "Etap 2. Remove if dispersion " << dispersion << "more than" << "enableDispersionBoxBtwContour" << enableDispersionBoxBtwContour;
            iterator.remove();
        }
    }

    return contoursData;
}


// Этап 3
QVector<ContourData> MoveDetector::removeBadAspectRatioContour(QVector<ContourData> contoursData)
{
    QMutableVectorIterator<ContourData> iterator(contoursData);
    while(iterator.hasNext()) {
        ContourData currentData=iterator.next();

        // Соотношение сторон - это ширина к высоте, причем ширина всегда должна быть больше высоты
        qreal w=qMax( (qreal)currentData.box.size.width, (qreal)currentData.box.size.height );
        qreal h=qMin( (qreal)currentData.box.size.width, (qreal)currentData.box.size.height );
        qreal aspectRatio=w/h;

        // Перебираются допустимые соотношения сторон
        bool isFind=false;
        foreach(qreal currentEnableAspectRatio, enableAspectRatio) {
            // Если соотношения сторон соответсвуют (с погрешностью) допустимым
            if(qAbs(currentEnableAspectRatio-aspectRatio) < enableAspectRatioDispersion) {
                isFind=true;
            }
        }

        if(!isFind) {
            qDebug() << "Etap 3. Remove object with bad adpect ratio" << aspectRatio;
            iterator.remove();
        }

    }

    return contoursData;
}


// Этап 4
// Данный метод должен оставить только два самых больших контура
QVector<ContourData> MoveDetector::removeNoiseContour(QVector<ContourData> contoursData)
{
    QMutableVectorIterator<ContourData> iterator(contoursData);
    while(iterator.hasNext()) {
        ContourData currentData=iterator.next();




    }

    return contoursData;
}


// Координаты ракетки
QPointF MoveDetector::getRocketBitPos()
{
    return getFakeRocketBitPos();

}


// Наклон ракетки, в радианах
qreal MoveDetector::getRocketBitAngle()
{
    return getFakeRocketBitAngle();
}


QPointF MoveDetector::getFakeRocketBitPos()
{
    static qreal angleShiftX=0.0;
    static qreal angleShiftY=0.0;
    static qreal anglePhase=0.0;

    angleShiftX+=0.025;
    angleShiftY+=0.015;
    anglePhase+=0.0005;

    qreal x=5.0+sin(angleShiftX+anglePhase)*4.0;
    qreal y=8.0+cos(angleShiftX+anglePhase)*1.5;

    // qDebug() << "RocketBit fake coordinats: " << x << y;

    return QPointF(x, y);
}


qreal MoveDetector::getFakeRocketBitAngle()
{
    static qreal angle=0.0;
    static int direction=1;
    qreal delta=0.01;
    qreal maxAngle=0.5;

    angle=angle+(qreal)direction*delta;

    if(fabs(angle)>maxAngle)
        direction=-direction;

    return angle;
}


#include <QPoint>
#include <QDebug>
#include <math.h>

#include "MoveDetector.h"
#include "AppConfig.h"
#include "main.h"

using namespace std;

extern AppConfig appConfig;

MoveDetector::MoveDetector(QObject *parent) : QObject(parent)
{
    enableDebug=false;

    rocetBitXY=QPointF(5.0, 8.0);
    rocetBitAngle=0.0;

    enableBoxMinArea=0.0008333;
    enableBoxMaxArea=0.25;
    enableDispersionBoxBtwContour=0.25;

    enableAspectRatio << 1.0/1.0 << 2.0/1.0 << 10.0/3.0; // Прямоугольники 3x3, 6x3, 10x3
    enableAspectRatioDispersion=0.46;

    dynamicAngleDispersion=11.0;

    borderSafetyX=0.12;
    borderSafetyY=0.12;

    // Инициализируется устройство захвата изображения
    captureDevice.init( appConfig.getParameter("captureDeviceFileName") );
    captureDevice.setBrigthnessThreshold( appConfig.getParameter("brigthnessThreshold").toInt() );

    debugWindow=nullptr;
    if(enableDebug) {
        debugWindow=new CaptureWindow;
        debugWindow->show();
    }
}


MoveDetector::~MoveDetector()
{
    delete debugWindow;
}


void MoveDetector::run()
{
    for(;;){
        update();
    }
}


void MoveDetector::update()
{
    // Этап 1 - Получение упрощенных данных о контурах на картинке, дальше работа только с этими данными
    QVector<ContourData> contoursData=getSimplificatedContourData();
    if(enableDebug) qDebug() << "Detect object stage 1: " << contoursData.size();

    // Этап 2 - удаление контуров со слишком большим или слишком маленьким ограничивающим прямоугольником
    // и контуров у которых разница с ограничивающим прямоугольником слишком большая
    contoursData=removeTooSmallBigCrookedContour(contoursData);
    if(enableDebug) qDebug() << "Detect object stage 2: " << contoursData.size();

    // Этап 3 - удаление контуров с недопустимыми пропорциями
    contoursData=removeBadAspectRatioContour(contoursData);
    if(enableDebug) qDebug() << "Detect object stage 3: " << contoursData.size();

    // Этап 4 - в массиве должны остаться только два самых больших контура, остальные отбрасываются как помехи
    contoursData=removeNoiseContour(contoursData);
    if(enableDebug) qDebug() << "Detect object stage 4: " << contoursData.size();

    // Метод определяет местоположение и угол маркера на основе его частей и запоминает его как игровые координаты
    detectMarkerLocation( getMarker(contoursData) );
}


// Конструирование маркера, описание его частей
Marker MoveDetector::getMarker(QVector<ContourData> contours)
{
    Marker marker;

    if(contours.size()==0) {
        marker.chunks=0;
        return marker;
    }

    qreal angle;

    // Данные части A
    marker.chunks=1;
    marker.verticlesA=getBoxVertex(contours[0]);
    marker.massCenterA=QPointF(contours[0].box.center.x, contours[0].box.center.y);

    angle=contours[0].box.angle; // Нормализация угла
    if(angle < 0.0)   angle+=360.0;
    if(angle > 360.0) angle-=360.0;
    marker.angleA=angle;

    marker.sizeA=QSizeF(contours[0].box.size.width, contours[0].box.size.height);
    marker.areaA=contours[0].box.size.width*contours[0].box.size.height;
    // marker.boundingBox=contours[0].box.boundingRect();

    if(contours.size()==1)
        return marker;

    // Данные части B
    marker.chunks=2; // Количество частей изменяется на 2. Далее заполняется только часть B, потому что часть A уже заполнена
    marker.verticlesB=getBoxVertex(contours[1]);
    marker.massCenterB=QPointF(contours[1].box.center.x, contours[1].box.center.y);

    angle=contours[1].box.angle; // Нормализация угла
    if(angle < 0.0)   angle+=360.0;
    if(angle > 360.0) angle-=360.0;
    marker.angleB=angle;

    marker.sizeB=QSizeF(contours[1].box.size.width, contours[1].box.size.height);
    marker.areaB=contours[1].box.size.width*contours[1].box.size.height;

    return marker;
}


// Получиение Qt-совместимых координат минимального прямоугольника
QList<QPointF> MoveDetector::getBoxVertex(ContourData contour)
{
     QList<QPointF> v;

     cv::Point2f vertices[4];
     contour.box.points(vertices); // В массив vertices засовываются точки углов минимального прямоугольника
     for (int i = 0; i < 4; i++) { // Из массива vertices значения записываются в итоговый список
         v << QPointF(vertices[i].x, vertices[i].y);
     }

     return v;
}


// Этап 1
// Метод обрабатывает контуры и извлекает обобщенную информацию - минимальный прямоугольник и площадь контура
QVector<ContourData> MoveDetector::getSimplificatedContourData()
{
    cv::Mat* currentBwFrame=captureDevice.getBwFrame();

    vector<vector<cv::Point> > contours; // Массив для нахождения вершин минимальных прямоугольников
    vector<cv::Vec4i> hierarchy; // Массив иерархии, передается как формальный параметр потому что иерархия в данном случае не используется

    // Поиск контуров
    cv::findContours(*currentBwFrame,
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
        ContourData data;

        // Для каждого контура определяется минимальный прямоугольник
        vector<cv::Point> points=contours[idx]; // Очередной контур "помещается" в массив points
        data.box=minAreaRect(points); // Определяется минимальный прямоугольник для контура
        data.area=contourArea(points); // Площадь текущего произвольного контура

        contoursData.append(data);

        if(enableDebug) {
            cv::Scalar color( rand()&255, rand()&255, rand()&255 );

            cv::Point2f vertices[4];
            data.box.points(vertices); // В массив vertices засовываются точки углов минимального прямоугольника
            for (int i = 0; i < 4; i++)
               cv::line(*currentBwFrame, vertices[i], vertices[(i+1)%4], color);
        }
    }

    // Картинка для отладки
    if(enableDebug) {
        QImage img((uchar*)currentBwFrame->data, currentBwFrame->cols, currentBwFrame->rows, currentBwFrame->step, QImage::Format_Grayscale8);
        debugWindow->setImage(&img);
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

        if( relationArea<enableBoxMinArea ) { // Объект очень маленький
            // qDebug() << "Stage 2. Remove if relationArea" << relationArea << "less than" << "enableBoxMinArea" << enableBoxMinArea;
            iterator.remove(); // Текущий элемент уничтожается
        } else if( relationArea>enableBoxMaxArea ) { // Очень большой
            // qDebug() << "Stage 2. Remove if relationArea" << relationArea << "more than" << "enableBoxMaxArea" << enableBoxMaxArea;
            iterator.remove();
        } else if( dispersion > enableDispersionBoxBtwContour ) { // Площадь контура далека от площади ограничивающего прямоугольника
            // qDebug() << "Stage 2. Remove if dispersion " << dispersion << "more than" << "enableDispersionBoxBtwContour" << enableDispersionBoxBtwContour;
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
            if(enableDebug) qDebug() << "Stage 3. Remove object with bad adpect ratio" << aspectRatio;
            iterator.remove();
        }

    }

    return contoursData;
}


// Этап 4
// Данный метод должен оставить только два самых больших контура, остальное считается шумом
QVector<ContourData> MoveDetector::removeNoiseContour(QVector<ContourData> contoursData)
{
    // Сортировка по убыванию
    qSort(contoursData.begin(), contoursData.end(), contourMoreThan);

    // foreach(ContourData contourData, contoursData) {
    //     qDebug() << "Stage 4. Sorted area:" << contourData.area;
    // }

    // Оставляется только два элемента
    if(contoursData.size()>2)
        contoursData.remove(2, contoursData.size()-2);

    return contoursData;
}


// Для обычной сортировки от малых значений к большим
bool MoveDetector::contourLessThan(const ContourData &c1, const ContourData &c2)
{
    return c1.area < c2.area;
}


// Для обратной сортировки от больших значений к малым
bool MoveDetector::contourMoreThan(const ContourData &c1, const ContourData &c2)
{
    return c1.area > c2.area;
}


void MoveDetector::detectMarkerLocation(Marker marker)
{
    static Marker previousMarker=marker;

    if(enableDebug) qDebug() << "Marker chunks: " << marker.chunks;

    if(marker.chunks==1) {
        // Приведение координат к виртуальному экрану
        QPointF centerMass=convertToSafetyCoord( QPointF(marker.massCenterA.x(), marker.massCenterA.y()) );

        // Приведение координат к игровому полю
        qreal x=centerMass.x()*10.0/(qreal)captureDevice.getFrameSize().width(); // Где 10.0 - это размер игры, заменить на дефайн
        qreal y=centerMass.y()*10.0/(qreal)captureDevice.getFrameSize().height();
        rocetBitXY=QPointF(x, y);

        // Если происходит переключение с двух видимых частей маркера на одну часть,
        // предполагается, что угол на двух частях был определен точно, и угол поворота одной части
        // надо подобрать с шагом 90 градусов так, чтобы он примерно соответствовал предыдущему значению
        // if(previousMarker.chunks==2) {
        // }

        // Угол подбирается так, чтобы он был ближайшим к предыдущему значению
        // В момент вызова объект еще хранит предыдущее значение угла
        qreal previousAngle=rocetBitAngle;
        rocetBitAngle=selectNearestAngle(marker.angleA, previousAngle, dynamicAngleDispersion);

        if(enableDebug) qDebug() << "Marker angle: " << marker.angleA << "prev:" << previousAngle << "disp:" << dynamicAngleDispersion << "res:" << rocetBitAngle;

    } else if(marker.chunks==2) {
        qreal xA=marker.massCenterA.x();
        qreal xB=marker.massCenterB.x();
        qreal yA=marker.massCenterA.y();
        qreal yB=marker.massCenterB.y();

        qreal xMass=(xA+xB)/2.0;
        qreal yMass=(yA+yB)/2.0;

        // Приведение координат к виртуальному экрану
        QPointF centerMass=convertToSafetyCoord( QPointF(xMass, yMass) );

        // Приведение координат к игровому полю
        qreal x=centerMass.x()*10.0/(qreal)captureDevice.getFrameSize().width(); // Где 10.0 - это размер игры, заменить на дефайн
        qreal y=centerMass.y()*10.0/(qreal)captureDevice.getFrameSize().height();

        rocetBitXY=QPointF(x, y);

        rocetBitAngle=getAngleByPoints(xA, yA, xB, yB);
    }

    previousMarker=marker;
}


QPointF MoveDetector::convertToSafetyCoord(QPointF iPos)
{
    qreal x=iPos.x();
    qreal y=iPos.y();

    qreal borderPixX=(qreal)captureDevice.getFrameSize().width()*borderSafetyX;
    qreal borderPixY=(qreal)captureDevice.getFrameSize().height()*borderSafetyY;

    qreal newScrSizeX=(qreal)captureDevice.getFrameSize().width()-borderPixX*2.0;
    qreal newScrSizeY=(qreal)captureDevice.getFrameSize().height()-borderPixY*2.0;

    // Координаты точки в уменьшенном прямоугольнике
    x=x-borderPixX;
    y=y-borderPixY;

    // Нормальзация
    if(x<0.0)x=0.0;
    if(x>newScrSizeX)x=newScrSizeX;
    if(y<0.0)y=0.0;
    if(y>newScrSizeY)y=newScrSizeY;

    // Получение нормированных на единицу координат
    x=x/newScrSizeX;
    y=y/newScrSizeY;

    // Получение координат в размере захватываемого фрейма
    x=x*(qreal)captureDevice.getFrameSize().width();
    y=y*(qreal)captureDevice.getFrameSize().height();

    return QPointF(x, y);
}


// Координаты ракетки
QPointF MoveDetector::getRocketBitPos()
{
    // return getFakeRocketBitPos();
    return rocetBitXY;
}


// Наклон ракетки, в градусах
qreal MoveDetector::getRocketBitAngle()
{
    // return getFakeRocketBitAngle();
    return rocetBitAngle;
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


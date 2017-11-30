#ifndef MOVEDETECTOR_H
#define MOVEDETECTOR_H

#include <QObject>
#include <QPointF>
#include <QRectF>
#include "opencv2/opencv.hpp"
#include "CaptureDevice.h"
#include "CaptureWindow.h"

// Итоговые сведения об обнаруженном маркере
// Физически маркер состоит из двух белых прямоугольников
// Первый - квадрат 3x3, второй - прямоугольник 6x3. Между ними расстояние 1.
// Общая площадь этих двух прямоугольников с учетом расстояния между ними 10x3
// Размеры даны в условных единицах, реально маркер можно сделать в сантиметрах и размещать его в 50-70 см. от камеры
struct Marker{

    int chunks; // Количество частей
                // 0 - нет маркера
                // 1 - одна большая часть A (10:3) - для движущегося смазанного и потому "слипшегося" изображения
                // 2 - две части A и B (1:1 и 2:1) - для четкого изображения
    QList<QPointF> verticlesA; // Вершины ограничивающего прямоугольника A
    QList<QPointF> verticlesB; // Вершины ограничивающего прямоугольника B

    QPointF massCenterA;
    QPointF massCenterB;

    qreal angleA;
    qreal angleB;

    QSizeF sizeA; // Размер ограничивающего прямоугольника
    QSizeF sizeB;

    qreal areaA; // Площадь ограничивающего прямоугольника (чтобы повторно не высчитывать)
    qreal areaB;

    QRectF boundingRect; // Ограничивающий прямоугольник с вертикальными и горизонтальными гранями
};


struct ContourData {
    cv::RotatedRect box; // Ограничивающий прямоугольник
    qreal area; // Площадь произвольного контура
};


class MoveDetector : public QObject
{
    Q_OBJECT

public:
    explicit MoveDetector(QObject *parent = 0);
    virtual ~MoveDetector();

public slots:
    void run(); // Метод, используемый для запуска в треде
    QPointF getRocketBitPos();
    qreal getRocketBitAngle();
    void doExit();

signals:
    void finished(); // Сигнал о реальном завершении работы (метод run() останавливается и происходит выход из него)

protected:
    void update();
    QVector<ContourData> getSimplificatedContourData();
    QVector<ContourData> removeTooSmallBigCrookedContour(QVector<ContourData> contoursData);
    QVector<ContourData> removeBadAspectRatioContour(QVector<ContourData> contoursData);
    QVector<ContourData> removeNoiseContour(QVector<ContourData> contoursData);

    static bool contourLessThan(const ContourData &c1, const ContourData &c2);
    static bool contourMoreThan(const ContourData &c1, const ContourData &c2);

    static QList<QPointF> getBoxVertex(ContourData contour);
    static Marker getMarker(QVector<ContourData> contours);
    void detectMarkerLocation(Marker marker);
    QPointF convertToSafetyCoord(QPointF iPos);

    QPointF getFakeRocketBitPos();
    qreal getFakeRocketBitAngle();

    QPointF rocetBitXY;
    qreal rocetBitAngle;

    CaptureDevice captureDevice;

    // Значения в процентах от площади картинки, где 1% записывается как 0.01
    // Например, для картинки с камеры 640x480 pix минимальный распознаваемый объект будет примерно 16x16 pix
    // 640x480=307200, 16x16=256, значит размер такого объекта относительно площади картинки будет 256/307200=0.0008333
    qreal enableBoxMinArea; // Граничное значение для минимально допустимой площади ограничивающего прямоугольника
    qreal enableBoxMaxArea; // Граничное значение для максимально допустимой площади ограничивающего прямоугольника

    qreal enableDispersionBoxBtwContour; // Допустимая разница в площади между ограничивающим прямоугольником и контуром
                                         // Если площади абсольно равны, отношение их площадей 1
                                         // Пример: площадь A=100, площадь B=115. Отношение 100/115=0.86956
                                         // Разница площади fabs(1 - 0.86956)=0.13043 - это значение и проверяется

    QList<qreal> enableAspectRatio; // Допустимые соотношения сторон для ограничивающих прямоугольников
    qreal enableAspectRatioDispersion; // Допустимая погрешность при проверке соотношения сторон ограничивающих прямоугольников

    qreal dynamicAngleDispersion; // Величина для подбора угла на основе предыдущего значения,
                                  // с учетом того, что opencv может дать значение кратное 90

    qreal borderSafetyX; // Границы безопасности от края кадра, за которыми не должно срабатывать распознавание маркера
    qreal borderSafetyY;

    CaptureWindow *debugWindow;

    bool enableDebug;
    bool exitFlag;

};

#endif // MOVEDETECTOR_H

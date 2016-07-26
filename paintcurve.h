#ifndef PAINTCURVE
#define PAINTCURVE
#include <QWidget>
//#include <mainwindow.h>
class PaintCurve:public QWidget
{
  Q_OBJECT
public:
    explicit PaintCurve(QWidget *parent);
    void getPainterValue(QPainterPath path,QList<QPointF> points,int HCoordinateTemp, int VCoordinateTemp);
    void getPainterValueForSpeed(QPainterPath path,QList<QPointF> points);
    void getPainterValueForLiqud(QPainterPath path,QList<QPointF> points);
    void getPainterValueForSpeedTorque(QPainterPath path, QList<QPointF> points);
    void getPainterValueForSpeedRatio(QPainterPath path, QList<QPointF> points);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QList<QPointF> knots;
    QPainterPath nonSmoothCurve;
    QPainterPath smoothCurve;
    int HCoordinate,VCoordinate;
    QList<QPointF> currentPointList;
    QString QStrPaintCurve;
    int Color_FLAG;
    int PAINT_CALLING_COUNT_FLAG;
    QList< QList<QPointF> > ListForList;
    QList<QPainterPath> smoothCurvePathList;

private slots:
    void showNonSmoothCurve();
    void showSmoothCurve();

};



#endif // PAINTCURVE


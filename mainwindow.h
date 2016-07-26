#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <paintcurve.h>
#include <QMainWindow>
#include <smoothcurvecreator.h>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initSubTableItem();
    void calculateEfficicy();
    QList<float> calculateGaugedEfficicy(QList<float> Speed,QList<float> Torque);
    void getTableItemValue();
    void updateTableWidget(QList<float> gorguedEfficicy,QList<float> powerEfficicy,QList<float> ratioEfficicy);
    QPainterPath getActualCoordinateValue(QList<float> HCoordinate,QList<float> VCoordinate);
    int getHCoordinateMaxValue(float X);//获取横坐标最大值
    int getVCoordinateMaxValue(float Y);//获取纵坐标最大值
private slots:
    void on_addItemBtn_clicked();

    void on_setDatabaseBtn_clicked();

    void on_saveLineBtn_clicked();

    void on_clearDataBtn_clicked();



    void on_TorqueEBtn_clicked();

    void on_speedEBtn_clicked();

    void on_liqidEBtn_clicked();

    void on_saveLocalBtn_clicked();

    void on_inPressBtn_clicked();



    void on_gorguEBtn_clicked();

    void on_SpeedTorqueBtn_clicked();

    void on_SpeedRatioBtn_clicked();

private:
    Ui::MainWindow *ui;
    bool isDatabaseOK;
    QSqlQuery *query;
    PaintCurve *paintcurve;
    QList<QPointF> pointFEfficicy;
    SmoothCurveCreator *smoothCurveCreator;
    QPainterPath curve_Efficicy_Path;
    QList<float> inPress,outPress,Speed,Torque,Liquid;//存tablewidget五个项目值的list
    QList<float> gorguEfficicy,powerEfficicy,ratioEfficicy;//存计算后测量功率，水力功率，效率list
    float tempHC,tempVC;
    QList<QPointF> QCurrentPointList;
    QString MainWindowsDebug;
};

#endif // MAINWINDOW_H

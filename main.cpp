#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    w.show();
    w.setFixedSize(w.width(),w.height());//设置固定大小
    w.setWindowTitle(QObject::tr("水涡轮参数监测系统"));//设置窗体名称
    qDebug()<<"Available drivers:";
    QStringList drivers=QSqlDatabase::drivers();
    foreach (QString driver,drivers)
    {
      qDebug()<<driver;
    }

    return a.exec();
}

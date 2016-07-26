#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFont>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString btn_style="border:2px groove gray outset;border-radius:10px;border-style:outset;";
    QString Combo_style="border:1px black;border-radius:4px;border-style:outset;";
    MainWindowsDebug="MainWindowsDebug";
    paintcurve=new PaintCurve(ui->widget);

    ui->addItemBtn->setStyleSheet(btn_style);
    ui->clearDataBtn->setStyleSheet(btn_style);
    ui->deleteLineBtn->setStyleSheet(btn_style);
    ui->liqidEBtn->setStyleSheet(btn_style);
    ui->openLocalBtn->setStyleSheet(btn_style);
    ui->gorguEBtn->setStyleSheet(btn_style);
    ui->saveLocalBtn->setStyleSheet(btn_style);
    ui->speedEBtn->setStyleSheet(btn_style);
    ui->TorqueEBtn->setStyleSheet(btn_style);
    ui->widgetOnFrame->setStyleSheet("border:5px gray;border-radius:20px;background-color:white;");
    initSubTableItem();

    ui->serialGroupBox->setStyleSheet("border:20px black;border-radius:20px;background-color:light gray");
    ui->serialCombo->setStyleSheet(Combo_style);
    ui->boundCombo->setStyleSheet(Combo_style);
    ui->textBitCombo->setStyleSheet(Combo_style);
    ui->stopBitCombo->setStyleSheet(Combo_style);
    ui->dataBitCombo->setStyleSheet(Combo_style);
    ui->searchSerialBtn->setStyleSheet(btn_style);
    ui->switchBtn->setStyleSheet(btn_style);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initSubTableItem()
{
    QStringList VHeader1,VHeader2,HHeader;
    VHeader1<<QObject::trUtf8("入口压力(KPa)")<<QObject::trUtf8("出口压力(KPa)")<<QObject::trUtf8("流量(m3/s)")<<QObject::trUtf8("转矩(N.m)");
    VHeader2<<trUtf8("转速(r/min)")<<trUtf8("测量功率(w)")<<trUtf8("水利功率(w)")<<trUtf8("效率");
    HHeader<<trUtf8("当前值");

    ui->data1tableWidget->setColumnCount(1);
    ui->data1tableWidget->setRowCount(4);
    ui->data1tableWidget->horizontalHeader()->setDefaultSectionSize(130);
    ui->data1tableWidget->verticalHeader()->setDefaultSectionSize(45);
    ui->data1tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->data1tableWidget->setHorizontalHeaderLabels(HHeader);
    ui->data1tableWidget->setVerticalHeaderLabels(VHeader1);


    ui->data2tableWidget->setColumnCount(1);
    ui->data2tableWidget->setRowCount(4);
    ui->data2tableWidget->horizontalHeader()->setDefaultSectionSize(130);
    ui->data2tableWidget->verticalHeader()->setDefaultSectionSize(45);
    ui->data2tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->data2tableWidget->setHorizontalHeaderLabels(HHeader);
    ui->data2tableWidget->setVerticalHeaderLabels(VHeader2);

}


void MainWindow::calculateEfficicy()
{

  QPointF currentPointF;
  float currentX,currentY;
  float itemX,itemY;
  int row_Count=ui->tableWidget->rowCount();
  for(int i=0;i<row_Count;i++)
   {
      itemX=ui->tableWidget->item(i,5)->text().toFloat();
      itemY=ui->tableWidget->item(i,6)->text().toFloat();
      qDebug()<<"itemY="<<itemY<<"itemX="<<itemX;
      currentX=itemX;
      currentY=(currentX/itemY)*250.0;
      currentPointF.setX((ui->tableWidget->item(i,2)->text().toInt())*25);
      currentPointF.setY(currentY);
      pointFEfficicy<<currentPointF;
      qDebug()<<"CurrentPointF="<<currentPointF.x()<<","<<currentPointF.y();
      ui->tableWidget->setItem(i,7,new QTableWidgetItem(QString::number(currentX/itemY)));
   }

}


QList<float> MainWindow::calculateGaugedEfficicy(QList<float> Speed, QList<float> Torque)
{ qDebug()<<"----calculateGaugedEfficicy----";
  int length=Speed.size();
  QList<float> result;
  for(int i=0;i<length;i++)
  {
      result[i]=Speed[i]*Torque[i]/9.55;
  }
   return result;
}

//获取TableWidget所有实际行之前五项实际值
//获取压力、流量、转速、转矩实际值
void MainWindow::getTableItemValue()
{
  qDebug()<<"----getTableItemValue----";
  int sum=ui->tableWidget->rowCount();
  qDebug()<<"sum="<<sum;
  inPress.clear();
  outPress.clear();
  Speed.clear();
  Torque.clear();
  Liquid.clear();
  for(int i=0;i<sum;i++)
  {  qDebug()<<"i="<<i;
     inPress.append(ui->tableWidget->item(i,2)->text().toFloat());
     outPress.append(ui->tableWidget->item(i,3)->text().toFloat());
     Speed.append(ui->tableWidget->item(i,5)->text().toFloat());
     Torque.append(ui->tableWidget->item(i,4)->text().toFloat());
     Liquid.append(ui->tableWidget->item(i,1)->text().toFloat());

  }
}


//更新列表后三项
void MainWindow::updateTableWidget(QList<float> gorguedEfficicy, QList<float> powerEfficicy,QList<float> ratioEfficicy)
{ qDebug()<<"----updataTableWidget----";
   int row=ui->tableWidget->rowCount();
   for(int i=0;i<row;i++)
   {
      ui->tableWidget->setItem(i,6,new QTableWidgetItem(QString::number(gorguedEfficicy[i])));
      ui->tableWidget->setItem(i,7,new QTableWidgetItem(QString::number(powerEfficicy[i])));
      ui->tableWidget->setItem(i,8,new QTableWidgetItem(QString::number(ratioEfficicy[i])));
   }
}


//获取实际坐标点的值，并生成path
QPainterPath MainWindow::getActualCoordinateValue(QList<float> HCoordinate, QList<float> VCoordinate)
{   qDebug()<<"----getActualCoordinateValue----";
    int HCoordinateSize=HCoordinate.size();
    int VCoordinateSize=VCoordinate.size();

    qDebug()<<MainWindowsDebug<<HCoordinate;
    qDebug()<<MainWindowsDebug<<VCoordinate;

    //获得heng坐标中的最大值tempHC
    for(int i=0;i<HCoordinateSize;i++)
    {
       if(i==0) tempHC=HCoordinate.at(i);
       else {
           if(HCoordinate.at(i)>tempHC)
               tempHC=HCoordinate.at(i);

       }
    }
     qDebug()<<MainWindowsDebug<<"The biggest HC is"<<tempHC;
    //获得纵坐标中的最大值tempVC
    for(int i=0;i<VCoordinateSize;i++)
    {
       if(i==0) tempVC=VCoordinate.at(0);
       else {
           if(VCoordinate.at(i)>tempVC)
               tempVC=VCoordinate.at(i);
       }
    }
   qDebug()<<MainWindowsDebug<<"The Biggest VC is"<<tempVC;
   QCurrentPointList.clear();
    //获取实际坐标点
    for(int i=0;i<HCoordinateSize;i++)
    { QPointF currentPoint;
      int currentX,currentY;
      currentX=(HCoordinate.at(i)/tempHC)*250.0;
      currentY=(VCoordinate.at(i)/tempVC)*250.0;
      currentPoint.setX(currentX);
      currentPoint.setY(currentY);
      qDebug()<<"MainWindows->getActualCoordinateValue::"<<currentPoint;
      QCurrentPointList<<currentPoint;

    }

    QPainterPath currentPainterPath;
    currentPainterPath=SmoothCurveCreator::createSmoothCurve(QCurrentPointList);
    return currentPainterPath;
}

int MainWindow::getHCoordinateMaxValue(float X)
{   int temp;
    X=tempHC;
   temp=int(X);
   temp=temp+(10-(temp)%10);
   return temp;
}

int MainWindow::getVCoordinateMaxValue(float Y)
{  int temp;
   Y=tempVC;
   temp=int(Y);
   temp=temp+10-(temp)%10;
   return temp;
}




void MainWindow::on_addItemBtn_clicked()
{
  int i=0,row_Count;
  int sixth,seventh,eighth;
  row_Count=ui->tableWidget->rowCount();
  qDebug()<<"row="<<row_Count;
  ui->tableWidget->insertRow(row_Count);
    for(i=0;i<5;i++)
  {
    ui->tableWidget->setItem(row_Count,i,new QTableWidgetItem(QString::number(row_Count)));
    if(i==4)
    {
       sixth=ui->tableWidget->item(row_Count,0)->text().toInt()*15;
       seventh=ui->tableWidget->item(row_Count,1)->text().toInt()*20;
       ui->tableWidget->setItem(row_Count,5,new QTableWidgetItem(QString::number(sixth)));
       ui->tableWidget->setItem(row_Count,6,new QTableWidgetItem(QString::number(seventh)));
    }
  }
}



void MainWindow::on_setDatabaseBtn_clicked()
{
  bool isOK;
  QString text=QInputDialog::getText(NULL,tr("请输入数据库名称："),tr("请输入数据库名称："),QLineEdit::Normal,tr("xxx.db"),&isOK);
  if(isOK)
  {
     qDebug()<<"The Database Name Is :"<<text;
     QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
     db.setDatabaseName(text);
     isDatabaseOK=db.open();
     query=new QSqlQuery(db);
     if(!isDatabaseOK)
    {QMessageBox::critical(this,tr("数据库无法打开"),tr("数据库无法打开"),QMessageBox::Cancel);}
     else{
         qDebug()<<"isDataBaseOK=="<<isDatabaseOK;
         query->exec("create table TABLE(id int primary key,"
                 "PANTON int)");
                // "LIULIANG int,"
                //"OUTPRESS int,"
                // "TORQUE int,"
                // "SPEED int)") ;
       //  QStringList tablelist=QSqlDatabase::tables();
         }
    }
}



void MainWindow::on_saveLineBtn_clicked()
{
 int DRow,DColumn;

 DRow=ui->tableWidget->rowCount();
 DColumn=ui->tableWidget->columnCount();
 qDebug()<<"DRow="<<DRow;
 for(int i=0;i<DRow;i++)
  {
   qDebug()<<"Execute query statement";
    int PanValue,LiuValue,OutValue,TorValue,SpeedValue;
    //  query->prepare("insert into TABLE(PANTON,LIULIANG,OUTPRESS,TORQUE,SPEED) VALUES(:PANTON,:LIULIANG,:OUTPRESS,:TORQUE,:SPEED)");
    // query->prepare("insert into TABLE(PANTON) values(:PANTON)");
    PanValue=ui->tableWidget->item(i,0)->text().toInt();
    // LiuValue=ui->tableWidget->item(i,1)->text().toInt();
    // OutValue=ui->tableWidget->item(i,2)->text().toInt();
    // TorValue=ui->tableWidget->item(i,3)->text().toInt();
    // SpeedValue=ui->tableWidget->item(i,4)->text().toInt();
    // query->bindValue(":PANTON",PanValue);
    // query->bindValue(":LIULIANG",LiuValue);
    // query->bindValue(":OUTPRESS",OutValue);
    // query->bindValue(":TORQUE",TorValue);
    // query->bindValue(":SPEED",SpeedValue);
    // query->exec();
     query->exec("insert into TABLE(PANTON) values(100)");
 }
}



void MainWindow::on_clearDataBtn_clicked()
{
 emit ui->tableWidget->clear();
}




void MainWindow::on_TorqueEBtn_clicked()
{  QFont ft;
   ft.setPointSize(15);
   ui->curveDisNameLabel->setText(tr("转矩-效率曲线"));
   ui->curveDisNameLabel->setFont(ft);
   ui->curveDisNameLabel->setAlignment(Qt::AlignHCenter);
}


//生成速度--测量功率曲线
void MainWindow::on_speedEBtn_clicked()
{   QFont ft;
    ft.setPointSize(15);
    ui->curveDisNameLabel->setText(tr("转速-效率曲线"));
    ui->curveDisNameLabel->setFont(ft);
    ui->curveDisNameLabel->setAlignment(Qt::AlignHCenter);

    getTableItemValue();
    gorguEfficicy.clear();//清空测量功率列表
    powerEfficicy.clear();//清空水力功率列表
    int sum=ui->tableWidget->rowCount();//获取table表总行数
    //qDebug()<<"----测量效率----for before----";
    for(int i=0;i<sum;i++)
    {
       gorguEfficicy<<(Speed[i]*Torque[i]/9.55);//测量功率
       powerEfficicy<<((inPress[i]-outPress[i])*Liquid[i]/3.6);//水力功率
       ratioEfficicy<<(gorguEfficicy.at(i)/powerEfficicy.at(i));//效率
       //qDebug()<<"xiaoLV="<<ratioEfficicy.at(i);
       //qDebug()<<"测量功率："<<gorguEfficicy.at(i);
    }

   // qDebug()<<"----测量效率-----for after---";
    //updateTableWidget(gorguEfficicy,powerEfficicy,ratioEfficicy);//生成测量、水力、效率table后三项

    QPainterPath path;
    //获取路径
    //path=getActualCoordinateValue(inPress,gorguEfficicy);//生成入口压力--测量效率曲线
    path=getActualCoordinateValue(Speed,gorguEfficicy);

      //直接绘图
    paintcurve->getPainterValueForSpeed(path,QCurrentPointList);

}




void MainWindow::on_liqidEBtn_clicked()
{   QFont ft;
    ft.setPointSize(15);
    ui->curveDisNameLabel->setText(tr("流量-效率曲线"));
    ui->curveDisNameLabel->setFont(ft);//设置Label大小
    ui->curveDisNameLabel->setAlignment(Qt::AlignHCenter);

    getTableItemValue();
    gorguEfficicy.clear();
    powerEfficicy.clear();
    ratioEfficicy.clear();

    int sum=ui->tableWidget->rowCount();
    //qDebug()<<"----测量效率----for before----";
    for(int i=0;i<sum;i++)
    {
       gorguEfficicy<<(Speed[i]*Torque[i]/9.55);//测量功率
       powerEfficicy<<((inPress[i]-outPress[i])*Liquid[i]/3.6);//水力功率
       ratioEfficicy<<(gorguEfficicy.at(i)/powerEfficicy.at(i));//效率
       //qDebug()<<"xiaoLV="<<ratioEfficicy.at(i);
       //qDebug()<<"测量功率："<<gorguEfficicy.at(i);
    }

    //qDebug()<<"----测量效率-----for after---";
    //updateTableWidget(gorguEfficicy,powerEfficicy,ratioEfficicy);

    QPainterPath path;
    //获取路径
    path=getActualCoordinateValue(Liquid,gorguEfficicy);//生成入口压力--测量效率曲线
    //path=getActualCoordinateValue(Speed,gorguEfficicy);

      //直接绘图
    paintcurve->getPainterValue(path,QCurrentPointList,getHCoordinateMaxValue(tempHC),getVCoordinateMaxValue(tempVC));


}




void MainWindow::on_saveLocalBtn_clicked()
{
    calculateEfficicy();
    curve_Efficicy_Path=SmoothCurveCreator::createSmoothCurve(pointFEfficicy);
    //paintcurve->getPainterValue(curve_Efficicy_Path);

}



void MainWindow::on_inPressBtn_clicked()
{

}



//生成测量效率，水力效率，效率并绘制曲线
void MainWindow::on_gorguEBtn_clicked()
{     qDebug()<<"----测量效率----";
      QFont ft;
      ft.setPointSize(15);
      ui->curveDisNameLabel->setText(tr("入口压力-测量功率"));
      ui->curveDisNameLabel->setFont(ft);
      ui->curveDisNameLabel->setAlignment(Qt::AlignHCenter);//文本对齐

      getTableItemValue();
      gorguEfficicy.clear();
      powerEfficicy.clear();
      int sum=ui->tableWidget->rowCount();
      //qDebug()<<"----测量效率----for before----";
      for(int i=0;i<sum;i++)
      {
         gorguEfficicy<<(Speed[i]*Torque[i]/9.55);//测量功率
         powerEfficicy<<((inPress[i]-outPress[i])*Liquid[i]/3.6);//水力功率
         ratioEfficicy<<(gorguEfficicy.at(i)/powerEfficicy.at(i));//效率
         //qDebug()<<"xiaoLV="<<ratioEfficicy.at(i);
         //qDebug()<<"测量功率："<<gorguEfficicy.at(i);
      }

      //qDebug()<<"----测量效率-----for after---";
      updateTableWidget(gorguEfficicy,powerEfficicy,ratioEfficicy);

      QPainterPath path;
      //获取路径
      path=getActualCoordinateValue(inPress,gorguEfficicy);//生成入口压力--测量效率曲线
      //path=getActualCoordinateValue(Speed,gorguEfficicy);

        //直接绘图
      paintcurve->getPainterValue(path,QCurrentPointList,getHCoordinateMaxValue(tempHC),getVCoordinateMaxValue(tempVC));
}




void MainWindow::on_SpeedTorqueBtn_clicked()
{
          qDebug()<<"----转速--转矩----";
          QFont ft;
          ft.setPointSize(15);
          ui->curveDisNameLabel->setText(tr("转矩--转速曲线"));
          ui->curveDisNameLabel->setFont(ft);
          ui->curveDisNameLabel->setAlignment(Qt::AlignHCenter);//文本对齐


          getTableItemValue();
          gorguEfficicy.clear();
          powerEfficicy.clear();
          int sum=ui->tableWidget->rowCount();
          //qDebug()<<"----测量效率----for before----";
          for(int i=0;i<sum;i++)
          {
             gorguEfficicy<<(Speed[i]*Torque[i]/9.55);//测量功率
             powerEfficicy<<((inPress[i]-outPress[i])*Liquid[i]/3.6);//水力功率
             ratioEfficicy<<(gorguEfficicy.at(i)/powerEfficicy.at(i));//效率
             //qDebug()<<"xiaoLV="<<ratioEfficicy.at(i);
             //qDebug()<<"测量功率："<<gorguEfficicy.at(i);
          }

          //qDebug()<<"----测量效率-----for after---";
          //updateTableWidget(gorguEfficicy,powerEfficicy,ratioEfficicy);

          QPainterPath path;
          //获取路径
          path=getActualCoordinateValue(Speed,Torque);//生成入口压力--测量效率曲线
          //path=getActualCoordinateValue(Speed,gorguEfficicy);

            //直接绘图
          paintcurve->getPainterValueForSpeedTorque(path,QCurrentPointList);


}

void MainWindow::on_SpeedRatioBtn_clicked()
{
    qDebug()<<"----转速--效率----";
    QFont ft;
    ft.setPointSize(15);
    ui->curveDisNameLabel->setText(tr("转矩--效率曲线"));
    ui->curveDisNameLabel->setFont(ft);
    ui->curveDisNameLabel->setAlignment(Qt::AlignHCenter);//文本对齐


    getTableItemValue();
    gorguEfficicy.clear();
    powerEfficicy.clear();
    int sum=ui->tableWidget->rowCount();
    //qDebug()<<"----测量效率----for before----";
    for(int i=0;i<sum;i++)
    {
       gorguEfficicy<<(Speed[i]*Torque[i]/9.55);//测量功率
       powerEfficicy<<((inPress[i]-outPress[i])*Liquid[i]/3.6);//水力功率
       ratioEfficicy<<(gorguEfficicy.at(i)/powerEfficicy.at(i));//效率

    }

    //qDebug()<<"----测量效率-----for after---";
    //updateTableWidget(gorguEfficicy,powerEfficicy,ratioEfficicy);

    QPainterPath path;
    //获取路径
    path=getActualCoordinateValue(Speed,ratioEfficicy);//生成入口压力--测量效率曲线
    //path=getActualCoordinateValue(Speed,gorguEfficicy);

      //直接绘图
    paintcurve->getPainterValueForSpeedRatio(path,QCurrentPointList);
}

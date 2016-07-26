#include <paintcurve.h>
#include <QtGui>
#include <QWidget>
#include <QDateTime>
#include <smoothcurvecreator.h>
PaintCurve::PaintCurve(QWidget *parent)
  :QWidget(parent)
{
  QTimer *timer=new QTimer(this);
  QObject::connect(timer,SIGNAL(timeout()),this,SLOT(update()));
  //timer->start(1000);
  resize(280,280);
  qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
  int x=0;
  while(x<250)
  {
      knots<<QPointF(x,qrand()%250);
      x+=qMin(qrand()%20+5,250);

  }
  smoothCurve=SmoothCurveCreator::createSmoothCurve(knots);//生成平滑曲线
  QStrPaintCurve="PaintCurve";

  PAINT_CALLING_COUNT_FLAG=0;
}

void PaintCurve::getPainterValue(QPainterPath path,QList<QPointF> points, int HCoordinateTemp,int VCoordinateTemp)
{ qDebug()<<"----PaintCurve::getPainterValue----";
  currentPointList.clear();
  qDebug()<<"the current currentPointList length of One is"<<currentPointList.size();
  smoothCurve=path;
  HCoordinate=HCoordinateTemp;
  VCoordinate=VCoordinateTemp;
  currentPointList=points;
  qDebug()<<"Now the length is "<<currentPointList.size();
  Color_FLAG=1;
  ListForList<<currentPointList;
  smoothCurvePathList<<smoothCurve;
  update();//paintEvent 事件调用

}

void PaintCurve::getPainterValueForSpeed(QPainterPath path, QList<QPointF> points)
{qDebug()<<"----PaintCurve::getPainterValueForSpeed----";
 currentPointList.clear();
 qDebug()<<"the current currentPointList length of Speed is"<<currentPointList.size();
 smoothCurve=path;
 currentPointList=points;
 qDebug()<<"Now the length is "<<currentPointList.size();
 Color_FLAG=2;
 ListForList<<currentPointList;
 smoothCurvePathList<<smoothCurve;
 update();
}

void PaintCurve::getPainterValueForLiqud(QPainterPath path, QList<QPointF> points)
{
     qDebug()<<"----PaintCurve::getPainterValueForLiqud----";
     currentPointList.clear();
     qDebug()<<"the current currentPointList length of Liqud is"<<currentPointList.size();
     smoothCurve=path;
     currentPointList=points;
     qDebug()<<"Now the length is "<<currentPointList.size();

     ListForList<<currentPointList;
     smoothCurvePathList<<smoothCurve;
     update();
}

void PaintCurve::getPainterValueForSpeedTorque(QPainterPath path, QList<QPointF> points)
{
    qDebug()<<"----PaintCurve::getPainterValueForSpeedTorque----";
    currentPointList.clear();
    qDebug()<<"the current currentPointList length of SpeedTorque is"<<currentPointList.size();
    smoothCurve=path;
    currentPointList=points;
    qDebug()<<"Now the length is "<<currentPointList.size();

    ListForList<<currentPointList;
    smoothCurvePathList<<smoothCurve;
    update();
}

void PaintCurve::getPainterValueForSpeedRatio(QPainterPath path, QList<QPointF> points)
{
    qDebug()<<"----PaintCurve::getPainterValueForSpeedRatio----";
    currentPointList.clear();
    qDebug()<<"the current currentPointList length of SpeedRatio is"<<currentPointList.size();
    smoothCurve=path;
    currentPointList=points;
    qDebug()<<"Now the length is "<<currentPointList.size();

    ListForList<<currentPointList;
    smoothCurvePathList<<smoothCurve;
    update();
}

void PaintCurve::paintEvent(QPaintEvent *event)
{/*
   static const QPoint hourHand[3]={
       QPoint(7,8),
       QPoint(-7,8),
       QPoint(0,40)
   };
   static const QPoint minuteHand[3]={
       QPoint(7,8),
       QPoint(-7,8),
       QPoint(0,70)
   };

   QColor hourColor(127,0,127);
   QColor minuteColor(0,127,127,191);

   QTime time=QTime::currentTime();

   QPainter painter(this);
   painter.translate(this->width()/2,this->height()/2);
   painter.setRenderHint(QPainter::Antialiasing);
   painter.setPen(Qt::NoPen);
   painter.setBrush(hourColor);

   painter.save();

   painter.rotate(30.0*(time.hour()+time.minute()/60.0));
   painter.drawConvexPolygon(hourHand,3);

   painter.restore();

   painter.setPen(hourColor);

   for(int i=0;i<12;i++)
   {
    painter.drawLine(88,0,96,0);
    painter.rotate(30.0);
   }

   painter.setPen(Qt::NoPen);
   painter.setBrush(minuteColor);

   painter.save();
   painter.rotate(6.0*(time.minute()+time.second()/60.0));
   painter.drawConvexPolygon(minuteHand,3);
   painter.restore();

   painter.setPen(minuteColor);

   for(int i=0;i<60;i++)
   {
       if(i%5!=0)
       {
           painter.drawLine(92,0,96,0);
           painter.rotate(6.0);
       }
   }

*/
    int rectX=25,rectY=25;
    int saveHC,saveVC;
    QPainter painter(this);
    saveHC=HCoordinate/10;//最大横坐标整数值除以10
    saveVC=VCoordinate/10;
    qDebug()<<QStrPaintCurve<<"==="<<HCoordinate;
    qDebug()<<QStrPaintCurve<<"==="<<VCoordinate;

    painter.translate(20,250);//原点
    //qDebug()<<"width="<<this->width();
    painter.setPen(QPen(Qt::black,3));
    painter.drawLine(QPoint(0,0),QPoint(0,-250));//纵轴

    painter.drawLine(QPoint(0,0),QPoint(250,0));//横轴
    //生成横轴坐标点
    for(int i=0;i<10;i++)
    {painter.setPen(QPen(Qt::black,2));
      painter.drawLine(QPoint(25*i,0),QPoint(25*i,-5));
    }
    //生成纵轴坐标点
    for(int i=0;i<10;i++)
    {
     painter.setPen(QPen(Qt::black,2));
     painter.drawLine(QPoint(0,-25*i),QPoint(5,-25*i));
    }
     painter.scale(1,-1);//坐标轴垂直翻转
     painter.drawText(QRect(-10,-15,20,20),"0");

     //绘制横轴坐标值
     painter.save();
     painter.scale(1,-1);
     for(int i=1;i<=10;i++)
     { int rectX=25;
       rectX=rectX*i-7.5;
       QRect rect(rectX,5,15,20);
       painter.drawText(rect,QString::number(i*saveHC));
     }
    // qDebug()<<QStrPaintCurve<<"=="<<HCoordinate;
     painter.restore();
     //绘制纵坐标值
     painter.save();
     painter.scale(1,-1);
    for(int i=10;i>0;i--)
    {
     int rectY=25;
     rectY=0-rectY*i;
     QRect rect(-15,rectY,15,20);
     painter.drawText(rect,QString::number(i*saveVC));
    }
    painter.restore();



///////////////////////////////////////////////////////////////////////////////////
                              /* 绘制曲线  */
 for(int paintCount=0;paintCount<ListForList.size();paintCount++)
{
 painter.save();

 if(paintCount==0){
painter.setPen(QPen(Qt::red,6));
}else if(paintCount==1)
{
painter.setPen(QPen(Qt::green,6));
}else if(paintCount==2)
 {
  painter.setPen(QPen(Qt::blue,6));
 }else if(paintCount==3)
 {
     painter.setPen(QPen(Qt::darkCyan,6));
 }else if(paintCount==4)
 {
     painter.setPen(QPen(Qt::darkMagenta,6));
 }

for(int i=0;i<ListForList.at(paintCount).size();i++)
{   qDebug()<<ListForList.at(paintCount).at(i);
    painter.drawPoint(ListForList.at(paintCount).at(i).x(),ListForList.at(paintCount).at(i).y());
}
painter.restore();
painter.drawPath(smoothCurvePathList.at(paintCount));
/////////////////////////////////////////////////////////////////////////////////////
}

}


void PaintCurve::showNonSmoothCurve()
{
    nonSmoothCurve.moveTo(knots[0]);
    for(int i=1,j=0;i<knots.size();i++)
     {qDebug()<<"knots[1%]="<<knots[i]<<" knots.size="<<knots.size();
      nonSmoothCurve.lineTo(knots[i]);
      qDebug()<<"j="<<j++;
     }
}

void PaintCurve::showSmoothCurve()
{

}

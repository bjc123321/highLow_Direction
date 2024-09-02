#include "mybtn.h"

MyBtn::MyBtn(QWidget *parent) : QPushButton(parent)
{
    btntext = "";
    MapStrVec["检测模块"] = ":/res/pushbutton/JCMK.png";
    MapStrVec["数据库模块"] = ":/res/pushbutton/SJKMK.png";
    MapStrVec["系统帮助"] = ":/res/pushbutton/XTBZ.png";
    MapStrVec["自检模块"] = ":/res/pushbutton/ZJMK.png";

    checkedState = false;
    MouseFLag = 0;
}

void MyBtn::settext(QString ModuleName)
{
    btntext = ModuleName;
    update();
}

void MyBtn::paintEvent(QPaintEvent *e)
{
    QString iconpath = MapStrVec[btntext];
    QPainter painter(this);
    QRect ICONREC{5,20,40,40};

    if(MouseFLag == 0 && checkedState) //点击状态
    {
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(24, 45, 68));
        painter.drawRect(0,0, width(),height());
        painter.restore();
    }
    else if(MouseFLag == 0 && !checkedState) //非点击状态
    {

    }
    else if(MouseFLag == 1 && checkedState) //点击状态
    {
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(24, 45, 68));
        painter.drawRect(0,0, width(),height());
        painter.restore();
    }
    else if(MouseFLag == 1 && !checkedState) //点击状态
    {
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 137, 137));
        painter.drawRect(0,0, width(),height());
        painter.restore();
    }
    else if(MouseFLag == 2)
    {
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 137, 137));
        painter.drawRect(0,0, width(),height());
        painter.restore();
    }

    if(MouseFLag == 2){
        iconpath = iconpath.insert(iconpath.indexOf(".png"),"2");
        painter.drawImage(ICONREC,QImage{iconpath});
    }
    else {
        iconpath = iconpath.insert(iconpath.indexOf(".png"),"1");
        painter.drawImage(ICONREC,QImage{iconpath});
    }
    QFont fo{"SimHei"};
    fo.setBold(true);
    fo.setPointSize(18);
    painter.setFont(fo);
    QPen pen;
    pen.setColor(QColor(255,255,255));
    painter.setPen(pen);
    painter.drawText(60,50,btntext);
}

void MyBtn::enterEvent(QEvent *e)
{
    MouseFLag = 1 ;
    update();
}

void MyBtn::leaveEvent(QEvent *e)
{
    MouseFLag = 0 ;
    update();
}

void MyBtn::mousePressEvent(QMouseEvent *e)
{
    MouseFLag = 2;
    emit clicked();
    update();
}

void MyBtn::mouseReleaseEvent(QMouseEvent *e)
{
    MouseFLag = 1 ;
    update();
}

void MyBtn::setCheckedState(bool value)
{
    checkedState = value;
    update();
}

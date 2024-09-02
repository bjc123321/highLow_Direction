#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    customW(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->titlelab->setText(SBMC);
    ui->label->setText(RJMC);

    connect(ui->btnClose,&QPushButton::clicked,[=](){OnClicked(ToClosed);});
    connect(ui->btnMax,&QPushButton::clicked,[=](){OnClicked(ToMaximize);});
    connect(ui->btnMin,&QPushButton::clicked,[=](){OnClicked(ToMinimize);});

    connect(ui->loginwid,&LoginWidget::LoginSuccess,[=](){this->LoginSuccess();});

//    startTimer(100);

//    thread = new operathread();  ////日志记录线程
//    thread->start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *e)
{
//    qDebug()<<"关闭程序";

//    thread->terminate();
//    thread->wait();

//    operainformation.push_back(OperationLogStruct{QDATE,TIME,"软件退出"});
//    this->exitlog();
}

void Widget::resizeEvent(QResizeEvent *e)
{
    emit centerwidget(ui->CenterWid->size());
}

void Widget::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e)
    ui->timelabel->setText(STRDATETIME);
}

void Widget::exitlog()
{
//    auto begin = operainformation.begin();
//    while (begin != operainformation.end()) {
//        QMutexLocker locker(&local_mutex); //上锁
//        bool a =  InsertData
//                ("insert into log ('datetime','operation','user') "
//                 "values ""('"
//                 +begin->date+" "
//                 +begin->time+"','"
//                 +begin->OperationString+"','"
//                 +LoginUserName+"')");
//        QThread::msleep(2); ////在有内容的情况下，2ms存储一条；
//        begin++;
//    }
}

void Widget::LoginSuccess()
{
    m_main = new MainInterface(ui->CenterWid);
    m_main->setGeometry(0,0,ui->CenterWid->width(),ui->CenterWid->height());
    m_main->show();
//    connect(this,&Widget::centerwidget,[=](QSize size){
//        m_main->setGeometry(0,0,size.width(),size.height());});

//    LOGINUSER == Administrator ?
//                ui->userlabel->setText("管理员:"+LoginUserName) :
//                LOGINUSER == AverageUser   ?
//                    ui->userlabel->setText("普通用户:"+LoginUserName):
//                    ui->userlabel->setText("超级用户:"+LoginUserName);
}

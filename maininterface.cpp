#include "maininterface.h"
#include "ui_maininterface.h"

MainInterface::MainInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainInterface)
{
    ui->setupUi(this);
    ui->mainlab->setText(RJMC);

    for(int i = 0 ; i < ModulePartitionList.length() ; i++)
    {
        MyBtn* btn1 = new MyBtn(ui->groupBox);
        btn1->setMinimumSize(200,80);
        btn1->setMaximumSize(200,80);
        ui->btnLayout->insertWidget(i,btn1);
        btn1->settext(ModulePartitionList[i]);
        mbtnVec<<btn1;
    }

    for(int i = 0 ; i < mbtnVec.length(); i++)
        connect(mbtnVec[i],&MyBtn::clicked,[=](){
            for(int j = 0 ;  j <  mbtnVec.length() ; j++)
                mbtnVec[j]->setCheckedState(false);
            mbtnVec[i]->setCheckedState(true);
            Animation(i);
            ui->stackedWidget->setCurrentIndex(i);
        });

    ////检测模块
    m_test = new FunctionTest(this);
    ui->stackedWidget->addWidget(m_test);

    ///数据处理模块
    m_damang = new DatabaseManag(this);
    ui->stackedWidget->addWidget(m_damang);

//    ///自检模块
//    m_self = new SelfInspection(this);
//    ui->stackedWidget->addWidget(m_self);
//    ///系统帮助
//    m_help = new SystemHelps(this);
//    ui->stackedWidget->addWidget(m_help);

//    ///数据处理
//    m_data = new Dataprocess();


//    mbtnVec[0]->setCheckedState(true);

//    ////将检测模块与数据处理模块进行信号槽连接
//    QObject::connect(m_test,QOverload<QString ,double >::of(&FunctionTest::valueChangedvalue),
//                     m_data,&Dataprocess::SignalOutputValue);

//    QObject::connect(m_test,QOverload<QString ,bool >::of(&FunctionTest::valueChangedflag),
//                     m_data,&Dataprocess::SignalOutputFlag);

//    QObject::connect(m_test,QOverload<QString>::of(&FunctionTest::valueChangedgear),
//                     m_data,&Dataprocess::SignalOutputGear);


//    /////接收与发送电磁阀和档位信息
//    QObject::connect(m_data,&Dataprocess::SolenoidValve,[=](QList<bool> Solenoidlist,QString DangWei){
//        m_test->ReceiveSolenoid(Solenoidlist,DangWei);
//    });

//    QObject::connect(m_data,&Dataprocess::ADdata,[=](QString adname , double value){
//        m_test->ReceiveADvalue(adname,value);
//    });

//    //CANA与CANB是否好使
//    QObject::connect(m_data,&Dataprocess::DA_CANA_change,[=](bool value){
//        m_test->CANA_fix(value);
//    });
//    QObject::connect(m_data,&Dataprocess::DA_CANB_change,[=](bool value){
//        m_test->CANB_fix(value);
//    });
//    //    void (QString sensorname , QString text);
//    //    void ();
//    //    void CANdata(QString candataname , QString data);
//    ////接收CAN总线上传的传感器数据信息
//    QObject::connect(m_data,&Dataprocess::CANSensorData,[=](QString sensorname , double value){
//        m_test->ReceiveSensor(sensorname,value);
//    });

//    QObject::connect(m_data,&Dataprocess::CANSensorFlag,[=](QString sensorname , bool flag){
//        m_test->ReceiveBJLLED(sensorname,flag);
//    });

//    QObject::connect(m_data,&Dataprocess::CANSensorText,[=](QString sensorname , int textid){
//        m_test->ReceiveBSDW(sensorname,textid);
//    });

//    QObject::connect(m_data,&Dataprocess::CANdata,[=](QString candataname , QString datalist){
//        m_test->ReceiveCanData(candataname,datalist);
//    });
}

MainInterface::~MainInterface()
{
    delete ui;
}

void MainInterface::timerEvent(QTimerEvent *e)
{

}

void MainInterface::Animation(int index)
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->linelab, "geometry");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setStartValue(ui->linelab->geometry());//开始位置
    animation->setEndValue(QRect{0,index*80,5,80});//结束位置
    animation->start(QAbstractAnimation::DeleteWhenStopped);//结束时自动释放资源
}

void MainInterface::on_toolhide_clicked()
{
    QPropertyAnimation *animation1 = new QPropertyAnimation(ui->leftwidget, "geometry");
    QPropertyAnimation *animation2 = new QPropertyAnimation(ui->centerwidget, "geometry");

    animation1->setEasingCurve(QEasingCurve::OutCubic);
    animation2->setEasingCurve(QEasingCurve::OutCubic);

    QRect rec1 =  ui->leftwidget->geometry();
    QRect rec2 =  ui->centerwidget->geometry();
    if(ui->toolhide->isChecked()) //隐藏
    {
        ui->leftwidget->setMinimumWidth(60);
        animation1->setDuration(300);
        animation2->setDuration(500);
    }
    else                         ///不隐藏
    {
        ui->leftwidget->setMaximumWidth(200);
        animation1->setDuration(0);
        animation2->setDuration(0);
    }

    if(ui->toolhide->isChecked())
    {
        animation1->setStartValue(rec1);//开始位置
        animation1->setEndValue(QRect{0,60,40,rec1.height()});//结束位置
        animation2->setStartValue(rec2);//开始位置
        animation2->setEndValue(QRect{60,60,ui->wid->width()-60,rec2.height()});//结束位置
    }
    else
    {
        animation1->setStartValue(ui->leftwidget->geometry());//开始位置
        animation1->setEndValue(QRect{0,60,200,ui->leftwidget->height()});//结束位置
        animation2->setStartValue(ui->centerwidget->geometry());//开始位置
        animation2->setEndValue(QRect{200,60,ui->wid->width()-200,rec2.height()});//结束位置
    }

    connect(animation1,&QPropertyAnimation::finished,[=](){
        if(ui->toolhide->isChecked()) //隐藏状态
        {
            ui->leftwidget->setMaximumWidth(60);
        }
        else
        {
            ui->leftwidget->setMinimumWidth(200);
            ui->leftwidget->setMaximumWidth(200);

        }
    });

    animation1->start(QAbstractAnimation::DeleteWhenStopped);//结束时自动释放资源
    animation2->start(QAbstractAnimation::DeleteWhenStopped);//结束时自动释放资源
}

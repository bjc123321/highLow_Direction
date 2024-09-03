#include "functiontest.h"
#include "ui_functiontest.h"
#include <QCoreApplication>

// 创建按钮文本与页面的映射关系
QMap<QString, QWidget*> buttonPageMap;
QMap<QString, QWidget*> buttonPageMap1;
QMap<QString, QWidget*> buttonPageMap2;
QMap<QString, QWidget*> buttonPageMap3;

QList<double> value ;

QString page_str;

double N_M ;
double N_m_04_direction;
double DA_Hua_04hight_low;
double DA_Hua_04_direction;
double DA_Hua_04_A_hight_low;
double DA_Hua_04_A_direction;
double YeYa_Shen_suo_Hight;   //高低机的液压伸缩
bool height_or_direction;
int page_num;

//电磁电流
float Electromagnetic_Cur =0;

FunctionTest::FunctionTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionTest)
{
    ui->setupUi(this);
    this->InitButtonGroup();

    m_AD=new SerialAD();
    m_AD->start();
    m_DA=new SerialDA();
    m_DA->start();
    m_IO=new SerialIO();
    m_IO->start();
    m_Power=new SerialThread();
    m_Power->start();
    m_AnPower=new SerialPowerThread();
    m_AnPower->start();

    ui->itemwidget->hide();//定制需求将此模块隐藏,后续根据需求解开-----------2024.9.3
    connect(ui->itemwidget,&NavButtonGroup::buttonClicked,[=](int index){
        ui->stackedWidget->setCurrentIndex(index);

        if(index == 0 )
        {
            //qDebug()<<"04步兵战车高低机";
            m_IO->set_channel(8,false);
            m_AnPower->setPowerDC(26,100);
            vflag=true;
            page_str = "04步兵战车高低机";
            height_or_direction = true;
            page_num = 0;
            ui->gridStackedWidget->setCurrentWidget(ui->gridStackedWidgetPage4);
        }
        else if (index == 1)
        {
            //qDebug()<<"04炮塔武器方向机";
            m_IO->set_channel(8,true);
            m_AnPower->setPowerDC(26,100);
            vflag=true;
            page_str = "04炮塔武器方向机";
            height_or_direction = false;
            page_num = 1;
            ui->gridStackedWidget_2->setCurrentWidget(ui->gridStackedWidgetPage1_5);
        }
        else if(index == 2)
        {
            //qDebug()<<"04A步兵战车高低机";
            m_IO->set_channel(8,false);
            m_AnPower->setPowerDC(26,100);
            vflag=false;
            page_str = "04A步兵战车高低机";
            height_or_direction = true;
            page_num = 2;
            ui->gridStackedWidget_3->setCurrentWidget(ui->gridStackedWidgetPage1_9);
        }
        else if(index == 3)
        {
            //qDebug()<<"04A炮塔武器方向机";
            m_IO->set_channel(8,true);
            m_AnPower->setPowerDC(26,100);
            vflag=false;
            page_str = "04A炮塔武器方向机";
            height_or_direction = false;
            page_num = 3;
            ui->gridStackedWidget_4->setCurrentWidget(ui->gridStackedWidgetPage4_2);
        }
    });
    ui->stackedWidget->setCurrentIndex(0);

    PageInit();
    InitButtonPage();


    ui->btnDW_20->hide();
    ui->btnDW_22->hide();
    ui->btnDW_26->hide();
    ui->btnDW_34->hide();

    ///测试信息录入类初始化
    m_info = new testinformation(this);
    m_info->hide();


//    timer=new QTimer(this);
//    connect(timer,&QTimer::timeout,this,&FunctionTest::timeslot);


}

FunctionTest::~FunctionTest()
{
    delete ui;
}

bool compare(double a,double b)
{
    return a>b;
}

bool sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    return true;
}

void FunctionTest::InitButtonGroup()
{
    int count = DetectionModuleList.count();
    ui->itemwidget->clearButton();
    for (int i = 0; i < count; i++) {
        QPushButton *btn = new QPushButton;
        btn->setCheckable(true);
        btn->setText(DetectionModuleList.at(i));
        btn->setMinimumHeight(38);
        ui->itemwidget->addButton((QAbstractButton *)btn, i);
    }
    ui->itemwidget->setBaseColor(QColor(101, 169, 200));
    ui->itemwidget->setLinePosition(NavButtonGroup::LinePosition_Bottom);
}

void FunctionTest::InitButtonPage()
{
    buttonPageMap["跑合测试"] = ui->gridStackedWidgetPage1;
    buttonPageMap["手轮力矩测试"] = ui->gridStackedWidgetPage2;
    buttonPageMap["空回量测试"] = ui->gridStackedWidgetPage3;
    buttonPageMap["打滑测试"] = ui->gridStackedWidgetPage4;
    
    buttonPageMap1["跑合测试"] = ui->gridStackedWidgetPage1_2;
    buttonPageMap1["手轮力矩测试"] = ui->gridStackedWidgetPage1_3;
    buttonPageMap1["空回量测试"] = ui->gridStackedWidgetPage1_4;
    buttonPageMap1["打滑测试"] = ui->gridStackedWidgetPage1_5;

    buttonPageMap2["跑合测试"] = ui->gridStackedWidgetPage1_6;
    buttonPageMap2["手轮力矩测试"] = ui->gridStackedWidgetPage1_7;
    buttonPageMap2["空回量测试"] = ui->gridStackedWidgetPage1_8;
    buttonPageMap2["打滑测试"] = ui->gridStackedWidgetPage1_9;

    buttonPageMap3["跑合测试"] = ui->gridStackedWidgetPage1_10;
    buttonPageMap3["手轮力矩测试"] = ui->gridStackedWidgetPage2_2;
    buttonPageMap3["空回量测试"] = ui->gridStackedWidgetPage3_2;
    buttonPageMap3["打滑测试"] = ui->gridStackedWidgetPage4_2;

}

void FunctionTest::PageInit()
{


    //计时间
    timer1 = new QTimer(this);
    connect(timer1,&QTimer::timeout,[=](){
        updateTimer();
    });


    //断继电器6计时器
    offTimer = new QTimer();//定时更新时间
    offCount=0;

    connect(ui->buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [=](QAbstractButton *button){ButtonGroupCLicked(button);});
    connect(ui->buttonGroup_2, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [=](QAbstractButton *button){ButtonGroupCLicked(button);});
    connect(ui->buttonGroup_3, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [=](QAbstractButton *button){ButtonGroupCLicked(button);});
    connect(ui->buttonGroup_4, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [=](QAbstractButton *button){ButtonGroupCLicked(button);});

    //电磁离合器电压与电流
    connect(m_AnPower,&SerialPowerThread::sendPowerValue, [=](float vol, float cur){
        Electromagnetic_Cur = cur;
        ui->linC_10->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_11->setText(QString::number(cur, 'f', 2)+" A");
        ui->linC_18->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_17->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_31->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_30->setText(QString::number(cur, 'f', 2)+" A");


        ui->linC_25->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_24->setText(QString::number(cur, 'f', 2)+" A");


        ui->linC_34->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_33->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_47->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_48->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_53->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_54->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_57->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_56->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_70->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_71->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_76->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_77->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_79->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_80->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_94->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_93->setText(QString::number(cur, 'f', 2)+" A");

        ui->linC_100  ->setText(QString::number(vol, 'f', 2)+" V");
        ui->linC_99->setText(QString::number(cur, 'f', 2)+" A");

    });

    connect(m_AD, &SerialAD::sendValue,[=](QList<double> &value_list){


        if(value_list[2]*50 > 4)
        {
            N_M = value_list[2]*50;
        }
        else{
            N_M = 0;
        }
        if(value_list[0]*50 > 7)
        {
            N_m_04_direction =value_list[0]*100;
        }
        else{
            N_m_04_direction = 0;
        }
        //04高低机
        ui->linC_14->setText(QString::number(N_M)+" N.M");
        ui->linC_15->setText(QString::number(value_list[3]*50)+" r/min");

        if(page_num == 0)
        {
           value<<N_M;
        }
        else if(page_num == 2)
        {
            value<<N_M;
        }
        if(N_M >= DA_Hua_04hight_low)
        {
            DA_Hua_04hight_low = N_M;
            //qDebug()<<"111111111111111111111111    "<<DA_Hua_04hight_low*1.1;
            ui->linC_29->setText(QString::number(DA_Hua_04hight_low)+" N.M");
        }
        //ui->linC_28->setText(QString::number(value_list[3]*50)+" N.M");

        //04方向机
        ui->linC_38->setText(QString::number(N_m_04_direction)+" N.M");
        ui->linC_37->setText(QString::number(value_list[1]*100)+" r/min");

        if(value_list[0] > DA_Hua_04_direction)
        {
            DA_Hua_04_direction = value_list[0];
            ui->linC_51->setText(QString::number(N_m_04_direction)+" N.M");
        }


        //04A高低机
        ui->linC_61->setText(QString::number(N_M)+" N.M");
        ui->linC_60->setText(QString::number(value_list[3]*50)+" r/min");

        if(N_M >= DA_Hua_04_A_hight_low)
        {
            DA_Hua_04_A_hight_low = N_M;
            ui->linC_74->setText(QString::number(N_M)+" N.M");
        }

        //04A方向机
        ui->linC_83->setText(QString::number(N_m_04_direction)+" N.M");
        ui->linC_84->setText(QString::number(value_list[1]*100)+" r/min");

        if(value_list[0] > DA_Hua_04_A_direction)
        {
            DA_Hua_04_A_direction = value_list[0];
            ui->linC_98->setText(QString::number(N_m_04_direction)+" N.M");
        }
        //ui->linC_97->setText(QString::number(value_list[1]*100)+" N.M");
        //高低机 液压杆伸缩位置
        //qDebug()<<"22222222222222222222222222222222222222222"<<value_list[9];
        ui->linC_32->setText(QString::number(value_list[9])+"V");
        ui->linC_50->setText(QString::number(value_list[9])+"V");
        if(value_list[9] > 3.80 && height_or_direction == true)
        {
            m_IO->set_channel(9, false);
            ui->pushButton_56->setChecked(false);
            ui->pushButton_17->setChecked(false);
            ui->widC_24->setstate(false);
            ui->widC_42->setstate(false);
            ui->widC_59->setstate(false);
            ui->widC_75->setstate(false);
            //ui->linC_32->setText("已脱齿");

        }else if(value_list[9] < 2.35 && height_or_direction == true)
        {
            m_IO->set_channel(10,false);
            ui->pushButton_18->setChecked(false);
            ui->pushButton_55->setChecked(false);
            ui->widC_25->setstate(false);
            ui->widC_41->setstate(false);
            ui->widC_58->setstate(false);
            ui->widC_74->setstate(false);
        }
        //qDebug()<<"11111111111111111111111111111111111111111"<<value_list[8];
        ui->linC_102->setText(QString::number(value_list[8])+"V");
        ui->linC_96->setText(QString::number(value_list[8])+"V");
        if(value_list[8] < 2.30 && height_or_direction == false)
        {
            m_IO->set_channel(9, false);
            ui->pushButton_37->setChecked(false);
            ui->pushButton_75->setChecked(false);
            ui->widC_42->setstate(false);
            ui->widC_75->setstate(false);
        }
        else if(value_list[8] > 3.80 && height_or_direction == false)
        {
            m_IO->set_channel(10, false);
            ui->pushButton_36->setChecked(false);
            ui->pushButton_74->setChecked(false);
            ui->widC_41->setstate(false);
            ui->widC_74->setstate(false);
        }


    });

    //电机电压电流
    connect(m_Power,&SerialThread::sendPowerValue, [=](QString vol, QString cur, QString pow){
        qDebug()<<"电源参数范围值为"+vol+'V'+cur+'A'+pow+'w';
        ui->linC_12->setText(vol+'V');
        ui->linC_13->setText(cur+'A');

        ui->linC_19->setText(vol+'V');
        ui->linC_20->setText(cur+'A');

        ui->linC_35->setText(vol+'V');
        ui->linC_36->setText(cur+'A');

        ui->linC_58->setText(vol+'V');
        ui->linC_59->setText(cur+'A');

        ui->linC_81->setText(vol+'V');
        ui->linC_82->setText(cur+'A');

        ui->linC_55->setText(vol+'V');
        ui->linC_26->setText(cur+'A');

        ui->linC_62->setText(vol+'V');
        ui->linC_72->setText(cur+'A');

        ui->linC_78->setText(vol+'V');
        ui->linC_73->setText(cur+'A');

        ui->linC_95->setText(vol+'V');
        ui->linC_85->setText(cur+'A');
    });

    //电磁离合器控制
    //04高低
    //上电与掉电
    connect(ui->pushButton_3, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_20, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    //04方向
    connect(ui->pushButton_23, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_33, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_39, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    //04A高低
    connect(ui->pushButton_41, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_52, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_58, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    //04A方向
    connect(ui->pushButton_60, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_70, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);
    connect(ui->pushButton_77, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_electrify_outage);



    //电磁启动与关闭
    //04高低
    connect(ui->pushButton_2, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_14, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_19, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    //04方向
    connect(ui->pushButton_22, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_32, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_38, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    //04A高低与方向
    connect(ui->pushButton_40, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_51, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_57, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    //04A方向
    connect(ui->pushButton_59, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_71, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);
    connect(ui->pushButton_76, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_activate_off);


    //电机上电
    //04高低
    connect(ui->pushButton_4, &QPushButton::clicked, this, &FunctionTest::Electric_machine_electrify_outage);
    connect(ui->pushButton_84, &QPushButton::clicked, this, &FunctionTest::Electric_machine_electrify_outage);
    //04方向
    connect(ui->pushButton_26, &QPushButton::clicked, this, &FunctionTest::Electric_machine_electrify_outage);
    connect(ui->pushButton_90, &QPushButton::clicked, this, &FunctionTest::Electric_machine_electrify_outage);
    //04A高低
    connect(ui->pushButton_44, &QPushButton::clicked, this, &FunctionTest::Electric_machine_electrify_outage);
    connect(ui->pushButton_89, &QPushButton::clicked, this, &FunctionTest::Electric_machine_electrify_outage);
    //04A方向
    connect(ui->pushButton_61, &QPushButton::clicked, this, &FunctionTest::Electric_machine_electrify_outage);
    connect(ui->pushButton_93, &QPushButton::clicked, this, &FunctionTest::Electric_machine_electrify_outage);


    //电机正传
    //04高低
    connect(ui->pushButton_5, &QPushButton::clicked, this, &FunctionTest::Electric_machine_forward_set);
    connect(ui->pushButton_85, &QPushButton::clicked, this, &FunctionTest::Electric_machine_forward_set);

    //04方向
    connect(ui->pushButton_24, &QPushButton::clicked, this, &FunctionTest::Electric_machine_forward_set);
    connect(ui->pushButton_91, &QPushButton::clicked, this, &FunctionTest::Electric_machine_forward_set);
    //04A高低
    connect(ui->pushButton_42, &QPushButton::clicked, this, &FunctionTest::Electric_machine_forward_set);
    connect(ui->pushButton_87, &QPushButton::clicked, this, &FunctionTest::Electric_machine_forward_set);
    //04A方向
    connect(ui->pushButton_62, &QPushButton::clicked, this, &FunctionTest::Electric_machine_forward_set);
    connect(ui->pushButton_94, &QPushButton::clicked, this, &FunctionTest::Electric_machine_forward_set);


    //电机反转
    //04高低
    connect(ui->pushButton_7, &QPushButton::clicked, this, &FunctionTest::Electric_machine_fan_set);
    connect(ui->pushButton_86, &QPushButton::clicked, this, &FunctionTest::Electric_machine_fan_set);
    //04方向
    connect(ui->pushButton_25, &QPushButton::clicked, this, &FunctionTest::Electric_machine_fan_set);
    connect(ui->pushButton_92, &QPushButton::clicked, this, &FunctionTest::Electric_machine_fan_set);
    //04A高低
    connect(ui->pushButton_43, &QPushButton::clicked, this, &FunctionTest::Electric_machine_fan_set);
    connect(ui->pushButton_88, &QPushButton::clicked, this, &FunctionTest::Electric_machine_fan_set);
    //04A方向
    connect(ui->pushButton_63, &QPushButton::clicked, this, &FunctionTest::Electric_machine_fan_set);
    connect(ui->pushButton_95, &QPushButton::clicked, this, &FunctionTest::Electric_machine_fan_set);





    //电机配置  04高低
    connect(ui->pushButton_6,&QPushButton::clicked,[=](){
        if(ui->lineEdit_2->text().toDouble()>0.8 || ui->lineEdit_2->text().toDouble()<0)
        {
            QMessageBox::information(this, "提示","配置值不允许超过0.8A");
            return;
        }
        else
        {
            m_DA->setVol_channel(1,ui->lineEdit_2->text().toDouble()/0.4);
        }
    });
    //电机配置  04方向
    connect(ui->pushButton_27,&QPushButton::clicked,[=](){
        if(ui->lineEdit->text().toDouble()>1.2 || ui->lineEdit->text().toDouble()<0)
        {
            QMessageBox::information(this, "提示","配置值不允许超过1.2A");
            return;
        }
        else
        {
            m_DA->setVol_channel(1,ui->lineEdit->text().toDouble()/0.4);
        }
    });

    //04A高低
    connect(ui->pushButton_45,&QPushButton::clicked,[=](){
        if(ui->lineEdit_7->text().toDouble()>0.8 || ui->lineEdit_7->text().toDouble()<0)
        {
            QMessageBox::information(this, "提示","配置值不允许超过0.8A");
            return;
        }
        else
        {
            m_DA->setVol_channel(1,ui->lineEdit_7->text().toDouble()/0.4);
        }
    });


    //04A方向配置
    connect(ui->pushButton_64,&QPushButton::clicked,[=](){
        if(ui->lineEdit_10->text().toDouble()>1.2 || ui->lineEdit_10->text().toDouble()<0)
        {
            QMessageBox::information(this, "提示","配置值不允许超过1.2A");
            return;
        }
        else
        {
            m_DA->setVol_channel(1,ui->lineEdit_10->text().toDouble()/0.4);
        }
    });

    //液压电磁阀
    //04高低机
    connect(ui->pushButton_15, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_hydraulic_activate_off_hight_low);
    //04方向机
    connect(ui->pushButton_34, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_hydraulic_activate_off_direction);
    //04A高低机
    connect(ui->pushButton_53, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_hydraulic_activate_off_hight_low);
    //04A方向机
    connect(ui->pushButton_72, &QPushButton::clicked, this, &FunctionTest::Electromagnetic_hydraulic_activate_off_direction);


    //液压电磁阀配置   04高低机
    connect(ui->pushButton_16,&QPushButton::clicked,[=](){
        if(ui->linC_27->text().toDouble()<4 || ui->linC_27->text().toDouble()>20)
        {
            QMessageBox::information(this, "提示","配置值在2~20mA内");
            return;
        }
        else{
            m_DA->setCur_channel(1,ui->linC_27->text().toDouble());
        }
    });
    connect(ui->pushButton_35,&QPushButton::clicked,[=](){
        if(ui->lineEdit_5->text().toDouble()<4 || ui->lineEdit_5->text().toDouble()>20)
        {
            QMessageBox::information(this, "提示","配置值在4~20mA内");
            return;
        }
        else{
            m_DA->setCur_channel(1,ui->lineEdit_5->text().toDouble());
        }
    });

    //04A高低机液压电磁阀配置
    connect(ui->pushButton_54,&QPushButton::clicked,[=](){
        if(ui->lineEdit_9->text().toDouble()<4 || ui->lineEdit_9->text().toDouble()>20){
            QMessageBox::information(this, "提示","配置值在4~20mA内");
            return;
        }else{
            m_DA->setCur_channel(1,ui->lineEdit_9->text().toDouble());
        }
    });
    //04A方向机液压电磁阀配置
    connect(ui->pushButton_73,&QPushButton::clicked,[=](){
        if(ui->lineEdit_12->text().toDouble()<4 || ui->lineEdit_12->text().toDouble()>20){
            QMessageBox::information(this, "提示","配置值在4~20mA内");
            return;
        }else{
            m_DA->setCur_channel(1,ui->lineEdit_12->text().toDouble());
        }
    });

    //液压电机正转
    //04高低机
    connect(ui->pushButton_17, &QPushButton::clicked, this, &FunctionTest::Electric_machine_hydraulic_forward_set);
    //04A高低机
    connect(ui->pushButton_56, &QPushButton::clicked, this, &FunctionTest::Electric_machine_hydraulic_forward_set);

    //04方向机
    connect(ui->pushButton_37, &QPushButton::clicked, this, &FunctionTest::Electric_machine_hydraulic_forward_set);
    //04A方向机
    connect(ui->pushButton_75, &QPushButton::clicked, this, &FunctionTest::Electric_machine_hydraulic_forward_set);


    //液压电机反转
    //04高低机
    connect(ui->pushButton_18, &QPushButton::clicked, this, &FunctionTest::Electric_machine_hydraulic_fan_set);
    //04A高低机
    connect(ui->pushButton_55, &QPushButton::clicked, this, &FunctionTest::Electric_machine_hydraulic_fan_set);

    //04方向机
    connect(ui->pushButton_36, &QPushButton::clicked, this, &FunctionTest::Electric_machine_hydraulic_fan_set);
    //04A方向机
    connect(ui->pushButton_74, &QPushButton::clicked, this, &FunctionTest::Electric_machine_hydraulic_fan_set);


    //风扇
    connect(ui->pushButton_78,&QPushButton::clicked,[=](){
        m_IO->set_channel(4,ui->pushButton_78->isChecked());
        ui->widC_78->setstate(ui->pushButton_78->isChecked());
    });

    connect(ui->pushButton_79,&QPushButton::clicked,[=](){
        m_IO->set_channel(4,ui->pushButton_79->isChecked());
        ui->widC_79->setstate(ui->pushButton_79->isChecked());
    });

    //最大力矩清空
    connect(ui->pushButton_80,&QPushButton::clicked,[=](){
        ui->linC_98->setText("");
        DA_Hua_04_A_direction = 0;
    });
    connect(ui->pushButton_81,&QPushButton::clicked,[=](){
        ui->linC_74->setText("");
        ui->linC_75->setText("");
        DA_Hua_04_A_hight_low = 0;
        value.clear();
    });
    connect(ui->pushButton_82,&QPushButton::clicked,[=](){
        ui->linC_51->setText("");
        DA_Hua_04_direction = 0;
    });
    connect(ui->pushButton_83,&QPushButton::clicked,[=](){
        ui->linC_29->setText("0  N.M");
        ui->linC_28->setText("0  N.M");
        DA_Hua_04hight_low = 0;
        value.clear();
    });
    connect(ui->pushButton_96,&QPushButton::clicked,[=](){

        QList<double> result;
        for(int i=0;i<value.size();i++)
        {
            for(int j=i+1;j<value.size();j++)
            {
                if(qAbs(value[i]-value[j])<=10)
                {
                    if(!result.contains(value[i]))
                    {
                        result.append(value[i]);
                        qDebug()<<value[i];
                    }
                    if(!result.contains(value[j]))
                    {
                        result.append(value[j]);
                        qDebug()<<value[j];
                    }
                }
            }
        }

        qSort(result.begin(),result.end(),compare);
        double sum;
        for(int i=0;i<3 && i<result.size();i++)
        {
            sum += result[i];
        }
        double average = sum / 3;
        ui->linC_28->setText(QString::number(average, 'f', 2)+" N.M");
        foreach (double i, value) {
           qDebug()<<"111111111111111111111"<<i;
        }
        qDebug()<<"2222222222222222222222"<<average<<"11111111111111111"<<sum;
    });
    connect(ui->pushButton_97,&QPushButton::clicked,[=](){

                QList<double> result;
                for(int i=0;i<value.size();i++)
                {
                    for(int j=i+1;j<value.size();j++)
                    {
                        if(qAbs(value[i]-value[j])<=10)
                        {
                            if(!result.contains(value[i]))
                            {
                                result.append(value[i]);
                                qDebug()<<value[i];
                            }
                            if(!result.contains(value[j]))
                            {
                                result.append(value[j]);
                                qDebug()<<value[j];
                            }
                        }
                    }
                }

                qSort(result.begin(),result.end(),compare);
                double sum;
                for(int i=0;i<3 && i<result.size();i++)
                {
                    sum += result[i];
                }
                double average = sum / 3;
                ui->linC_75->setText(QString::number(average, 'f', 2)+" N.M");
                foreach (double i, value) {
                   qDebug()<<"111111111111111111111"<<i;
                }
                qDebug()<<"2222222222222222222222"<<average<<"11111111111111111"<<sum;
            });
}

void FunctionTest::ButtonGroupCLicked(QAbstractButton *btn)
{

    /*
     * buttonPageMap  -- buttonGroup_2 -- gridStackedWidget
     * buttonPageMap1 -- buttonGroup   -- gridStackedWidget_2
     * buttonPageMap2 -- buttonGroup3   -- gridStackedWidget_3
     * buttonPageMap3 -- buttonGroup4   -- gridStackedWidget_4
    */

    Q_UNUSED(btn)
    QAbstractButton *checkedbtn = ui->buttonGroup_2->checkedButton();
    if(checkedbtn)
    {
        // 获取按钮文本
        QString buttonText = checkedbtn->text();
        // 根据映射关系设置当前页面
        if (buttonPageMap.contains(buttonText)) {
            ui->gridStackedWidget->setCurrentWidget(buttonPageMap[buttonText]);
        }
    }

    checkedbtn = ui->buttonGroup->checkedButton();
    if(checkedbtn)
    {
        // 获取按钮文本
        QString buttonText = checkedbtn->text();
        if (buttonPageMap.contains(buttonText)) {
            ui->gridStackedWidget_2->setCurrentWidget(buttonPageMap1[buttonText]);
        }
    }

    checkedbtn = ui->buttonGroup_3->checkedButton();
    if(checkedbtn)
    {
        // 获取按钮文本
        QString buttonText = checkedbtn->text();
        if (buttonPageMap2.contains(buttonText)) {
            ui->gridStackedWidget_3->setCurrentWidget(buttonPageMap2[buttonText]);
        }
    }

    checkedbtn = ui->buttonGroup_4->checkedButton();
    if(checkedbtn)
    {
        // 获取按钮文本
        QString buttonText = checkedbtn->text();
        if (buttonPageMap3.contains(buttonText)) {
            ui->gridStackedWidget_4->setCurrentWidget(buttonPageMap3[buttonText]);
        }
    }

}


//计算时间
void FunctionTest::updateTimer()
{
    int temp_time =timer1->remainingTime()/1000;
    qDebug()<<"时间"<<currentSecond;
    currentSecond+=temp_time;
    if(currentSecond>=seconds)
    {
        timer1->stop();
        currentSecond = 0;
    }
    else
    {
        int remainingMinutes = currentSecond/60;
        int remainingSeconds = currentSecond%60;
        ui->linC_16->setText(QString("%1:%2").arg(remainingMinutes,2,10,QChar('0')).arg(remainingSeconds,2,10,QChar('0')));
        ui->linC_23->setText(QString("%1:%2").arg(remainingMinutes,2,10,QChar('0')).arg(remainingSeconds,2,10,QChar('0')));
        ui->linC_21->setText(QString("%1:%2").arg(remainingMinutes,2,10,QChar('0')).arg(remainingSeconds,2,10,QChar('0')));
        ui->linC_39->setText(QString("%1:%2").arg(remainingMinutes,2,10,QChar('0')).arg(remainingSeconds,2,10,QChar('0')));

        ui->linC_49->setText(QString("%1:%2").arg(remainingMinutes,2,10,QChar('0')).arg(remainingSeconds,2,10,QChar('0')));


    }
}

//电磁上电与掉电
void FunctionTest::Electromagnetic_electrify_outage()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender()); // 获取发送信号的按钮

    if (btn && btn->isChecked())
    {
        m_AnPower->powerStart();
        m_AnPower->flag=true;//设置上电为true，
    }
    else
    {
        m_AnPower->powerStop();
    }
    ui->widC_12->setstate(btn->isChecked());
    ui->widC_17->setstate(btn->isChecked());
    ui->widC_21->setstate(btn->isChecked());
    ui->widC_27->setstate(btn->isChecked());
    ui->widC_29->setstate(btn->isChecked());

    ui->widC_39->setstate(btn->isChecked());
    ui->widC_44->setstate(btn->isChecked());
    ui->widC_46->setstate(btn->isChecked());
    ui->widC_56->setstate(btn->isChecked());
    ui->widC_61->setstate(btn->isChecked());
    ui->widC_62->setstate(btn->isChecked());
    ui->widC_71->setstate(btn->isChecked());
    ui->widC_77->setstate(btn->isChecked());
}
//电磁启动与关闭
void FunctionTest::Electromagnetic_activate_off()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender()); // 获取发送信号的按钮
    if(btn->isChecked())
    { 
        DO6();
        m_IO->set_channel(6, btn->isChecked());
    }
    else
    {
        m_IO->set_channel(5, false);
    }
    ui->widC_9->setstate(btn->isChecked());
    ui->widC_16->setstate(btn->isChecked());
    ui->widC_22->setstate(btn->isChecked());
    ui->widC_26->setstate(btn->isChecked());
    ui->widC_28->setstate(btn->isChecked());
    ui->widC_38->setstate(btn->isChecked());
    ui->widC_43->setstate(btn->isChecked());
    ui->widC_45->setstate(btn->isChecked());
    ui->widC_55->setstate(btn->isChecked());
    ui->widC_60->setstate(btn->isChecked());
    ui->widC_10->setstate(btn->isChecked());
    ui->widC_72->setstate(btn->isChecked());
    ui->widC_76->setstate(btn->isChecked());
}
//电磁离合器DO6封装
void FunctionTest::DO6()
{
    offTimer->start(1000);
    connect(offTimer, &QTimer::timeout,[=](){
        if((offCount>1)&&(Electromagnetic_Cur>40)){
            m_IO->set_channel(5, true);
            //ui->DO_5->setChecked(true);
            sleep(100);
            m_IO->set_channel(6, false);
            //ui->DO_6->setChecked(false);
            offTimer->stop();
            offCount=0;
        }
        qDebug()<<offCount;
        offCount++;
        if((Electromagnetic_Cur>40) && (offCount>3)){
            m_AnPower->powerState(false);
            offCount=0;
            offTimer->stop();
        }
    });
}
//电机上电与掉电
void FunctionTest::Electric_machine_electrify_outage()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender()); // 获取发送信号的按钮

    if (btn && btn->isChecked())
    {
        if(vflag==true) m_Power->powerStart26();
        if(vflag==false) m_Power->powerStart110();
    }
    else
    {
        m_Power->powerStop();
    }
    ui->widC_13->setstate(btn->isChecked());
    ui->widC_20->setstate(btn->isChecked());
    ui->widC_32->setstate(btn->isChecked());
    ui->widC_49->setstate(btn->isChecked());
    ui->widC_63->setstate(btn->isChecked());
    ui->widC_80->setstate(btn->isChecked());
    ui->widC_85->setstate(btn->isChecked());
    ui->widC_86->setstate(btn->isChecked());
    ui->widC_89->setstate(btn->isChecked());
}
//电机正传
void FunctionTest::Electric_machine_forward_set()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender()); // 获取发送信号的按钮
    if(btn && btn->isChecked())
    {
        if(btn->objectName() == "pushButton_62" || btn->objectName() == "pushButton_24")
        {
            QMessageBox::StandardButton rb = QMessageBox::question(this,  "提示",  "测试方向机功率过大，请打开风扇");
        }
        qDebug()<<"正传开始";
        timer1->start(1000);
    }
    else{
        qDebug()<<"正传关闭";
        timer1->stop();
        currentSecond = 0;
        ui->linC_16->setText("");
        ui->linC_21->setText("");
        ui->linC_39->setText("");
        ui->linC_49->setText("");
        sleepmsec(10,true);
        m_DA->setVol_channel(1,0);
        ui->lineEdit_2->setText("");
    }
    m_IO->set_channel(2, btn->isChecked());
    ui->widC_14->setstate(btn->isChecked());
    ui->widC_30->setstate(btn->isChecked());
    ui->widC_47->setstate(btn->isChecked());
    ui->widC_64->setstate(btn->isChecked());
    ui->widC_81->setstate(btn->isChecked());
    ui->widC_83->setstate(btn->isChecked());
    ui->widC_87->setstate(btn->isChecked());
    ui->widC_90->setstate(btn->isChecked());
}
//电机反转
void FunctionTest::Electric_machine_fan_set()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(btn && btn->isChecked())
    {
        if(btn->objectName() == "pushButton_63" || btn->objectName() == "pushButton_25")
        {
            QMessageBox::StandardButton rb = QMessageBox::question(this,  "提示",  "测试方向机功率过大，请打开风扇");
        }
        qDebug()<<"反传开始";
        timer1->start(1000);
    }
    else{
        qDebug()<<"反传关闭";
        timer1->stop();
        currentSecond = 0;
        ui->linC_16->setText("");
        ui->linC_21->setText("");
        ui->linC_39->setText("");
        ui->linC_49->setText("");
        sleepmsec(10,true);
        m_DA->setVol_channel(1,0);
        ui->lineEdit_2->setText("");
    }
    m_IO->set_channel(3,btn->isChecked());
    ui->widC_15->setstate(btn->isChecked());
    ui->widC_31->setstate(btn->isChecked());
    ui->widC_48->setstate(btn->isChecked());
    ui->widC_65->setstate(btn->isChecked());
    ui->widC_82->setstate(btn->isChecked());
    ui->widC_84->setstate(btn->isChecked());
    ui->widC_88->setstate(btn->isChecked());
    ui->widC_91->setstate(btn->isChecked());
}

//液压电磁阀启动   高低
void FunctionTest::Electromagnetic_hydraulic_activate_off_hight_low()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    m_IO->set_channel(11, btn->isChecked());
    ui->widC_23->setstate(btn->isChecked());
    ui->widC_57->setstate(btn->isChecked());

}

//液压电磁阀启动   方向
void FunctionTest::Electromagnetic_hydraulic_activate_off_direction()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    m_IO->set_channel(12, btn->isChecked());
    ui->widC_40->setstate(btn->isChecked());
    ui->widC_73->setstate(btn->isChecked());
}

//液压电机正转
void FunctionTest::Electric_machine_hydraulic_forward_set()
{

    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    m_IO->set_channel(9, btn->isChecked());
    ui->widC_24->setstate(btn->isChecked());
    ui->widC_42->setstate(btn->isChecked());
    ui->widC_59->setstate(btn->isChecked());
    ui->widC_75->setstate(btn->isChecked());
}
//液压电机反转
void FunctionTest::Electric_machine_hydraulic_fan_set()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    m_IO->set_channel(10, btn->isChecked());
    ui->widC_25->setstate(btn->isChecked());
    ui->widC_41->setstate(btn->isChecked());
    ui->widC_58->setstate(btn->isChecked());
    ui->widC_74->setstate(btn->isChecked());
}


void FunctionTest::timeslot()
{
//    endtime = QTime::currentTime();
//    int msec = begintime.msecsTo(endtime);
//    qDebug()<<"切换用时："<<msec;
//    ui->label->setText("换挡时间："+QString::number(msec)+"ms");
//    int dw = ui->DWwid_1->getNumber();
//    QAbstractButton *checkedbtn = ui->buttonGroup->checkedButton();
//    if(DangWeiMap.find(checkedbtn->text()).value()==dw)
//    {
////        timer->stop();
//    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief FunctionTest::SetSensorOutput
/// \param sensorname 传感器名称
/// \param value 传感器输出值
/// \note  操纵压力传感器与补偿压力传感器 值X100  对应1-5V
void FunctionTest::SetSensorOutput(QString sensorname, double value)
{
    if(sensorname=="变速三轴转速传感器"){
        if((value>400)&&(value<=500)){
            value-=14;
        }else if((value>300)&&(value<=400)){
            value-=8;
        }else if((value>200)&&(value<=300)){
            value-=3;
        }else if((value>100)&&(value<=200)){
            value-=1;
        }else if((value>1500)&&(value<=1600)){
            value-=4;
        }else if((value>2300)&&(value<=2400)){
            value-=4;
        }else if((value>3000)&&(value<=3100)){
            value-=4;
        }
    }
    if(sensorname=="涡轮转速传感器"){
        //补偿
        if((value>665)&&(value<=688)){
            value-=23;
        }else{
            value+=1;
        }
    }
    qDebug()<<"传感器"<<sensorname<<value;
    emit valueChangedvalue(sensorname,value);
}
/// \brief FunctionTest::SetDoOutput
/// \param signalname  DO输出的信号命名成
/// \param flag 状态 true false
void FunctionTest::SetDoOutput(QString signalname, bool flag)
{
    qDebug()<<"DO信号"<<signalname<<flag;
    emit valueChangedflag(signalname,flag);
}



/////测试信息录入
/// 负责 录入 部件编号 测试人员信息 部件信息
void FunctionTest::on_btn_1_clicked()
{
    m_info->show();
    m_info->move(this->geometry().center()-QPoint(400,300));
    m_info->InitInformation();
}

////保存检测结果
void FunctionTest::on_btn_3_clicked()
{
    ConclusionShow *conclu = new ConclusionShow(this);
    conclu->move(this->geometry().center()-QPoint(conclu->width()/2,conclu->height()/2)+QPoint(200,50));

    int index = ui->stackedWidget->currentIndex();
    //保存测试结果
    QStringList modelist{"04高低机测试","04方向机测试","04A高低机测试","04A方向机测试"};
    conclu->setMode(modelist[index]);
    if(index ==0 || index == 2)
    {
        if(index ==0)
        {
            conclu->setText_height(ui->lineEdit_13->text(),ui->lineEdit_14->text(),ui->lineEdit_15->text(),ui->lineEdit_6->text());
        }
        else{
            conclu->setText_height(ui->lineEdit_17->text(),ui->lineEdit_18->text(),ui->lineEdit_16->text(),ui->lineEdit_8->text());
        }
        conclu->setTestIndex(0);
    }
    else{
        if(index ==1)
        {
            conclu->setText_direction(ui->lineEdit_4->text());
        }
        else{
            conclu->setText_direction(ui->lineEdit_11->text());
        }
        conclu->setTestIndex(1);

    }
    conclu->show();
}
////系统上电
void FunctionTest::on_btn_2_clicked()
{
    bool flag = ui->btn_2->isChecked();

    m_IO->set_channel(1,flag);
    //flag ? ui->btn_2->setText("系统断电") : ui->btn_2->setText("系统上电") ;


}

void FunctionTest::sleepmsec(int msec,bool flag)
{
    QTime delayTime = QTime::currentTime().addMSecs(msec);
    /* lyh delete -> 避免有拉拽小票冲突 */
    if(flag)
    {
        while( QTime::currentTime() < delayTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
    else
    {
        while( QTime::currentTime() < delayTime)
        {

        }
    }


}




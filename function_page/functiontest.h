#ifndef FUNCTIONTEST_H
#define FUNCTIONTEST_H

#include <QWidget>
#include <QPushButton>
#include "globalVariable.h"
#include <QTime>
#include "conclusionshow.h"
#include "testinformation.h"
namespace Ui {
class FunctionTest;
}

class FunctionTest : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionTest(QWidget *parent = nullptr);
    ~FunctionTest();


    void InitButtonGroup();
    void PageInit();
    void InitButtonPage();


    void timeslot();
    void sleepmsec(int msec,bool flag);



private:
    void SetSensorOutput(QString sensorname , double value);
    void SetDoOutput(QString signalname , bool flag);
    void ButtonGroupCLicked(QAbstractButton *btn);

    //计算时间
    void updateTimer();
    //电磁控制
    void Electromagnetic_electrify_outage();  //电磁上电与掉电
    void Electromagnetic_activate_off();      //电磁启动与关闭
    void DO6();                               //电磁离合器分装DO6
    //电机控制
    void Electric_machine_electrify_outage(); //电机上电与掉电
    void Electric_machine_forward_set();      //电机正转
    void Electric_machine_fan_set();          //电机反转

    //液压电磁阀
    void Electromagnetic_hydraulic_activate_off_hight_low();  //液压电磁阀启动与关闭  高低
    void Electromagnetic_hydraulic_activate_off_direction();  //液压电磁阀启动与关闭  方向

    //液压电机
    void Electric_machine_hydraulic_forward_set();  //液压电机正转
    void Electric_machine_hydraulic_fan_set();  //液压电机正转

signals:
    void valueChangedvalue(QString senorname , double value);
    void valueChangedflag(QString senorname , bool flag);
    void valueChangedgear(QString text);


private slots:
    void on_btn_1_clicked();

    void on_btn_3_clicked();

    void on_btn_2_clicked();


private:
    Ui::FunctionTest *ui;

    testinformation *m_info;

    SerialAD *m_AD;
    SerialDA *m_DA;
    SerialIO *m_IO;
    SerialThread *m_Power;
    SerialPowerThread *m_AnPower;

    QTimer *offTimer; //断继电器6计时器
    int offCount;

    QTime begintime;
    QTime endtime;
    QTimer *timer;
    QTimer *timer1;


    int minutes=15;
    int seconds=minutes*60;
    int currentSecond = 0;

};

#endif // FUNCTIONTEST_H

#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QWidget>
#include "custom/mybtn.h"
#include "globalVariable.h"
#include <QPropertyAnimation>
#include <QSpacerItem>
#include <QTimer>

#include "function_page/functiontest.h"   ////检测模块
#include "DataManagement/databasemanag.h"
////数据库模块

//#include "CommunicationThread/dataprocess.h"
//#include "OTHERXT/selfinspection.h"
//#include "OTHERXT/systemhelps.h"


namespace Ui {
class MainInterface;
}

class MainInterface : public QWidget
{
    Q_OBJECT

public:
    explicit MainInterface(QWidget *parent = nullptr);
    ~MainInterface();

private slots:
    void on_toolhide_clicked();

private:
    void timerEvent(QTimerEvent *e);
    void Animation(int index);

private:
    Ui::MainInterface *ui;
    int index = 0;
    QVector<MyBtn*> mbtnVec;

    FunctionTest *m_test;

    SerialAD *m_AD;
    SerialDA *m_DA;
    SerialIO *m_IO;

    SerialThread *m_Power;
    SerialPowerThread *m_AnPower;

    Electromagnetic *electromagnetic;


    DatabaseManag *m_damang;
//    SelfInspection *m_self;
//    SystemHelps *m_help;

//    Dataprocess *m_data; /////加载数据处理
};

#endif // MAININTERFACE_H

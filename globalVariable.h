#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H


#include <QString>
#include <QSql>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDateTime>
#include <QDebug>
#include <QMutex>
#include <QDebug>
#include <QMessageBox>
#include "CommunicationThread/serialAD.h"
#include "CommunicationThread/serialDA.h"
#include "CommunicationThread/serialIO.h"
#include "CommunicationThread/serialthread.h"
#include "CommunicationThread/serialpowerthread.h"
#include "electromagnetic/electromagnetic.h"

#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
#define TIME            qPrintable(QTime::currentTime().toString("HH:mm:ss"))
#define QDATE           qPrintable(QDate::currentDate().toString("yyyy-MM-dd"))
#define DT qPrintable (QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))
extern QSqlDatabase db;
extern QString SBMC;
extern QString RJMC;
extern QStringList ModulePartitionList;
extern QStringList DetectionModuleList;
extern bool vflag;
extern QString reportsavepath;
extern QString page_str;   //保存页面名称



/////测试数据保存类
struct ResultSaving
{
    QString Partnumber;       //部件编号
    QString Testproject;      //测试项目
    QString Testdata;         //测试数据
    QString Testresults;      //测试结论
    QString Tester;           //测试人员
    QString Testtime ;        //测试时间
    QString Partinformation;  //部件信息
};
extern QString LoginUserName;
extern ResultSaving testinfoeresult;   /////保存测试部件的信息 只需要部件编号测试人员和部件信息
extern QVector<ResultSaving> ResultSavingVec;
extern QStringList TabwidgetHeadlist;
extern QStringList TabwidgetWidthlist;
#endif // GLOBALVARIABLE_H


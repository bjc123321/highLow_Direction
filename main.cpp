#include <QFile>
#include <QDebug>
#include <QApplication>
#include "widget.h"

QSqlDatabase db;
QString SBMC;
QString RJMC;
QVector<ResultSaving> ResultSavingVec;
QStringList ModulePartitionList;  ////模块划分
QStringList DetectionModuleList;  ///检测模块划分
ResultSaving testinfoeresult;
QString LoginUserName;
QStringList TabwidgetHeadlist;  //检测数据管理与历史保存数据两个界面tablewidget的列名称
QStringList TabwidgetWidthlist; //检测数据管理界面tablewidget的列宽
QString reportsavepath;  //报告保存位置


bool vflag=false;

void ReadingComfiguraTxt()
{
    QFile file("ConfigDoc.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QStringList strlist {"设备名称","软件名称","模块划分","检测模块"};

    qDebug()<<"打开成功";

    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);

        switch (strlist.indexOf(str.mid(0,4))) {

        case 0:
        {
            SBMC = str.mid(5,str.length()-5).simplified();  //设备名称
            break;
        }
        case 1:
        {
            RJMC = str.mid(5,str.length()-5).simplified();  //软件名称
            break;
        }
        case 2:
        {
            ModulePartitionList = str.mid(5,str.length()-5).simplified().split("&");
            break;
        }
        case 3:
        {
            DetectionModuleList = str.mid(5,str.length()-5).simplified().split("&");
            break;
        }
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    db = QSqlDatabase::addDatabase("QSQLITE","sqlite");
    db.setDatabaseName("data.db");
    if(db.open())
        qDebug()<<"数据库打开成功";

    ReadingComfiguraTxt();

    Widget w;
    w.setMinimumSize(1800,800);
    w.showMaximized();

    TabwidgetHeadlist  << "序号" << "部件编号" << "测试项目"<< "测试数据"<< "测试结论"<< "测试人员"<< "测试时间" << "部件信息";
    TabwidgetWidthlist <<  "80"<< "180" << "260" << "620"<< "130"<< "130"<< "200"<< "200";
    LoginUserName = "null";
    QString path=QCoreApplication::applicationDirPath();
    reportsavepath = path+"/SAVE_FILE/";

    return a.exec();
}

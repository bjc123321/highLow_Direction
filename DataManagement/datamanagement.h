#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include <QWidget>
#include <QDebug>
#include "globalVariable.h"
#include "projectselect.h"
#include "custom/report.h"
#include "custom/databaseoperations.h"

namespace Ui {
class DataManagement;
}

class DataManagement : public QWidget
{
    Q_OBJECT

public:
    explicit DataManagement(QWidget *parent = nullptr);
    ~DataManagement();

    void InitSQL();

private:

    void Showtablewidget();

    void ShowProjectSelect(ProjectSelect *pr, QPushButton *btn, QStringList &strlist, QString btntext);

    QString retteststr(QString str);

private slots:
    void on_btn_1_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_6_clicked();



    void on_btnquery_clicked();

    void on_btn_7_clicked();

    void on_btn_5_clicked();

private:
    Ui::DataManagement *ui;


    ////数据库查询时的条件设定
    QString Testpartstr;         //测试部件
    QString Partnumberstr;       //部件编号
    QString Testprojectstr;      //测试项目
    QString Testerstr;           //测试人员

    QStringList TestpartList;    //测试部件List
    QStringList PartnumberList;  //部件编号List
    QStringList TestprojectList; //测试项目List
    QStringList TesterList;      //测试人员List

    bool ProjectShowflag;

    QVector<ResultSaving> HistoryResultVec;

    QString sqldelete;
    int selectrow;
};

#endif // DATAMANAGEMENT_H

#ifndef DATABASEOPERATIONS_H
#define DATABASEOPERATIONS_H

#include <QString>
#include <QVector>
#include "globalVariable.h"
#include <QTableWidget>
#include <QHeaderView>

//public:
////插入数据
bool InsertData(QString sqlstr);
////查询数据
void QueryData(QString sqlstr , QVector<QVector<QString>> &VecRet);
////删除数据
bool DeleteData(QString sqlstr);
////删除指定行
bool DeleteData(QString tablename , int index);
////修改数据
bool ModifyData(QString sqlstr);

////查询数据  返回结果，不带参数
QVector<QVector<QString>>  QueryData(QString tablename , QString sqlstr);

void QueryData(QString sqlstr , QStringList &stringlist);


void InitTableWidget(QTableWidget *tabwid , QStringList &headstrlist , QStringList headwidthlist);

void SetItemString(QTableWidget *tabwid , int row, int coulm, QString str);

void GetTablewidgetString(QVector<QVector<QString>> &vec , QTableWidget *tabwid);


//private:
//    QSqlDatabase db;

//};

#endif // DATABASEOPERATIONS_H

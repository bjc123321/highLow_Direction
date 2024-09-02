#include "databaseoperations.h"

//插入
bool /*DatabaseOperations::*/InsertData(QString sqlstr)
{
    QSqlQuery query(QSqlDatabase::database("sqlite"));
    query.prepare(sqlstr);
    return query.exec(sqlstr);
}
//查询
void /*DatabaseOperations::*/QueryData(QString sqlstr, QVector<QVector<QString> > &VecRet)
{
    QSqlQuery query(QSqlDatabase::database("sqlite"));
    query.exec(sqlstr);
    while(query.next()){//循环执行每行
        QVector<QString> strV;
        for(int i = 0 ; i > -1 ; i++)
        {
            QString va = query.value(i).toString();
            if(va.length() ==0)
                break;
            strV.push_back(va);
        }
        VecRet.push_back(strV);
    }
}
//删除
bool /*DatabaseOperations::*/DeleteData(QString sqlstr)
{
    QSqlQuery query(QSqlDatabase::database("sqlite"));
    query.prepare(sqlstr);
    return query.exec(sqlstr);
}
//删除
bool /*DatabaseOperations::*/DeleteData(QString tablename, int  index)
{
//    QSqlTableModel  *model = new QSqlTableModel(nullptr,db);
//    model->setTable(tablename);//执行sql查询
//    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    model->select();
//    model->removeRow(index);
//    bool a = model->submitAll();//否则提交，在数据库中删除该行
//    return a;
}
//修改
bool /*DatabaseOperations::*/ModifyData(QString sqlstr)
{
    QSqlQuery query(QSqlDatabase::database("sqlite"));
    query.prepare(sqlstr);
    return query.exec(sqlstr);
}

QVector<QVector<QString> > /*DatabaseOperations::*/QueryData(QString tablename, QString sqlstr)
{
    QString sql;
    sql += "PRAGMA table_info('"+tablename+"')";
    QSqlQuery querypra(QSqlDatabase::database("sqlite"));
    querypra.exec(sql);
    int index = 0;
    while(querypra.next()){//循环执行每行
        index++;
    }

    QVector<QVector<QString> > VecRet;
    QSqlQuery query(QSqlDatabase::database("sqlite"));
    query.exec(sqlstr);
    while(query.next()){//循环执行每行
        QVector<QString> strV;
        for(int i = 0 ; i < index ; i++)
        {
            QString va = query.value(i).toString();
            if(va.length() == 0)
                strV.push_back("");
            else
                strV.push_back(va);
        }
        VecRet.push_front(strV);
    }
    return VecRet;
}

void InitTableWidget(QTableWidget *tabwid, QStringList &headstrlist, QStringList headwidthlist)
{
    tabwid->setStyleSheet("QTableWidget{background-color:rgba(124, 152, 182,0.8);border:none;}"
                          "QTableWidget::item{color:rgb(255,255,255)}"
                          "QTableWidget::item:hover{background-color:rgb(92,188,227,200)}"
                          "QTableWidget::item:selected{background-color:rgb(0, 85, 127)}");
    ////设置表头
    tabwid->setColumnCount(headwidthlist.length()); //设置列数
    tabwid->horizontalHeader()->setDefaultSectionSize(150);
    tabwid->horizontalHeader()->setSectionsClickable(false); //设置表头不可点击（默认点击后进行排序）

    for(int j = 0 ; j<headwidthlist.length() ; j++)
    {
        QString str = headwidthlist[j];
        tabwid->setColumnWidth(j,str.toInt());
    }

    tabwid->setHorizontalHeaderLabels(headstrlist);

    //设置表头字体加粗
    QFont font = tabwid->horizontalHeader()->font();
    font.setBold(true);
    font.setPixelSize(15);
    tabwid->horizontalHeader()->setFont(font);
    tabwid->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{background:rgb(78, 132, 161);color:#FFFFFF}"); //设置表头背景色

    tabwid->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    tabwid->setFrameShape(QFrame::NoFrame); //设置无边框
    tabwid->setShowGrid(true); //设置显示格子线
    tabwid->verticalHeader()->setVisible(false); //设置垂直头不可见
    tabwid->setSelectionMode(QAbstractItemView::SingleSelection); //不可多选
    tabwid->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为时每次选择一行
    tabwid->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    tabwid->horizontalHeader()->setFixedHeight(32); //设置表头的高度
    tabwid->setFocusPolicy(Qt::NoFocus);

    font.setBold(false);
    font.setPixelSize(18);
    tabwid->setFont(font);
    tabwid->verticalHeader()->setDefaultSectionSize(42);

    //    tabwid->setAlternatingRowColors(true); // 隔行变色
    //    tabwid->setPalette(QPalette(QColor(78, 132, 161)));
    //    tabwid->setFocusPolicy(Qt::NoFocus);

}
void SetItemString(QTableWidget *tabwid, int row, int coulm, QString str)
{
    tabwid->setItem(row,coulm,new QTableWidgetItem(str));
    tabwid->item(row,coulm)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //设置居中显示
}
void GetTablewidgetString(QVector<QVector<QString> > &vec, QTableWidget *tabwid)
{
    int row = tabwid->rowCount();
    int col = tabwid->columnCount();
    for(int i = 0 ; i < row ; i++)
    {
        QVector<QString> rowvec;
        for(int j = 0 ; j < col ; j++)
        {
            rowvec.push_back(tabwid->item(i,j)->text());
        }
        vec.push_back(rowvec);
    }
}

void QueryData(QString sqlstr, QStringList &stringlist)
{
    QSqlQuery query(QSqlDatabase::database("sqlite"));
    query.exec(sqlstr);
    while(query.next()){//循环执行每行
        stringlist.push_back(query.value(0).toString());
    }
}

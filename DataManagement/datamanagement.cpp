#include "datamanagement.h"
#include "ui_datamanagement.h"

///按时间查询
///按工作方查询（火控系统联调试验装置有工作方式）
///按部件查询
///按编号查询
///按项目查询
///按人员查询

DataManagement::DataManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataManagement)
{
    ui->setupUi(this);

    //TabwidgetHeadlist TabwidgetWidthlist全局变量定义的 在main.cpp里初始化的
    InitTableWidget(ui->tableWidget,TabwidgetHeadlist,TabwidgetWidthlist);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);

    QDate date1 = QDate::currentDate().addDays(1);
    QDate date2 = QDate::currentDate().addDays(-1);
    QTime time = QTime::currentTime();
    ui->startdatetime->setYear(date2.year());
    ui->startdatetime->setMonth(date2.month());
    ui->startdatetime->setDay(date2.day());
    ui->startdatetime->setHour(time.hour());
    ui->startdatetime->setMin(time.minute());
    ui->startdatetime->setSec(time.second());
    ui->enddatetime->setYear(date1.year());
    ui->enddatetime->setMonth(date1.month());
    ui->enddatetime->setDay(date1.day());
    ui->enddatetime->setHour(time.hour());
    ui->enddatetime->setMin(time.minute());
    ui->enddatetime->setSec(time.second());

    ProjectShowflag = false;
    selectrow = -1;

    connect(ui->tableWidget,&QTableWidget::cellClicked,[=](int row, int column){selectrow = row;});

    ui->gro_2->hide();

    InitSQL();
}

DataManagement::~DataManagement()
{
    delete ui;
}

void DataManagement::InitSQL()
{
    TestpartList.clear();
    QString sql{"select distinct testparts from testdata"};
    QueryData(sql,TestpartList);

    PartnumberList.clear();
    sql = {"select distinct partnumber from testdata"};
    QueryData(sql,PartnumberList);

    TestprojectList.clear();
    sql = {"select distinct testproject from testdata"};
    QueryData(sql,TestprojectList);

    TesterList.clear();
    sql = {"select distinct tester from testdata"};
    QueryData(sql,TesterList);
}

void DataManagement::Showtablewidget()
{   
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    int size = HistoryResultVec.length();
    ui->tableWidget->setRowCount(size);
    for(int i = 0 ; i < size ; i++)
    {
        SetItemString(ui->tableWidget,i,0,QString::number(i+1));
        SetItemString(ui->tableWidget,i,1,HistoryResultVec[i].Partnumber);
        SetItemString(ui->tableWidget,i,2,HistoryResultVec[i].Testproject);
        SetItemString(ui->tableWidget,i,3,HistoryResultVec[i].Testdata);
        SetItemString(ui->tableWidget,i,4,HistoryResultVec[i].Testresults);
        SetItemString(ui->tableWidget,i,5,HistoryResultVec[i].Tester);
        SetItemString(ui->tableWidget,i,6,HistoryResultVec[i].Testtime);
        SetItemString(ui->tableWidget,i,7,HistoryResultVec[i].Partinformation);
        //随内容分配行高
        ui->tableWidget->verticalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }
}

void DataManagement::ShowProjectSelect(ProjectSelect *m_pr, QPushButton *btn, QStringList &strlist, QString btntext)
{
    m_pr->show();
    btn->setEnabled(false);
    m_pr->setbtngroup(strlist);
    int heig = m_pr->size().height();
    m_pr->setbtncheckedgroup(btn->text());
    m_pr->move(btn->pos().x()+20,20+btn->pos().y()-heig-10);
    ProjectShowflag = true;

    connect(m_pr,&ProjectSelect::unchecked,[=](){
        btn->setChecked(false);
        btn->setText(btntext);
        btn->setEnabled(true);
        ProjectShowflag = false;
    });

    connect(m_pr,&ProjectSelect::checkedbtnstr,[=](QString str){
        btn->setChecked(true);
        btn->setText(str);
        btn->setEnabled(true);
        ProjectShowflag = false;
    });
}

QString DataManagement::retteststr(QString str)
{
    QString s = str;
    for(int i = 0 ; i < s.length() ; i++)
    {
        if(s.mid(i,1) == ",")
        {
            s.insert(i,"'");
            s.insert(i+2,"'");
            i += 2;
        }
    }
    return s;
}
//测试项目
void DataManagement::on_btn_1_clicked()
{
    TestprojectList.clear();
    QString sql = {"select distinct testproject from testdata"};
    QueryData(sql,TestprojectList);

    if(ProjectShowflag) return;
    ProjectSelect *m_pr = new ProjectSelect(this);
    ShowProjectSelect(m_pr,ui->btn_1,TestprojectList,"测试项目");
}
//测试编号
void DataManagement::on_btn_3_clicked()
{
    PartnumberList.clear();
    QString sql = {"select distinct partnumber from testdata"};
    QueryData(sql,PartnumberList);

    if(ProjectShowflag) return;
    ProjectSelect *m_pr = new ProjectSelect(this);
    ShowProjectSelect(m_pr,ui->btn_3,PartnumberList,"部件编号");
}
//测试人员
void DataManagement::on_btn_4_clicked()
{
    TesterList.clear();
    QString sql = {"select distinct tester from testdata"};
    QueryData(sql,TesterList);

    if(ProjectShowflag) return;
    ProjectSelect *m_pr = new ProjectSelect(this);
    ShowProjectSelect(m_pr,ui->btn_4,TesterList,"测试人员");
}

/////生成EXCEL报告
void DataManagement::on_btn_6_clicked()
{
    //  TabwidgetHeadlist 生成的excel报告的列头
    QVector<QVector<QString>> tablewidgetitemstring;
    GetTablewidgetString(tablewidgetitemstring,ui->tableWidget);
    CreateReport(reportsavepath,DT,TabwidgetHeadlist,tablewidgetitemstring);
}


////数据库查询按键
void DataManagement::on_btnquery_clicked()
{
    //    QString sql{"select * from testdata where datetime between '"+
    //                begDateTime.toString("yyyy-MM-dd hh:mm:ss")+"' and '"+
    //                        endDateTime.toString("yyyy-MM-dd hh:mm:ss")
    //                        +"' and user in ("+username+")"};

    QString sql{"select * from testdata where"};

    if(ui->gro_1->isChecked()) //如果点击了按日期时间查询 testtime
    {
        QDateTime begDateTime = ui->startdatetime->getDateTime();
        QDateTime endDateTime = ui->enddatetime->getDateTime();
        QString timesql{" testtime between '"+begDateTime.toString("yyyy-MM-dd hh:mm:ss")+"' and '"+
                    endDateTime.toString("yyyy-MM-dd hh:mm:ss")+"'"};
        sql +=  timesql;
    }
    else
    {
        QDateTime begDateTime = QDateTime(QDate{2000,01,01},QTime(0,0,0));
        QDateTime endDateTime =  QDateTime(QDate{2050,01,01},QTime(0,0,0));
        QString timesql{" testtime between '"+begDateTime.toString("yyyy-MM-dd hh:mm:ss")+"' and '"+
                    endDateTime.toString("yyyy-MM-dd hh:mm:ss")+"'"};
        sql +=  timesql;
    }
    Partnumberstr =   retteststr(ui->btn_3->text());
    if(Partnumberstr  != "部件编号")
    {
        sql += " and partnumber in ('"+Partnumberstr+"')";
    }
    Testprojectstr =   retteststr(ui->btn_1->text());
    if(Testprojectstr != "测试项目")
    {
        sql += " and testproject in ('"+Testprojectstr+"')";
    }
    Testerstr =  retteststr(ui->btn_4->text());
    if(Testerstr != "测试人员")
    {
        sql += " and tester in ('"+Testerstr+"')";
    }

    sqldelete = sql;

    /////查询数据库
    auto queryData = QueryData("testdata",sql);

    HistoryResultVec.clear();

    int i = 0 ;
    for( ; i < queryData.length() ; i++)
    {
        ResultSaving res;
        res.Partnumber = queryData[i].at(0);
        res.Testproject = queryData[i].at(1);
        res.Testdata = queryData[i].at(2);
        res.Testresults = queryData[i].at(3);
        res.Tester = queryData[i].at(4);
        res.Testtime = queryData[i].at(5);
        res.Partinformation = queryData[i].at(6);
        HistoryResultVec.push_back(res);
    }
    Showtablewidget();
}

/////删除所有数据
void DataManagement::on_btn_7_clicked()
{
    sqldelete.replace("select *","delete");
    bool a = DeleteData(sqldelete);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

}

////删除选定数据
void DataManagement::on_btn_5_clicked()
{
    if(selectrow == -1)
        return;

    //由于精确到秒，理论上来说，每条数据的检测时间都是唯一的！！！！！
    HistoryResultVec.remove(selectrow);

    QString testtime = ui->tableWidget->item(selectrow,6)->text();
    QString testpro = ui->tableWidget->item(selectrow,2)->text();
    QString sql{"delete from testdata where testtime = '"+testtime + "' and testproject = '"+ testpro + "'"};
    DeleteData(sql);
    selectrow = -1;
    Showtablewidget();
}

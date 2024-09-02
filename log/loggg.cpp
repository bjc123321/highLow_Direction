#include "loggg.h"
#include "ui_loggg.h"

loggg::loggg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loggg)
{
    ui->setupUi(this);

    ui->verticalLay_2->setAlignment(Qt::AlignHCenter); ////居中显示

    QStringList headlist;
    headlist << "序号" << "日期时间" << "日志内容" << "用户名称";
    QStringList widthlist;
    widthlist << "80" << "250" << "750" << "120";
    InitTableWidget(ui->tableWidget,headlist,widthlist);

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    ui->startdatetime->setYear(date.year());
    ui->startdatetime->setMonth(date.month());
    ui->startdatetime->setDay(date.day());
    ui->startdatetime->setHour(time.hour());
    ui->startdatetime->setMin(time.minute());
    ui->startdatetime->setSec(time.second());

    ui->enddatetime->setYear(date.year());
    ui->enddatetime->setMonth(date.month());
    ui->enddatetime->setDay(date.day());
    ui->enddatetime->setHour(time.hour());
    ui->enddatetime->setMin(time.minute());
    ui->enddatetime->setSec(time.second());

    this->InitCheckedButton();

    connect(ui->verticalScrollBar,&QScrollBar::valueChanged,[=](int value){
        ui->checkwidget->move(0,(-1)*value);
    });

    connect(ui->checkwidget,&popupwidget::whellMoveDistance,[=](int distance){
        ui->verticalScrollBar->setValue(ui->verticalScrollBar->value() - distance/20);
    });

    connect(ui->gro_1,&QGroupBox::clicked,[=](){
        if(ui->gro_1->isChecked() || ui->gro_2->isChecked())
            ui->btnquery->setText("查询");
        else
            ui->btnquery->setText("查询所有日志");
    });
    connect(ui->gro_2,&QGroupBox::clicked,[=](){
        if(ui->gro_1->isChecked() || ui->gro_2->isChecked())
            ui->btnquery->setText("查询");
        else
            ui->btnquery->setText("查询所有日志");
    });
}

loggg::~loggg()
{
    delete ui;
}

void loggg::InitCheckedButton()
{
    usernamelist.clear();
    checklist.clear();

    QString sql{"select distinct user from log"};
    auto queryData = QueryData("log",sql);
    for(int i = 0 ; i < queryData.length() ; i++)
        usernamelist.push_back(queryData[i].at(0));

    QFont fon{"SimHei",22,50};
    fon.setBold(true);
    for(int i = 0 ; i < usernamelist.length() ; i++)
    {
        QCheckBox *check = new QCheckBox(usernamelist[i]);
        check->setFont(fon);
        ui->checklayout->addWidget(check);
        checklist.push_back(check);
    }
    int length = checklist.length()*48;
    ui->checkwidget->setGeometry(0,0,353,length);
    ui->verticalScrollBar->setMaximum(length);
}

void loggg::showtablewidget()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    int size = logInVec.length();
    ui->tableWidget->setRowCount(size);
    for(int i = 0 ; i < size ; i++)
    {
        SetItemString(ui->tableWidget,i,0,QString::number(i+1));
        SetItemString(ui->tableWidget,i,1,logInVec[i].datetime);
        SetItemString(ui->tableWidget,i,2,logInVec[i].logcontent);
        SetItemString(ui->tableWidget,i,3,logInVec[i].username);
    }
}

/////日志查询
void loggg::on_btnquery_clicked()
{
    logInVec.clear();

    QDateTime begDateTime = QDateTime{QDate{1900,1,1},QTime{0,0,0,0}};
    QDateTime endDateTime =  QDateTime::currentDateTime();
    QString username;
    if(ui->gro_1->isChecked())
    {
        begDateTime = ui->startdatetime->getDateTime();
        endDateTime = ui->enddatetime->getDateTime();
    }
    if(ui->gro_2->isChecked())
    {
        for(auto a : checklist)
        {
            if(a->isChecked())
            {
                username += "'";
                username += a->text();
                username += "',";
            }
        }
    }
    else
    {
        for(auto a : usernamelist)
        {
            username += "'";
            username += a;
            username += "',";
        }
    }
    username.remove(username.length()-1,1);

    QString sql{"select * from log where datetime between '"+
                begDateTime.toString("yyyy-MM-dd hh:mm:ss")+"' and '"+
                        endDateTime.toString("yyyy-MM-dd hh:mm:ss")
                        +"' and user in ("+username+")"};
    /////查询数据库
    auto queryData = QueryData("log",sql);
    int i = 0 ;
    for( ; i < queryData.length() ; i++)
    {
        logInVec.push_back(logInformation{queryData[i].at(0)
                                          ,queryData[i].at(1),queryData[i].at(2)});
    };

    showtablewidget();
}

#include "projectselect.h"
#include "ui_projectselect.h"

ProjectSelect::ProjectSelect(QWidget *parent) :
    customW(parent),
    ui(new Ui::ProjectSelect)
{
    ui->setupUi(this);

    setMoveflag(false);
    setResizeflag(false);
    setDoubleclickflag(false);

    connect(ui->prowidget,&popupwidget::whellMoveDistance,[=](int distance){
        int y = distance/20;
        ui->verticalScrollBar->setValue(ui->verticalScrollBar->value()-y);
    });

    connect(ui->verticalScrollBar,&QScrollBar::valueChanged,[=](int value){
        qDebug()<<"..........."<<value;
        ui->btnwidget->move(ui->btnwidget->x(),value*(-1));
    });
}

ProjectSelect::~ProjectSelect()
{
    delete ui;
}

void ProjectSelect::setbtngroup(QStringList btnstrlist)
{
    int i = 0 ;
    for(; i < btnstrlist.length() ; i++)
    {
        QPushButton *btn = new QPushButton(ui->btnwidget);
        btn->setGeometry(20,20+i*60,330,50);
        QString text = "    "+btnstrlist[i];
        if(text.length() > 16)
            text = text.insert(16,"\n    ");

        btn->setText(text);
        btn->show();
        btn->setCheckable(true);

        qDebug()<<"text"<<text;
    }

    int length = 80+i*60;
    ui->btnwidget->resize(370-16,length);
    if(length < 460)
    {
        this->resize(370,length+40);
        ui->verticalScrollBar->setMaximum(0);
    }
    else {
        ui->verticalScrollBar->setMaximum(length-400);
    }
}

void ProjectSelect::setbtncheckedgroup(QString btnstrlist)
{
    QList<QPushButton*> btnList = ui->btnwidget->findChildren<QPushButton*>();
    QStringList strlist = btnstrlist.split(",");
    for(auto a : strlist)
    {
        QString text = "    "+a;
        if(text.length() > 16)
            text = text.insert(16,"\n    ");

        for(auto a : btnList)
        {
            if(a->text() == text)
            {
                a->setChecked(true);
                break;
            }
        }
    }
}

//重置
void ProjectSelect::on_btn1_clicked()
{
    QList<QPushButton*> btnList = ui->btnwidget->findChildren<QPushButton*>();
    for(auto a : btnList)
        a->setChecked(false);
}
//确定
void ProjectSelect::on_btn2_clicked()
{
    QString checkedtext="";
    QList<QPushButton*> btnList = ui->btnwidget->findChildren<QPushButton*>();
    for(auto a : btnList)
    {
        if(a->isChecked())
        {
            checkedtext += "'";
            checkedtext += a->text().simplified();
            checkedtext += ",";
            checkedtext = checkedtext.replace(" ","");
        }
    }

    checkedtext.remove(checkedtext.length()-1,1).replace("'","");

    if(checkedtext == "")
        emit unchecked();
    else
        emit checkedbtnstr(checkedtext);
}

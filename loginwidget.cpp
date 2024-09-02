#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    connect(ui->exitbtn,&QPushButton::clicked,[=](){qApp->closeAllWindows();});
    connect(ui->btnPassword,&QPushButton::clicked,[=](){
        ui->btnPassword->isChecked()?
                    ui->passwordine->setEchoMode(QLineEdit::Normal):
                    ui->passwordine->setEchoMode(QLineEdit::Password);
    });

    QSettings Read("config.ini", QSettings::IniFormat);
    Read.beginGroup("LoginInformation");
    //ui->nameline->setText(Read.value("name","admin").toString());
    ui->nameline->setText("7451");
//    ui->passwordine->setText(Read.value("password","").toString());
     ui->passwordine->setText("7451");

    ui->radRemember->setChecked(!ui->passwordine->text().isEmpty());
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

//登录
void LoginWidget::on_loginbtn_clicked()
{
    QString name = ui->nameline->text();
    QString password = ui->passwordine->text();

    auto queryData = /*DatabaseOperations::*/QueryData("login","select * from login");
    int i = 0 ;
    for( ; i < queryData.length() ; i++)
    {
        if(queryData[i].at(0) == name && queryData[i].at(1) == password)
        {
            qDebug()<<"登录成功";
//            if(queryData[i].at(2) == "0")
//                LOGINUSER = AverageUser; ////普通用户
//            else if(queryData[i].at(2) == "1")
//                LOGINUSER = Administrator; ////管理员用户
//            LoginUserName = name;
            emit LoginSuccess();
            RememberPassword();
            SaveTheLog();
            return;
        }
    }
}

void LoginWidget::RememberPassword()
{
    QSettings Write("config.ini", QSettings::IniFormat);

    Write.beginGroup("LoginInformation");
    Write.setValue("name",ui->nameline->text());
    ui->radRemember->isChecked()?
                Write.setValue("password",ui->passwordine->text()):
                Write.setValue("password","");
    Write.endGroup();
}

void LoginWidget::SaveTheLog()
{
//    QMutexLocker locker(&local_mutex); //上锁
//    operainformation.push_back
//            (OperationLogStruct{QDATE,TIME,"用户<"+ui->nameline->text()+">登录"});
}

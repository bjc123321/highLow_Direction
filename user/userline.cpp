#include "userline.h"
#include "ui_userline.h"

userline::userline(QString username,QString passstr,QWidget *parent):
    QWidget(parent),
    ui(new Ui::userline),
    Privilegflag(false)   //账户权限标志位，默认为普通用户
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    setWindowState(Qt::WindowNoState );
    setFocusPolicy(Qt::NoFocus);
    //设置显示的用户名和密码
    lineliststr[0] = username;
    lineliststr[1] = passstr;
    //写入用户名密码
    ui->lineE->setText(username);
    ui->lineE_2->setText(passstr);
    //连接按键点击信号槽
    connect(ui->btn,&QPushButton::clicked,[=]()
    {emit btnClick(Privilegflag,this->pos(),this->objectName());});

    ui->btnOK->hide();

    operaindex = 0;
}

userline::~userline()
{
    delete ui;
}

void userline::SetText(QList<QString> &textlist)
{
    lineliststr[0] = textlist[0];
    lineliststr[1] = textlist[1];
    RestorState();
}

/// \note 执行修改/删除/恢复密码操作
///  \note index = 0 修改  = 1 删除   = 2 恢复
void userline::operationPassword(QString na, int index)
{
    if(na != this->objectName())
        return;

    operaindex = index;

    if(index == 0 )
    {
        ui->lineE->setEnabled(true);
        ui->lineE->setFocus();
        ui->lineE_2->setEnabled(true);
        ui->lineE_2->setFocus();
        ui->btnOK->show();
    }
    if(index == 1 )
    {
        ui->lineE->setEnabled(true);
        ui->lineE->clear();
        ui->lineE_2->setEnabled(true);
        ui->lineE_2->clear();
    }
    if(index == 2 )
    {
        if(linliststrVec.length() == 0)
            return;
        //将密码修改成上一次的值，直接执行修改程序
        previousliststr[0] = linliststrVec[linliststrVec.length()-1].at(0);
        previousliststr[1] = linliststrVec[linliststrVec.length()-1].at(1);
        linliststrVec.erase(linliststrVec.end()-1);
        emit ModifyPassword(lineliststr,previousliststr,Privilegflag);
        return;
    }
}

/// \note 恢复状态
///  \note ==0  ==2
void userline::RestorState(int ind)
{
    if(ind == 0)
    {
        ui->lineE->setText(lineliststr[0]);
        ui->lineE_2->setText(lineliststr[1]);
    }
    else {
        if(operaindex == 2)
        {
            ui->lineE->setText(previousliststr[0]);
            ui->lineE_2->setText(previousliststr[1]);
        }
        lineliststr[0] = ui->lineE->text();
        lineliststr[1] = ui->lineE_2->text();
    }
    ui->lineE->setEnabled(false);
    ui->lineE_2->setEnabled(false);
    ui->btnOK->hide();
}

void userline::on_btnOK_clicked()
{
    if(operaindex == 0)
    {
        ///修改
        linliststrVec.push_back(lineliststr);
        emit ModifyPassword(lineliststr , QList<QString>{ui->lineE->text(),
                                                         ui->lineE_2->text(),},Privilegflag);
        return;
    }
}

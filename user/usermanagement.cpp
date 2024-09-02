#include "usermanagement.h"
#include "ui_usermanagement.h"

void sleepmsec(int msec,bool flag)
{
    QTime delayTime = QTime::currentTime().addMSecs(msec);
    /* lyh delete -> 避免有拉拽小票冲突 */
    if(flag)
    {
        while( QTime::currentTime() < delayTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
    else
    {
        while( QTime::currentTime() < delayTime)
        {

        }
    }


}


UserManagement::UserManagement(QWidget *parent) :
    customW(parent),
    ui(new Ui::UserManagement)
{
    ui->setupUi(this);
    this->setDoubleclickflag(false);
    this->setResizeflag(false);
    this->setMoveflag(false);

    Init_ReadPassWord();

    connect(this,&UserManagement::mousePress,[=](){
        ui->widget->move(1200,0);
    });

    connect(ui->toolDel,&QToolButton::clicked,[=](){
        //删除密码
        deletePassword(ui->widget->objectName());
        ui->widget->move(1200,0);
    });
    connect(ui->toolMod,&QToolButton::clicked,[=](){
        emit OperaInstruction(ui->widget->objectName(),0);
        ui->widget->move(1200,0);
    });
    connect(ui->toolRev,&QToolButton::clicked,[=](){
        emit OperaInstruction(ui->widget->objectName(),2);
        ui->widget->move(1200,0);
    });
}

UserManagement::~UserManagement()
{
    delete ui;
}

//从数据库读取密码，并显示
void UserManagement::Init_ReadPassWord()
{
    PassVec.clear();
    uservect.clear();
    managerindex = 0;
    ordinaryindex = 0;

    //查询得到所有用户名和密码和权限信息
    QueryData("select * from login",PassVec);
    for(int i = 0 ; i < PassVec.length() ; i++)
    {
        QString username = PassVec[i].at(0);
        QString password = PassVec[i].at(1);
        QString permisson = PassVec[i].at(2);
        userline *m_use;
        if(permisson == "1")
        {
            if(username == "工作账户" || username == "维修账户" )
                continue;
            //管理员账户,标志位置true
            m_use = new userline(username,password,ui->passframe);
            m_use->Privilegflag = true;
            m_use->move(10,10+80*managerindex);
            managerindex++;
        }
        if(permisson == "0")
        {
            //普通账户
            m_use = new userline(username,password,ui->passframe_2);
            m_use->move(10,10+80*ordinaryindex);
            ordinaryindex++;
        }

        m_use->show();

        connect(m_use,&userline::btnClick,[=](bool Privilegflag , QPoint pos , QString obname){
            ReceiveUserClick(Privilegflag , pos , obname);});

        connect(this,&UserManagement::OperaInstruction,[=]
                (QString name , int index){m_use->operationPassword(name,index);
        });

        connect(this,&UserManagement::RestoreTheState,[=](int index){m_use->RestorState(index);});

        connect(m_use,&userline::ModifyPassword,[=](QList<QString> oldstr,QList<QString> newstr,bool flag)
        {ModifyPassword(oldstr,newstr,flag);});

        uservect.push_back(m_use);
    }
    ui->passframe->resize(1065,managerindex*80);
    ui->passframe_2->resize(1065,ordinaryindex*80);

    for(int i = 0 ; i < uservect.length() ; i++)
        uservect[i]->setObjectName(QString::number(i));
}
/// \brief UserManagement::ModifyPassword
/// \param olds 旧的用户名密码
/// \param news 新的用户名密码
/// \param flag 用户权限
void UserManagement::ModifyPassword(QList<QString> olds, QList<QString> news, bool flag)
{
    ///如果新用户名或密码为空，返回
    if(news[0].length()==0 || news[1].length()==0 )
    {
        emit RestoreTheState();
        return;
    }
    ///如果新用户名或密码与旧用户名密码相同，返回
    if(news[0]==olds[0] && news[1]==olds[1] )
    {
        emit RestoreTheState();
        return;
    }
    ///判断是否存在相同的用户名和密码
    for(int i = 0 ; i < PassVec.length() ; i++)
    {
        if(PassVec[i].at(0) == news[0] && PassVec[i].at(1) == news[1])
        {
            qDebug()<<"存在相同用户";
            ///弹出提示框>>>>>>>>>>>>>
            ///发送信号 RestoreTheState  //恢复状态
            emit RestoreTheState();
            return;
        }
    }
    QString fl;
    fl = flag? "1" : "0" ;
    ///进行数据库操作，替换数据
    bool a =  ModifyData("update login set authentication = '"+news[0]+
            "', password = '"+news[1]+ "', permissions = '"+fl+"' "
                                                               "where authentication = '"+olds[0]+"'");
    if(a)
    {
        emit RestoreTheState(1);
        ////将olds替换成news
        for(int i = 0 ; i < PassVec.length() ; i++)
        {
            if(PassVec[i].at(0) == olds[0])
            {
                PassVec[i][0] = news[0];
                PassVec[i][1] = news[1];
                PassVec[i][2] = fl;
                break;
            }
        }
    }
}

//删除密码
/// \note 根据widget的objectname，从数组中读取判断要删除的是哪个密码
void UserManagement::deletePassword(QString name)
{
    int index = name.toInt();
    bool flag = DeleteData("login" , index);
    if(!flag)
    {
        return; //删除失败
    }
    uservect[index]->close();
    uservect.remove(index);
    PassVec.remove(index);
    int in = 0 , inn = 0 ;
    for(int i = 0 ; i < uservect.length() ; i++)
    {
        if(uservect[i]->Privilegflag)
        {
            //管理员用户
            uservect[i]->move(10,10+80*in);
            in++;
        }
        else {
            uservect[i]->move(10,10+80*inn);
            inn++;
        }
        uservect[i]->setObjectName(QString::number(i));
    }
    ui->passframe->resize(1065,in*80);
    ui->passframe_2->resize(1065,inn*80);
}

//Qt的滚轮事件
void UserManagement::wheelEvent(QWheelEvent *event)
{
    QPoint pos = event->pos();
    ui->widget->move(1200,0);
    if(ui->widget_2->geometry().contains(pos))
    {
        int widheight = ui->passframe->height()-160;
        int widposition = ui->passframe->y()+(event->delta()/4);
        if(widposition < widheight * (-1))
            widposition = widheight* (-1);
        if(widposition > 0)
            widposition = 0;
        ui->passframe->move(0,widposition);
    }
    else if(ui->widget_3->geometry().contains(pos))
    {
        int widheight = ui->passframe_2->height()-160;
        int widposition = ui->passframe_2->y()+(event->delta()/4);
        if(widposition < widheight * (-1))
            widposition = widheight* (-1);
        if(widposition > 0)
            widposition = 0;
        ui->passframe_2->move(0,widposition);
    }
}

void UserManagement::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit RestoreTheState();
}


///添加验证密码
void UserManagement::on_btnadd_clicked()
{
    emit RestoreTheState();
    QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->widget_4, "geometry");
    pAnimation->setDuration(200);
    pAnimation->setStartValue(QRect(50, 635, 0, 97));
    pAnimation->setEndValue(QRect(50, 635, 1065, 97));
    pAnimation->start();
}
///取消密码添加
void UserManagement::on_btncanel_clicked()
{
    ui->lineE->clear();
    ui->lineEE->clear();
    QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->widget_4, "geometry");
    pAnimation->setDuration(200);
    pAnimation->setStartValue(QRect(50, 635, 1065, 97));
    pAnimation->setEndValue(QRect(1115, 635, 0, 97));
    pAnimation->start();
}

//添加密码
void UserManagement::on_btnOK_clicked()
{
    //首先判断账户名密码是否为空
    QString newname = ui->lineE->text();
    QString newpassword = ui->lineEE->text();
    if(newname.length()==0 || newpassword.length()==0)
        return;

    ///判断是否存在相同的用户名和密码
    for(int i = 0 ; i < PassVec.length() ; i++)
    {
        if(PassVec[i].at(0) == newname && PassVec[i].at(1) ==newpassword)
        {
            qDebug()<<"存在相同用户";
            ///弹出提示框>>>>>>>>>>>>>
            ///发送信号 RestoreTheState  //恢复状态
            on_btncanel_clicked();
            return;
        }
    }
    QString permisson;
    if(ui->radioButton->isChecked())
        permisson = "1";
    else
        permisson = "0";
    //添加到数据库
    bool a =  InsertData
            ("insert into login ('authentication','password','permissions') values "
             "('"+newname+"','"+newpassword+"','"+permisson+"')");
    if(a)
    {
        qDebug()<<"密码添加成功";
        on_btncanel_clicked();
        ///更新显示
        Init_ReadPassWord();
    }
}

/// 接收m_user的按键点击信号
/// \brief UserManagement::ReceiveUserClick
/// \param flag 用户状态
/// \param pos  当前X Y坐标位置
///  \param namestr 当前userline的名称
void UserManagement::ReceiveUserClick(bool flag, QPoint pos , QString namestr)
{
    if(flag)//管理员账户

        ui->widget->move(60,pos.y()+155+ui->passframe->y());
    else
        ui->widget->move(60,pos.y()+445+ui->passframe_2->y());

    ui->widget->setObjectName(namestr);
    emit RestoreTheState();
}

#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QWidget>
#include <QMessageBox>
#include <QTime>
#include "custom/customw.h"
#include "userline.h"
#include "custom/databaseoperations.h"
#include <QPropertyAnimation>

namespace Ui {
class UserManagement;
}

class UserManagement : public customW
{
    Q_OBJECT

public:
    explicit UserManagement(QWidget *parent = nullptr);
    ~UserManagement();

signals:
    // index = 0 修改  = 1 删除   = 2 恢复
    // name 为objectname，当名字相同时执行操作
    void OperaInstruction(QString name , int index);
    //userline恢复状态
    void RestoreTheState(int index = 0 );
    //添加用户
    void Adduser();

private slots:

    void on_btnadd_clicked();

    void on_btncanel_clicked();

    void on_btnOK_clicked();

private slots:
    void ReceiveUserClick(bool flag , QPoint pos, QString namestr);

private:

    void Init_ReadPassWord();
    void ModifyPassword(QList<QString> olds,QList<QString> news,bool flag);
    void deletePassword(QString name);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void museConnect(userline *m_use);
private:
    Ui::UserManagement *ui;
    QVector<QVector<QString>> PassVec;
    QVector<userline*> uservect;
    int managerindex ;
    int ordinaryindex;

};

#endif // USERMANAGEMENT_H

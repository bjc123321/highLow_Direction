#ifndef USERLINE_H
#define USERLINE_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class userline;
}

class userline : public QWidget
{
    Q_OBJECT

public:
    explicit userline(QString username , QString passstr ,QWidget *parent = nullptr);
    ~userline();

    void  SetText(QList<QString> &textlist);
    void  operationPassword(QString na , int index);
    void  RestorState(int ind=0);

    bool Privilegflag; //==false 普通用户  ==true  管理员用户

signals:
    void btnClick(bool Privilegflag , QPoint pos , QString obname);
    void ModifyPassword(QList<QString> oldstr , QList<QString> newstr, bool flag);

private slots:
    void on_btnOK_clicked();

private:
    Ui::userline *ui;

    int operaindex;

    QList<QString> lineliststr{"",""};
    QVector<QList<QString>> linliststrVec;
    QList<QString> previousliststr{"",""} ;
};

#endif // USERLINE_H

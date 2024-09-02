#ifndef MYBTN_H
#define MYBTN_H

#include <QWidget>
#include <QPushButton>
#include "QPainter"
#include <QDebug>

class MyBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit MyBtn(QWidget *parent = nullptr);

    void settext(QString ModuleName);

    void setCheckedState(bool value);

private:
    void paintEvent(QPaintEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    int MouseFLag;  //鼠标状态  1 是悬浮 2 是按下 0 是离开
    QString btntext;
    QMap<QString , QString> MapStrVec;
    bool checkedState;

signals:

public slots:
};

#endif // MYBTN_H

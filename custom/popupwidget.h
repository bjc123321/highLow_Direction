#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QWheelEvent>

class popupwidget : public QWidget
{
    Q_OBJECT
public:
    explicit popupwidget(QWidget *parent = nullptr);

    ////大小调节方向  //左右    //上下   //全部
    enum SizeDirection{LeftRight,UpDown,All,None};
    Q_ENUMS(SizeDirection)

    void setSizeD(const SizeDirection &value);

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
signals:
    void wheelMo(QWheelEvent *event);
    void whellMoveDistance(int distance);
    void doubleclick(QPoint pos);
    void click(QPoint pos);
    void MouseMove(QPoint pos);

    void MouseMoveDistance(int X , int Y);
    void EnterEvent();

private:
    SizeDirection  SizeD;
    QPoint MOUSEPRESSPOINT;
    int mouseState;
    bool MouseMoveFlag;
public slots:
};

#endif // POPUPWIDGET_H

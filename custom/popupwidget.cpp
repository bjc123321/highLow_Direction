#include "popupwidget.h"

popupwidget::popupwidget(QWidget *parent) : QWidget(parent)
{
    SizeD = None;
    mouseState = 0;
    MouseMoveFlag = false;
    this->setMouseTracking(true);
}

void popupwidget::wheelEvent(QWheelEvent *event)
{
    emit wheelMo(event);
    emit whellMoveDistance(event->delta());
}

void popupwidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleclick(event->pos());
}

void popupwidget::mousePressEvent(QMouseEvent *event)
{
    mouseState = 1;
    MOUSEPRESSPOINT = event->pos();
    emit click(event->pos());
}

void popupwidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    MouseMoveFlag = true;

    if(mouseState != 1)
        emit EnterEvent();

    if(mouseState == 1)
    {
        emit MouseMoveDistance(event->pos().x() - MOUSEPRESSPOINT.x()
                               ,event->pos().y() - MOUSEPRESSPOINT.y());
        MOUSEPRESSPOINT = event->pos();
    }
    emit MouseMove(event->pos());
}

void popupwidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouseState = 0 ;
}
void popupwidget::enterEvent(QEvent *event)
{
    if(SizeD == LeftRight)
    {
        this->setCursor(Qt::SplitHCursor);
    }
    else if(SizeD == UpDown)
    {
        this->setCursor(Qt::SplitVCursor);
    }
    else  if(SizeD == All)
    {
        this->setCursor(Qt::SizeFDiagCursor);
    }
    else  if(SizeD == None)
    {
        this->setCursor(Qt::ArrowCursor);
    }
}
void popupwidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    this->setCursor(Qt::ArrowCursor);
    MouseMoveFlag = false;
}

void popupwidget::setSizeD(const SizeDirection &value)
{
    SizeD = value;
}

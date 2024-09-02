#include "customw.h"
#include <QDebug>

customW::customW(QWidget *parent) : QWidget(parent),
    DoubleClickFlag(true),
    ResizeFlag(true),
    MoveFlag(true),
    ResDirect(CENTER),
    MinHeight(300),
    MinWidth(200),
    FrameShare(10)
{
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    setWindowState(Qt::WindowNoState );
    setFocusPolicy(Qt::NoFocus);
    m_rightPress = false;
    m_bLeftPress = false;
    startTimer(50);
}

void customW::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->y() < 30 && DoubleClickFlag)
        emit  OnClicked(2);
}

void customW::mousePressEvent(QMouseEvent *event)
{
    //鼠标在窗口的相近位置整数型
    m_iCalCursorPos = CalCursorPos(event->pos(),CalCursorCol(event->pos()));
    if (event->button() == Qt::LeftButton && !this->isMaximized() )
    {
        if(m_iCalCursorPos != CENTER && ResizeFlag)//CENTER=22
        {
            if(ResDirect == CENTER)
                m_bLeftPress = true;
            else if(ResDirect == TOP && m_iCalCursorPos == TOP)
                m_bLeftPress = true;
            else if(ResDirect == BUTTOM && m_iCalCursorPos == BUTTOM)
                m_bLeftPress = true;
            else if(ResDirect == LEFT && m_iCalCursorPos == LEFT)
                m_bLeftPress = true;
            else if(ResDirect == RIGHT && m_iCalCursorPos == RIGHT)
                m_bLeftPress = true;
        }
        else if(MoveFlag)
        {
            m_rightPress = true;
            setCursor(Qt::SizeAllCursor);
        }
    }
    m_rtPreGeometry = geometry();//窗口几何矩形
    m_currentPosition = this->pos();
    m_ptViewMousePos = event->globalPos();//鼠标按下时的全局点

    emit mousePress(event);
}

void customW::mouseMoveEvent(QMouseEvent *event)
{
    //窗体不是最大的话就改变鼠标的形状
    if(Qt::WindowMaximized != windowState())
    {
        setCursorShape(CalCursorPos(event->pos(),CalCursorCol(event->pos())));
    }
    //获取当前的点，这个点是全局的
    QPoint ptCurrentPos = QCursor::pos();
    //计算出移动的位置，当前点 - 鼠标左键按下的点
    QPoint ptMoveSize = ptCurrentPos - m_ptViewMousePos;
    QRect  rtTempGeometry = m_rtPreGeometry;//窗口矩形
    if(m_bLeftPress)
    {
        switch(m_iCalCursorPos)
        {
        case TOPLEFT:
            rtTempGeometry.setTopLeft(m_rtPreGeometry.topLeft()+ptMoveSize);
            break;
        case TOP:
            rtTempGeometry.setTop(m_rtPreGeometry.top()+ptMoveSize.y());
            break;
        case TOPRIGHT:
            rtTempGeometry.setTopRight(m_rtPreGeometry.topRight()+ptMoveSize);
            break;
        case LEFT:
            rtTempGeometry.setLeft(m_rtPreGeometry.left()+ptMoveSize.x());
            break;
        case RIGHT:
            rtTempGeometry.setRight(m_rtPreGeometry.right()+ptMoveSize.x());
            break;
        case BUTTOMLEFT:
            rtTempGeometry.setBottomLeft(m_rtPreGeometry.bottomLeft()+ptMoveSize);
            break;
        case BUTTOM:
            rtTempGeometry.setBottom(m_rtPreGeometry.bottom()+ptMoveSize.y());
            break;
        case BUTTOMRIGHT:
            rtTempGeometry.setBottomRight(m_rtPreGeometry.bottomRight()+ptMoveSize);
            break;
        default:
            break;
        }
        //移动窗体，如果比最小窗体大，就移动
        if(rtTempGeometry.width() >= MinWidth && rtTempGeometry.height() >= MinHeight)
            setGeometry(rtTempGeometry);

        emit CurrentGeometry(rtTempGeometry);
    }
    else if(m_rightPress)
    {
        setCursor(Qt::SizeAllCursor);
        this->move(m_currentPosition + ptMoveSize);
    }
}

void customW::mouseReleaseEvent(QMouseEvent *event)
{
    m_bLeftPress = false;
    m_rightPress = false;
    setCursor(Qt::ArrowCursor);
    QApplication::restoreOverrideCursor();
}
void customW::timerEvent(QTimerEvent *e)
{
    emit timeout();
    emit CurrentDateTime(QDateTime::currentDateTime());
    emit CurrentDateTimeString(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}
int customW::CalCursorCol(QPoint pt)
{
    return (pt.x() < FrameShare ?
                1 : ((pt.x() > this->width() - FrameShare) ? 3 : 2));//返回整数型位置个位
}

int customW::CalCursorPos(QPoint pt, int colPos)
{
    return ((pt.y() < FrameShare ?
                 10 : ((pt.y() > this->height() - FrameShare) ? 30 : 20)) + colPos);//返回整数型位置十位和个位
}

void customW::setCursorShape(int CalPos)
{
    Qt::CursorShape cursor;
    switch(CalPos)
    {
    case TOPLEFT:
    case BUTTOMRIGHT:
        cursor = Qt::SizeFDiagCursor;
        break;
    case TOPRIGHT:
    case BUTTOMLEFT:
        cursor = Qt::SizeBDiagCursor;
        break;
    case TOP:
    case BUTTOM:
        cursor = Qt::SizeVerCursor;
        break;
    case LEFT:
    case RIGHT:
        cursor = Qt::SizeHorCursor;
        break;
    default:
        cursor = Qt::ArrowCursor;
        break;
    }
    setCursor(cursor);
}

void customW::OnClicked( int index)
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (index == 1)
        {
            pWindow->showMinimized();
        }
        else if (index == 2)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (index == 3)
        {
            pWindow->close();
        }
    }
}

bool customW::getDoubleclickflag() const
{
    return DoubleClickFlag;
}

void customW::setDoubleclickflag(bool value)
{
    DoubleClickFlag = value;
}

bool customW::getMoveflag() const
{
    return MoveFlag;
}

void customW::setMoveflag(bool value)
{
    MoveFlag = value;
}

bool customW::getResizeflag() const
{
    return ResizeFlag;
}

void customW::setResizeflag(bool value)
{
    ResizeFlag = value;
}
void customW::setResDirect(const int &value)
{
    ResDirect = value;
}

int customW::getMinHeight() const
{
    return MinHeight;
}

void customW::setMinHeight(int value)
{
    MinHeight = value;
}

int customW::getMinWidth() const
{
    return MinWidth;
}

void customW::setMinWidth(int value)
{
    MinWidth = value;
}

int customW::getFrameShare() const
{
    return FrameShare;
}

void customW::setFrameShare(int value)
{
    FrameShare = value;
}

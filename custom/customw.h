#ifndef CUSTOMW_H
#define CUSTOMW_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QDateTime>
#include <QApplication>

enum {
    TOPLEFT = 11,
    TOP = 12,
    TOPRIGHT = 13,
    LEFT = 21,
    CENTER = 22,
    RIGHT = 23,
    BUTTOMLEFT = 31,
    BUTTOM = 32,
    BUTTOMRIGHT = 33,
    ToMinimize = 1,
    ToMaximize = 2,
    ToClosed = 3
};

class customW : public QWidget
{
    Q_OBJECT
public:

    explicit customW(QWidget *parent = nullptr);

    Q_PROPERTY(bool DoubleClickFlag READ getDoubleclickflag WRITE setDoubleclickflag)
    Q_PROPERTY(bool MoveFlag READ getMoveflag WRITE setMoveflag)
    Q_PROPERTY(bool ResizeFlag READ getResizeflag WRITE setResizeflag)
    Q_PROPERTY(int ResDirect WRITE setResDirect)
    Q_PROPERTY(int MinHeight READ getMinHeight WRITE setMinHeight)
    Q_PROPERTY(int MinWidth READ getMinWidth WRITE setMinWidth)
    Q_PROPERTY(int FrameShare READ getFrameShare WRITE setFrameShare)

protected:

    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void timerEvent(QTimerEvent *e);

private:
    int CalCursorCol(QPoint pt);                 //计算鼠标X的位置
    int CalCursorPos(QPoint pt, int colPos);     //计算鼠标的位置
    void setCursorShape(int CalPos);             //设置鼠标对应位置的形状
public:
    void OnClicked( int index);

    bool getDoubleclickflag() const;
    void setDoubleclickflag(bool value);

    bool getMoveflag() const;
    void setMoveflag(bool value);

    bool getResizeflag() const;
    void setResizeflag(bool value);

    void setResDirect(const int &value);

    int getMinHeight() const;
    void setMinHeight(int value);

    int getMinWidth() const;
    void setMinWidth(int value);

    int getFrameShare() const;
    void setFrameShare(int value);

private:
    int     m_iCalCursorPos;
    bool    m_bLeftPress;
    bool    m_rightPress;
    QRect   m_rtPreGeometry;
    QPoint  m_currentPosition;
    QPoint  m_ptViewMousePos;

    bool DoubleClickFlag; //是否允许双击标题栏最大化最小化
    bool MoveFlag ;       //是否允许移动
    bool ResizeFlag;      //是否允许修改尺寸
    int  ResDirect;       //在哪个方向上可以修改尺寸
    int MinHeight;        //允许的最小高度
    int MinWidth;         //允许的最小宽度
    int FrameShare;       //鼠标距离边缘的距离

signals:
    void mousePress(QMouseEvent *event);
    void widgetResizeChange();
    void timeout();
    void CurrentDateTime(QDateTime datetime);
    void CurrentDateTimeString(QString datetime);
    void CurrentGeometry(QRect rect);
};

#endif // CUSTOMW_H

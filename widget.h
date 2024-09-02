#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDateTime>
#include "globalVariable.h"
#include "custom/customw.h"
#include "maininterface.h"
#include "custom/databaseoperations.h"

namespace Ui {
class Widget;
}

class Widget : public customW
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
    void timerEvent(QTimerEvent *e);
    void exitlog();
    void LoginSuccess();

signals:
    void centerwidget(QSize size);

private:
    Ui::Widget *ui;
    MainInterface *m_main;
};

#endif // WIDGET_H

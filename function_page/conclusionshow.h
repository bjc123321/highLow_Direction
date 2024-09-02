#ifndef CONCLUSIONSHOW_H
#define CONCLUSIONSHOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QDebug>
#include "globalVariable.h"
#include "custom/databaseoperations.h"
#include "QMessageBox"
#include <QLineEdit>
#include <QTextEdit>
#include <QRadioButton>
#include "custom/report.h"


namespace Ui {
class ConclusionShow;
}

class ConclusionShow : public QDialog
{
    Q_OBJECT

public:
    explicit ConclusionShow(QWidget *parent = nullptr);
    ~ConclusionShow();

    void setMode(const QString &value);
    void setTestIndex(int index);
    void setText_height(const QString &text1,const QString &text2,const QString &text3,const QString &text4);
    void setText_direction(const QString &text4);
private slots:
    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_7_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

private:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    void ShowTableWidget();

    void btnSave(int index);

private:
    Ui::ConclusionShow *ui;

    QRect   m_rtPreGeometry;
    QPoint  m_currentPosition;
    QPoint  m_ptViewMousePos;
    bool state;
    int selectrow;
    QString mode;
};

#endif // CONCLUSIONSHOW_H

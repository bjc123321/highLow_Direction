#ifndef LOGGG_H
#define LOGGG_H

#include <QWidget>
#include "custom/databaseoperations.h"
#include "globalVariable.h"
//#include "dataprocess.h"
#include <QCheckBox>

struct logInformation
{
    QString datetime;
    QString logcontent;
    QString username;
};

namespace Ui {
class loggg;
}

class loggg : public QWidget
{
    Q_OBJECT

public:
    explicit loggg(QWidget *parent = nullptr);
    ~loggg();

    void InitCheckedButton();

private:
    void showtablewidget();

private slots:
    void on_btnquery_clicked();

private:
    Ui::loggg *ui;

    QList<QString> usernamelist;
    QList<QCheckBox*> checklist;

    QVector<logInformation> logInVec;
};

#endif // LOG_H

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QSettings>
#include "globalVariable.h"
#include "custom/databaseoperations.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
    void LoginSuccess();

private slots:
    void on_loginbtn_clicked();
    void RememberPassword();
    void SaveTheLog();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H

#ifndef PROJECTSELECT_H
#define PROJECTSELECT_H

#include <QWidget>
#include "custom/customw.h"
#include <QPushButton>
#include "globalVariable.h"
#include "custom/popupwidget.h"
namespace Ui {
class ProjectSelect;
}

class ProjectSelect : public customW
{
    Q_OBJECT

public:
    explicit ProjectSelect(QWidget *parent = nullptr);
    ~ProjectSelect();

    void setbtngroup(QStringList btnstrlist);
    void setbtncheckedgroup(QString btnstrlist);

signals:
    void checkedbtnstr(QString str);
    void unchecked();

private slots:
    void on_btn1_clicked();

    void on_btn2_clicked();

private:
    Ui::ProjectSelect *ui;
};

#endif // PROJECTSELECT_H

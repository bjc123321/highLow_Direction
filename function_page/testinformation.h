#ifndef TESTINFORMATION_H
#define TESTINFORMATION_H

#include <QWidget>
#include "custom/customw.h"
#include "globalVariable.h"

namespace Ui {
class testinformation;
}

class testinformation : public customW
{
    Q_OBJECT

public:
    explicit testinformation(QWidget *parent = nullptr);
    ~testinformation();

    void InitInformation();

private slots:
    void on_btn_1_clicked();

private:
    Ui::testinformation *ui;
};

#endif // TESTINFORMATION_H

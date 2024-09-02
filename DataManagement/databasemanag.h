#ifndef DATABASEMANAG_H
#define DATABASEMANAG_H

#include <QWidget>
#include <QPushButton>
#include "user/usermanagement.h"
#include "log/loggg.h"
#include "DataManagement/datamanagement.h"

namespace Ui {
class DatabaseManag;
}

class DatabaseManag : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseManag(QWidget *parent = nullptr);
    ~DatabaseManag();

    void InitButtonGroup();

private:
    Ui::DatabaseManag *ui;
};

#endif // DATABASEMANAG_H

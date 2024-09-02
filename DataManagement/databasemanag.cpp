#include "databasemanag.h"
#include "ui_databasemanag.h"
#include <qmath.h>
#include <QtMath>

DatabaseManag::DatabaseManag(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseManag)
{
    ui->setupUi(this);
    this->InitButtonGroup();

    DataManagement *m_data = new DataManagement();
    ui->verpage_1->addWidget(m_data);
    ui->verpage_1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_data->show();

    UserManagement *m_user = new UserManagement();
    ui->verpage_3->addWidget(m_user);
    ui->verpage_3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_user->show();

    loggg *m_log = new loggg();
    ui->verpage_2->addWidget(m_log);
    ui->verpage_2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_log->show();


    connect(ui->itemwidget,&NavButtonGroup::buttonClicked,[=](int index){
        ui->stackedWidget->setCurrentIndex(index);
    });
    ui->stackedWidget->setCurrentIndex(0);
}

DatabaseManag::~DatabaseManag()
{
    delete ui;
}

void DatabaseManag::InitButtonGroup()
{
    QStringList strlist;
    strlist<<"检测数据管理"<<"日志管理"<<"用户管理";
    int count = strlist.count();
    ui->itemwidget->clearButton();
    for (int i = 0; i < count; i++) {
        QPushButton *btn = new QPushButton;
        btn->setCheckable(true);
        btn->setText(strlist.at(i));
        btn->setMinimumHeight(38);
        ui->itemwidget->addButton((QAbstractButton *)btn, i);
    }
    ui->itemwidget->setBaseColor(QColor(101, 169, 200));
    ui->itemwidget->setLinePosition(NavButtonGroup::LinePosition_Bottom);
}

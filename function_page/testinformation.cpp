#include "testinformation.h"
#include "ui_testinformation.h"

testinformation::testinformation(QWidget *parent) :
    customW(parent),
    ui(new Ui::testinformation)
{
    ui->setupUi(this);

    setMoveflag(true);
    setDoubleclickflag(false);
    setResizeflag(false);

    testinfoeresult.Partnumber = "001";
    testinfoeresult.Tester = LoginUserName;
    testinfoeresult.Partinformation = "0001";
}

testinformation::~testinformation()
{
    delete ui;
}

void testinformation::InitInformation()
{
    ui->lin_1->setText(testinfoeresult.Partnumber);
    ui->textEdit_1->setText(testinfoeresult.Partinformation);
    ui->textEdit_2->setText(testinfoeresult.Tester);
}

void testinformation::on_btn_1_clicked()
{
    testinfoeresult.Partnumber =  ui->lin_1->text();
    testinfoeresult.Partinformation =  ui->textEdit_1->toPlainText();
    testinfoeresult.Tester =  ui->textEdit_2->toPlainText();

    if(testinfoeresult.Partnumber == "")
        testinfoeresult.Partnumber = " ";
    if(testinfoeresult.Partinformation == "")
        testinfoeresult.Partinformation = " ";
    if(testinfoeresult.Tester == "")
        testinfoeresult.Tester = " ";

    this->hide();
}

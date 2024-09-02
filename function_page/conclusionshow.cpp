#include "conclusionshow.h"
#include "ui_conclusionshow.h"

ConclusionShow::ConclusionShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConclusionShow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    setWindowState(Qt::WindowNoState );
    setFocusPolicy(Qt::NoFocus);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0, 85, 127));
    setPalette(pal);

    state = false;
    selectrow = -1;
    ui->stackedWidget->setCurrentIndex(0);

    //部件编号
    //测试项目
    //测试数据
    //测试结论
    //测试人员
    //测试时间
    //部件信息
    QStringList headlist;
    headlist << "序号" << "部件编号" << "测试项目"<< "测试数据"<< "测试结论"<< "测试人员"<< "测试时间"<< "部件信息";
    QStringList widthlist;
    widthlist << "80"<< "130" << "200" << "620"<< "130"<< "130"<< "200"<< "200";

    InitTableWidget(ui->tableWidget,headlist,widthlist);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);

    connect(ui->tableWidget,&QTableWidget::cellClicked,[=](int row, int column){selectrow = row;});

    mode = "";

    QVector<QPushButton*>  btnsave{ui->btn_8,ui->btn_9,ui->btn_10,ui->btn_11,ui->btn_12,ui->btn_13
                ,ui->btn_14,ui->btn_15
                ,ui->btn_16,ui->btn_17
                ,ui->btn_18,ui->btn_19};

    for(int i = 0 ; i < btnsave.length() ; i++)
        connect(btnsave[i],&QPushButton::clicked,[=](){this->btnSave(i);});

//    ui->text_4->setText("手动空回量: "+this->height_shou_konghui+"   电动空回量:  "+this->height_diandong_konghui );
//    ui->text_6->setText(this->height_shoulun);

//    ui->text_14->setText("千分表值: "+this->direction_konghui );
//    ui->text_13->setText(ditrction_shoulin);


}

ConclusionShow::~ConclusionShow()
{
    delete ui;
}

void ConclusionShow::mousePressEvent(QMouseEvent *event)
{
    state = true;
    setCursor(Qt::SizeAllCursor);
    m_rtPreGeometry = geometry();//窗口几何矩形
    m_currentPosition = this->pos();
    m_ptViewMousePos = event->globalPos();//鼠标按下时的全局点
}

void ConclusionShow::mouseMoveEvent(QMouseEvent *event)
{
    if(!state)
        return;

    //获取当前的点，这个点是全局的
    QPoint ptCurrentPos = QCursor::pos();
    //计算出移动的位置，当前点 - 鼠标左键按下的点
    QPoint ptMoveSize = ptCurrentPos - m_ptViewMousePos;
    this->move(m_currentPosition + ptMoveSize);
}

void ConclusionShow::mouseReleaseEvent(QMouseEvent *event)
{
    state = false;
    setCursor(Qt::ArrowCursor);
    QApplication::restoreOverrideCursor();
}

void ConclusionShow::ShowTableWidget()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    int size = ResultSavingVec.length();
    ui->tableWidget->setRowCount(size);
    for(int i = 0 ; i < size ; i++)
    {
        SetItemString(ui->tableWidget,i,0,QString::number(i+1));
        SetItemString(ui->tableWidget,i,1,ResultSavingVec[i].Partnumber);
        SetItemString(ui->tableWidget,i,2,ResultSavingVec[i].Testproject);
        SetItemString(ui->tableWidget,i,3,ResultSavingVec[i].Testdata);
        SetItemString(ui->tableWidget,i,4,ResultSavingVec[i].Testresults);
        SetItemString(ui->tableWidget,i,5,ResultSavingVec[i].Tester);
        SetItemString(ui->tableWidget,i,6,ResultSavingVec[i].Testtime);
        SetItemString(ui->tableWidget,i,7,ResultSavingVec[i].Partinformation);
        //随内容分配行高
        ui->tableWidget->verticalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }
}

void ConclusionShow::btnSave(int index)
{
    QLabel *lab = this->findChild<QLabel *>("lab_"+QString::number(index+1));
    QTextEdit *text = this->findChild<QTextEdit *>("text_"+QString::number(index+1));
    QRadioButton *rad1 = this->findChild<QRadioButton *>("rad_"+QString::number((index+1)*2-1));
    QRadioButton *rad2 = this->findChild<QRadioButton *>("rad_"+QString::number((index+1)*2));

    ResultSaving savres;

    savres.Partnumber = testinfoeresult.Partnumber;
    savres.Tester = testinfoeresult.Tester;
    savres.Partinformation = testinfoeresult.Partinformation;
    int ind = lab->text().indexOf("检测");
    savres.Testproject = lab->text();
            //.remove(ind+2,lab->text().length()-ind-2);
    savres.Testdata = text->toPlainText();
    savres.Testtime = STRDATETIME;

    if(rad1->isChecked())
        savres.Testresults ="正常";
    else  if(rad2->isChecked())
        savres.Testresults ="故障";
    else
        savres.Testresults ="未检测";

    QString sql{"insert into testdata ('partnumber','testproject','testdata','testresults','tester','testtime','partinformation')"
                " values ""('"
                +savres.Partnumber+"','"
                +savres.Testproject+"','"
                +savres.Testdata+"','"
                +savres.Testresults+"','"
                +savres.Tester+"','"
                +savres.Testtime+"','"
                +savres.Partinformation+"')"};

    bool ret = InsertData(sql);

    if(!ret)
    {
        QMessageBox msgBox;
        msgBox.setText("保存失败！");
        msgBox.exec();
        return;
    }

    QMessageBox msgBox;
    msgBox.setText(savres.Testproject+"数据及结论保存成功！");
    msgBox.exec();

    ResultSavingVec.push_back(savres);
}

void ConclusionShow::setMode(const QString &value)
{
    mode = value;
    ui->titlelabel->setText(mode+"结论");
}

void ConclusionShow::setTestIndex(int index)
{
    ui->stackedWidget_2->setCurrentIndex(index);
}

void ConclusionShow::setText_height(const QString &text1,const QString &text2,const QString &text3,const QString &text4)
{
        ui->text_4->setText("手动空回量: "+text1+"   电动空回量:  "+text2 );
        ui->text_6->setText(text3);
        ui->text_1->setText("跑合力矩: "+text4);
}

void ConclusionShow::setText_direction(const QString &text4)
{
    ui->text_14->setText("千分表值: "+text4 );
}
void ConclusionShow::on_btn_1_clicked()
{
    ResultSaving savres;
    savres.Partnumber = testinfoeresult.Partnumber;
    savres.Partinformation = testinfoeresult.Partinformation;
    savres.Tester = testinfoeresult.Tester;

    int index = ui->stackedWidget_2->currentIndex();

    QVector<QLabel*> testlineVec;
    QVector<QTextEdit*> testtextVec;
    QVector<QRadioButton*> testradioVec1;
    QVector<QRadioButton*> testradioVec2;

    if(index == 0) ////传感器检测保存
    {
        QVector<QLabel*> testlineVecSon{ui->lab_1,ui->lab_2,ui->lab_3,ui->lab_4,ui->lab_5,ui->lab_6};
        QVector<QTextEdit*> testtextVecSon{ui->text_1,ui->text_2,ui->text_3,ui->text_4,ui->text_5,ui->text_6};
        QVector<QRadioButton*> testradioVec1Son{ui->rad_1,ui->rad_3,ui->rad_5,ui->rad_7,ui->rad_9,ui->rad_11};
        QVector<QRadioButton*> testradioVec2Son{ui->rad_2,ui->rad_4,ui->rad_6,ui->rad_8,ui->rad_10,ui->rad_12};
        std::copy(testlineVecSon.begin(),testlineVecSon.end(),std::back_inserter(testlineVec));
        std::copy(testtextVecSon.begin(),testtextVecSon.end(),std::back_inserter(testtextVec));
        std::copy(testradioVec1Son.begin(),testradioVec1Son.end(),std::back_inserter(testradioVec1));
        std::copy(testradioVec2Son.begin(),testradioVec2Son.end(),std::back_inserter(testradioVec2));
    }
//    else if(index == 1) ////换挡 控制检测保存
//    {
//        QVector<QLabel*> testlineVecSon{ui->lab_7,ui->lab_8};
//        QVector<QTextEdit*> testtextVecSon{ui->text_7,ui->text_8};
//        QVector<QRadioButton*> testradioVec1Son{ui->rad_13,ui->rad_15};
//        QVector<QRadioButton*> testradioVec2Son{ui->rad_14,ui->rad_16};
//        std::copy(testlineVecSon.begin(),testlineVecSon.end(),std::back_inserter(testlineVec));
//        std::copy(testtextVecSon.begin(),testtextVecSon.end(),std::back_inserter(testtextVec));
//        std::copy(testradioVec1Son.begin(),testradioVec1Son.end(),std::back_inserter(testradioVec1));
//        std::copy(testradioVec2Son.begin(),testradioVec2Son.end(),std::back_inserter(testradioVec2));
//    }
//    else if(index == 2) ///变矩器闭锁控制检测保存
//    {
//        QVector<QTextEdit*> testtextVecSon{ui->text_9,ui->text_10};
//        QVector<QLabel*> testlineVecSon{ui->lab_9,ui->lab_10};
//        QVector<QRadioButton*> testradioVec1Son{ui->rad_17,ui->rad_19};
//        QVector<QRadioButton*> testradioVec2Son{ui->rad_18,ui->rad_20};
//        std::copy(testlineVecSon.begin(),testlineVecSon.end(),std::back_inserter(testlineVec));
//        std::copy(testtextVecSon.begin(),testtextVecSon.end(),std::back_inserter(testtextVec));
//        std::copy(testradioVec1Son.begin(),testradioVec1Son.end(),std::back_inserter(testradioVec1));
//        std::copy(testradioVec2Son.begin(),testradioVec2Son.end(),std::back_inserter(testradioVec2));
//    }
//    else if(index == 3) ///CAN总线传输检测
//    {
//        QVector<QLabel*> testlineVecSon{ui->lab_11,ui->lab_12};
//        QVector<QTextEdit*> testtextVecSon{ui->text_11,ui->text_12};
//        QVector<QRadioButton*> testradioVec1Son{ui->rad_21,ui->rad_23};
//        QVector<QRadioButton*> testradioVec2Son{ui->rad_22,ui->rad_24};
//        std::copy(testlineVecSon.begin(),testlineVecSon.end(),std::back_inserter(testlineVec));
//        std::copy(testtextVecSon.begin(),testtextVecSon.end(),std::back_inserter(testtextVec));
//        std::copy(testradioVec1Son.begin(),testradioVec1Son.end(),std::back_inserter(testradioVec1));
//        std::copy(testradioVec2Son.begin(),testradioVec2Son.end(),std::back_inserter(testradioVec2));
//    }

    QStringList sqllist;

    for(int i = 0 ; i < testlineVec.length() ; i++)
    {
        ResultSaving savres;

        savres.Partnumber = testinfoeresult.Partnumber;
        savres.Tester = testinfoeresult.Tester;
        savres.Partinformation = testinfoeresult.Partinformation;

        int index = testlineVec[i]->text().indexOf("检测");
        savres.Testproject = testlineVec[i]->text();
        //.remove(index+2,testlineVec[i]->text().length()-index-2);
        savres.Testdata = testtextVec[i]->toPlainText();
        savres.Testtime = STRDATETIME;

        if(testradioVec1[i]->isChecked())
            savres.Testresults ="正常";
        else  if(testradioVec2[i]->isChecked())
            savres.Testresults ="故障";
        else
            savres.Testresults ="未检测";

        ResultSavingVec.push_back(savres);

        QString sql{"insert into testdata ('partnumber','testproject','testdata','testresults','tester','testtime','partinformation')"
                    " values ""('"
                    +savres.Partnumber+"','"
                    +savres.Testproject+"','"
                    +savres.Testdata+"','"
                    +savres.Testresults+"','"
                    +savres.Tester+"','"
                    +savres.Testtime+"','"
                    +savres.Partinformation+"')"};

        sqllist.push_back(sql);
    }
    for(QString a : sqllist)
    {
        bool ret = InsertData(a);
        qDebug()<<"ret"<<a;

        if(!ret)
        {
            QMessageBox msgBox;
            msgBox.setText("数据保存失败.");
            msgBox.exec();
            return;
        }
    }

    QMessageBox msgBox;
    msgBox.setText("save success!");
    msgBox.exec();

    on_btn_2_clicked();
}

void ConclusionShow::on_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ShowTableWidget();
}

void ConclusionShow::on_btn_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
//删除保存的数据（按行删除）
void ConclusionShow::on_btn_5_clicked()
{
    if(selectrow == -1)
        return;
    //由于精确到秒，理论上来说，每条数据的检测时间都是唯一的！！！！！
    //由于有一个全部保存的功能，时间不再唯一，再加一条检测项目吧 20220822

    ResultSavingVec.remove(selectrow);

    QString testtime = ui->tableWidget->item(selectrow,6)->text();
    QString testpro = ui->tableWidget->item(selectrow,2)->text();
    QString sql{"delete from testdata where testtime = '"+testtime + "' and testproject = '"+ testpro + "'"};
    DeleteData(sql);
    selectrow = -1;
    ShowTableWidget();
}

void ConclusionShow::on_btn_6_clicked()
{
    QVector<QVector<QString>> tablewidgetitemstring;
    GetTablewidgetString(tablewidgetitemstring,ui->tableWidget);
    CreateReport(reportsavepath,DT,TabwidgetHeadlist,tablewidgetitemstring);
}

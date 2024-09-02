#include "report.h"

//#include "QtXlsx/QtXlsx"
#include "xlsxdocument.h"
#include "xlsxcell.h"
#include <QUrl>
QRegExp rx("(\\d+\\.\\d+)");
QRegExp rx1("(\\d+)");

void CreateReport(QString Savelocation, QString SaveName, QStringList headlist, QVector<QVector<QString> > itemstring)
{
    QStringList letterlist{"A","B","C","D","E","F","G","H","I","J","K"};
    QXlsx::Document xlsx;

    QList<int> ColumnWidthList{80,130,250,250,160,160,180,300};

    QXlsx::Format titleFormat;//标题单元格样式
    titleFormat.setFontSize(15);/*设置字体大小*/
    titleFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);//横向居中
    titleFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);//纵向居中
    titleFormat.setBorderStyle(QXlsx::Format::BorderThin);//边框样式
    titleFormat.setPatternBackgroundColor(QColor{189, 215, 238});
    titleFormat.setBottomBorderStyle(QXlsx::Format::BorderDouble);//边框样式

    //设置表的标题
    xlsx.mergeCells("A1:F1", titleFormat); /// 合并单元格 第一行1-5格合并成一格
    xlsx.write(1, 1,page_str); /// 给合并后的单元格写数据, 注意参数 (1，1，"title")

    //设置表格样式
    xlsx.write("A2", "检测人员",titleFormat);
    xlsx.write("C2", "检测时间",titleFormat);
    xlsx.write("A3", "车号",titleFormat);
    xlsx.write("C3", "部队编号",titleFormat);
    xlsx.write("E3", "部队名称",titleFormat);

    xlsx.write("A4", "检测项",titleFormat);
    xlsx.write("B4", "检测标准",titleFormat);
    xlsx.write("C4", "标准最小值",titleFormat);
    xlsx.write("D4", "标准最大值",titleFormat);
    xlsx.write("E4", "检测数值",titleFormat);
    xlsx.write("F4", "是否合格",titleFormat);

    xlsx.setRowHeight(1,33);
    xlsx.setColumnWidth(1, 6, 25); // 将A列到F列的列宽都设置为200

    if(itemstring.length() == 0)
        return;

    QXlsx::Format cellFormat;//标题单元格样式

    cellFormat.setFontSize(12);/*设置字体大小*/
    cellFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);//
    cellFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);//
    cellFormat.setBorderStyle(QXlsx::Format::BorderThin);//边框样式
    cellFormat.setTextWarp(true);//自动换行

    //检测人员
    xlsx.write("B2",itemstring[0][5],cellFormat);
    //检测时间
    xlsx.write("D2",itemstring[0][6],cellFormat);
    //车号
    xlsx.write("B3",itemstring[0][7],cellFormat);
    //部件编号
    xlsx.write("F3",itemstring[0][1],cellFormat);

    for(int i = 0 ; i < itemstring.length() ; i++)
    {
        for(int j = 0 ; j < itemstring[0].length() ; j++)
        {
            //检测项
            xlsx.write("A"+QString::number(i+5),itemstring[i][2],cellFormat);
            //检测标准
            if(itemstring[i][2] == "CANA总线输出状态信息检测")
            {
                xlsx.write("B"+QString::number(i+5),"CANA总线是否正常",cellFormat);
            }
            else if(itemstring[i][2] == "CANB总线输出状态信息检测")
            {
                xlsx.write("B"+QString::number(i+5),"CANB总线是否正常",cellFormat);
            }
            else if(itemstring[i][2] == "强制闭锁控制检测")
            {
                xlsx.write("B"+QString::number(i+5),"强制闭锁控制正常",cellFormat);
            }
            else if(itemstring[i][2] == "自动闭解锁控制检测")
            {
                xlsx.write("B"+QString::number(i+5),"自动闭解锁控制正常",cellFormat);
            }
            else if(itemstring[i][2] == "自动挂空挡控制检测")
            {
                xlsx.write("B"+QString::number(i+5),"自动挂空挡控制正常",cellFormat);
            }
            else if(itemstring[i][2] == "手动换挡控制检测")
            {
                xlsx.write("B"+QString::number(i+5),"手动换挡控制正常",cellFormat);
            }
            else if(itemstring[i][2] == "操纵精滤报警信号检测")
            {
                xlsx.write("B"+QString::number(i+5),"按钮开关正常控制设备",cellFormat);
            }
            else if(itemstring[i][2] == "风扇常闭信号与常开信号检测")
            {
                xlsx.write("B"+QString::number(i+5),"按钮开关正常控制设备",cellFormat);
            }
            else if(itemstring[i][2] == "补偿压力传感器检测")
            {
                xlsx.write("B"+QString::number(i+5),"误差≤1.5%",cellFormat);
                QString strValue = itemstring[i][3].trimmed().replace(" ","");
                rx.indexIn(strValue);
                double value = rx.cap(1).toDouble();
                double min = value-value*0.015;
                double max = value+value*0.015;
                xlsx.write("C"+QString::number(i+5),min,cellFormat);
                xlsx.write("D"+QString::number(i+5),max,cellFormat);
            }
            else if(itemstring[i][2] == "变速三轴传感器检测")
            {
                xlsx.write("B"+QString::number(i+5),"误差≤1.5%",cellFormat);
                QString strValue = itemstring[i][3].trimmed().replace(" ","");
                rx1.indexIn(strValue);
                double value = rx1.cap(1).toDouble();
                double min = value-value*0.015;
                double max = value+value*0.015;
                xlsx.write("C"+QString::number(i+5),min,cellFormat);
                xlsx.write("D"+QString::number(i+5),max,cellFormat);
            }
            else if(itemstring[i][2] == "油液温度传感器检测")
            {
              xlsx.write("B"+QString::number(i+5),"误差≤2%",cellFormat);
              QString strValue = itemstring[i][3].trimmed().replace(" ","");
              rx.indexIn(strValue);
              double value = rx.cap(1).toDouble();
              double min = value-value*0.02;
              double max = value+value*0.02;
              xlsx.write("C"+QString::number(i+5),min,cellFormat);
              xlsx.write("D"+QString::number(i+5),max,cellFormat);
            }
            else if(itemstring[i][2] == "操纵压力传感器检测")
            {
              xlsx.write("B"+QString::number(i+5),"误差≤1.5%",cellFormat);
              QString strValue = itemstring[i][3].trimmed().replace(" ","");
              rx.indexIn(strValue);
              double value = rx.cap(1).toDouble();
              double min = value-value*0.015;
              double max = value+value*0.015;
              xlsx.write("C"+QString::number(i+5),min,cellFormat);
              xlsx.write("D"+QString::number(i+5),max,cellFormat);
            }

            xlsx.write("E"+QString::number(i+5),itemstring[i][3],cellFormat);
            xlsx.write("E"+QString::number(i+5),itemstring[i][3],cellFormat);
            xlsx.write("F"+QString::number(i+5),itemstring[i][4],cellFormat);
        }
    }




    QString path =Savelocation + SaveName+".xlsx";
    xlsx.saveAs(path);

    QDesktopServices::openUrl(QUrl::fromLocalFile((path)));
}

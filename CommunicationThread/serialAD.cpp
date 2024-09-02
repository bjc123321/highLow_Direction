#include "serialAD.h"

QList<double> value_list{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; //1~16路数据
SerialAD::SerialAD(QWidget *parent) : QThread(parent)
{
    m_serial = new QSerialPort();
    m_serial->setPortName("COM4");
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    m_serial->open(QIODevice::ReadWrite);
    timer1.setTimerType(Qt::PreciseTimer);
    timer1.start(1000);
    connect(&timer1,QTimer::timeout,[=](){
        timer1Slot();
    });
}

void SerialAD::run()
{
    while (1) {
        msleep(1);
    }
}

void SerialAD::AD_16GetValue()
{
    QByteArray sdDataTemp;
    sdDataTemp[0]=0x01;
    sdDataTemp[1]=0x03;
    sdDataTemp[2]=0x00;
    sdDataTemp[3]=0x60;
    sdDataTemp[4]=0x00;
    sdDataTemp[5]=0x10;
    quint16 crc16=ModbusCRC16(sdDataTemp);
    sdDataTemp[6]=crc16;
    sdDataTemp[7]=crc16>>8;
    m_serial->write(sdDataTemp);
}

void SerialAD::ReceiveData()
{
    static QByteArray comData;
    while (!m_serial->atEnd())
    {
        comData += m_serial->readAll().toHex();
        com_16BatAnalyze(comData);
    }
}

void SerialAD::com_16BatAnalyze(QByteArray &allData)
{
    bool ok;
    QByteArray dataTemp;
    int len;
    int start;
    int move;
    int tempValue;
    dataTemp=allData;
    while((len=dataTemp.count())>=37*2){
        start=dataTemp.indexOf("010320",0);
        if(start>=0){
            len=len-start;
            dataTemp=dataTemp.right(len);
            if(len>=37*2){
                if(dataTemp.mid(0,6)=="010320"){
                    for(int i = 0 ; i < 16; i++)
                    {
                        QString hex = dataTemp.mid(6+i*4,4);
                        tempValue =  hex.toInt(&ok,16);
                        value_list[i] = tempValue/1000.00;
                    }
                    emit sendValue(value_list);
                    move=37*2;
                }
                dataTemp=dataTemp.right(len-move);
            }else{
                break;
            }
        }else{
            dataTemp.clear();
            break;
        }
    }
    allData=dataTemp;
}

void SerialAD::timer1Slot()
{
    AD_16GetValue();
}

quint16 SerialAD::ModbusCRC16(QByteArray senddata)
{
    int len=senddata.size();
    quint16 wcrc=0xFFFF;//预置16位crc寄存器，初值全部为1
    quint8 temp;//定义中间变量
    int i=0,j=0;//定义计数
    for(i=0;i<len;i++)//循环计算每个数据
    {
       temp=senddata.at(i);
       wcrc^=temp;
       for(j=0;j<8;j++){
          //判断右移出的是不是1，如果是1则与多项式进行异或。
          if(wcrc&0x0001){
              wcrc>>=1;//先将数据右移一位
              wcrc^=0xA001;//与上面的多项式进行异或
          }
          else//如果不是1，则直接移出
              wcrc>>=1;//直接移出
       }
    }
    temp=wcrc;//crc的值
    return wcrc;
}

void SerialAD::ADSendStr(QString str)
{
    bool ok;
    sdData.clear();
    str.replace(" ","");
    for(int i=0;i<str.length();i+=2){
        sdData[i/2]=str.mid(i,2).toInt(&ok,16);
    }
    m_serial->write(sdData);
}

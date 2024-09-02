#include "serialDA.h"

SerialDA::SerialDA(QWidget *parent) : QThread(parent)
{
    m_serial = new QSerialPort();
    m_serial->setPortName("COM5");
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    m_serial->open(QIODevice::ReadWrite);

}

void SerialDA::run()
{
    while (1) {
        msleep(1);
    }
}

void SerialDA::ReceiveData()
{

}

void SerialDA::setVol_channel(int channel, double vol)
{
    int value=vol*1000;
    QByteArray sdDataTemp;
    sdDataTemp[0]=0x01;
    sdDataTemp[1]=0x06;
    sdDataTemp[2]=0x00;
    sdDataTemp[3]=21+channel;
    sdDataTemp[4]=value>>8;
    sdDataTemp[5]=value;
    quint16 crc16=ModbusCRC16(sdDataTemp);
    sdDataTemp[6]=crc16;
    sdDataTemp[7]=crc16>>8;
    m_serial->write(sdDataTemp);
}

void SerialDA::setCur_channel(int channel, double Cur)
{
    int value=Cur*1000;
    QByteArray sdDataTemp;
    sdDataTemp[0]=0x02;
    sdDataTemp[1]=0x06;
    sdDataTemp[2]=0x00;
    sdDataTemp[3]=21+channel;
    sdDataTemp[4]=value>>8;
    sdDataTemp[5]=value;
    quint16 crc16=ModbusCRC16(sdDataTemp);
    sdDataTemp[6]=crc16;
    sdDataTemp[7]=crc16>>8;
    m_serial->write(sdDataTemp);
}

quint16 SerialDA::ModbusCRC16(QByteArray senddata)
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
    return  wcrc;
}

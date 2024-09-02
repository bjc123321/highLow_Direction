#include "serialIO.h"

QList<bool> DI_state{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
SerialIO::SerialIO(QWidget *parent) : QThread(parent)
{
    m_serial = new QSerialPort();
    m_serial->setPortName("COM3");
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    m_serial->open(QIODevice::ReadWrite);
    timer1.setTimerType(Qt::PreciseTimer);
    //timer1.start(1000);
    connect(&timer1,QTimer::timeout,[=](){
        timer1Slot();
    });

}

void SerialIO::run()
{
    while (1) {
        msleep(1);
    }
}

void SerialIO::timer1Slot()
{
    DISendStr();//读DI状态
}

void SerialIO::ReceiveData()
{

    comData += m_serial->readAll().toHex();
    comBatAnalyze(comData);
//    qDebug()<<"eeeeeeeeeeeeeee"<<comData.size();
}

void SerialIO::DISendStr()
{
    QByteArray sdData;
    sdData[0]=0x02;
    sdData[1]=0x03;
    sdData[2]=0x00;
    sdData[3]=0x20;
    sdData[4]=0x00;
    sdData[5]=0x03;
    sdData[6]=0x04;
    sdData[7]=0x32;
    m_serial->write(sdData);
}

void SerialIO::set_channel(int channel, bool flag)
{
    //qDebug()<<"channel"<<channel<<"flag"<<flag;
    //timer1.stop();
    QByteArray sdDataTemp;
    sdDataTemp[0]=0x01;
    sdDataTemp[1]=0x05;
    sdDataTemp[2]=0x00;

    sdDataTemp[3]=31+channel;
    if(flag){
        sdDataTemp[4]=0xff;
    }else{
        sdDataTemp[4]=0x00;
    }
    sdDataTemp[5]=0x00;
    quint16 crc16=ModbusCRC16(sdDataTemp);
    sdDataTemp[6]=crc16;
    sdDataTemp[7]=crc16>>8;
    m_serial->write(sdDataTemp);
    //timer1.start(1000);
}

quint16 SerialIO::ModbusCRC16(QByteArray senddata)
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

void SerialIO::comBatAnalyze(QByteArray &allData)
{
    bool ok;
    QByteArray dataTemp;
    int len;
    int start;
    int move;
    int tempValue;
    dataTemp=allData;
    while((len=dataTemp.count())>=11*2){
        start=dataTemp.indexOf("020306",0);
        if(start>=0){
            len=len-start;
            dataTemp=dataTemp.right(len);
            if(len>=11*2){
                move=22;
                if(allData.mid(0,6)=="020306"){
                    bool ok;
                    unsigned short int dataTemp=allData.mid(14,4).toUShort(&ok,16);
                    for(int i=0;i<16;i++){
                        if(((dataTemp>>i)&0x01)==1){
                            DI_state[i]=true;
                        }else{
                            DI_state[i]=false;
                        }
                    }
                    emit sendDIState(DI_state);
                }
                dataTemp=dataTemp.right(len-move);
            }else{
                break;
            }
        }else{
            allData.clear();
            break;
        }
    }
    allData=dataTemp;
}

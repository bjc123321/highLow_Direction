#include "serialthread.h"

QList<bool> state_list{false,false,false,false,false,false};
SerialThread::SerialThread(QWidget *parent) : QThread(parent)
{
    m_serial = new QSerialPort();
    m_serial->setPortName("COM6");
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    m_serial->open(QIODevice::ReadWrite);
    timer1.setTimerType(Qt::PreciseTimer);
    timer1.start(500);
    connect(&timer1,QTimer::timeout,[=](){
        timer1Slot();
    });
}

void SerialThread::run()
{
    while (1) {
        msleep(1);
    }
}

void SerialThread::timer1Slot()
{
    QByteArray sdDataTemp;
    sdDataTemp[0]=0x01;
    sdDataTemp[1]=0x03;
    sdDataTemp[2]=0x00;
    sdDataTemp[3]=0x00;
    sdDataTemp[4]=0x00;
    sdDataTemp[5]=0x07;
    quint16 crc16=ModbusCRC16(sdDataTemp);
    sdDataTemp[6]=crc16;
    sdDataTemp[7]=crc16>>8;
    m_serial->write(sdDataTemp);
}

void SerialThread::powerState(bool state)
{
    if(state){
        timer1.stop();
        sleep(100);
        //powerStart();
        timer1.start(1000);
    }else{
        timer1.stop();
        sleep(100);
        powerStop();
        timer1.start(1000);
    }
}

void SerialThread::setPowerDC(float voltage,float current, bool state)
{
    int cur = current*1000;
    int vol = voltage*1000;
    QByteArray sdDataTemp;
    timer1.stop();
    sleep(100);

    sdDataTemp[0]=0x01;
    sdDataTemp[1]=0x10;
    sdDataTemp[2]=0x00;
    sdDataTemp[3]=0x07;
    sdDataTemp[4]=0x00;
    sdDataTemp[5]=0x05;
    sdDataTemp[6]=0x0A;
    sdDataTemp[7]=vol>>24;
    sdDataTemp[8]=vol>>16;
    sdDataTemp[9]=vol>>8;
    sdDataTemp[10]=vol;
    sdDataTemp[11]=cur>>24;
    sdDataTemp[12]=cur>>16;
    sdDataTemp[13]=cur>>8;
    sdDataTemp[14]=cur;
    if(state){
        sdDataTemp[15]=0xFF;
    }else{
        sdDataTemp[15]=0x00;
    }
    sdDataTemp[16]=0x00;
    quint16 crc16=ModbusCRC16(sdDataTemp);
    sdDataTemp[17]=crc16;
    sdDataTemp[18]=crc16>>8;
    m_serial->write(sdDataTemp);
    timer1.start(500);
}

void SerialThread::setPowerVorC(float voltage,float current, bool state)
{
    int cur = current*1000;
    int vol = voltage*1000;
    QByteArray sdDataTemp;
    timer1.stop();
    sleep(100);
    if(state){
        sdDataTemp[0]=0x01;
        sdDataTemp[1]=0x10;
        sdDataTemp[2]=0x00;
        sdDataTemp[3]=0x07;
        sdDataTemp[4]=0x00;
        sdDataTemp[5]=0x02;
        sdDataTemp[6]=0x04;
        sdDataTemp[7]=vol>>24;
        sdDataTemp[8]=vol>>16;
        sdDataTemp[9]=vol>>8;
        sdDataTemp[10]=vol;
        quint16 crc16=ModbusCRC16(sdDataTemp);
        sdDataTemp[11]=crc16;
        sdDataTemp[12]=crc16>>8;
        m_serial->write(sdDataTemp);
    }else{
        sdDataTemp[0]=0x01;
        sdDataTemp[1]=0x10;
        sdDataTemp[2]=0x00;
        sdDataTemp[3]=0x09;
        sdDataTemp[4]=0x00;
        sdDataTemp[5]=0x02;
        sdDataTemp[6]=0x04;
        sdDataTemp[7]=cur>>24;
        sdDataTemp[8]=cur>>16;
        sdDataTemp[9]=cur>>8;
        sdDataTemp[10]=cur;
        quint16 crc16=ModbusCRC16(sdDataTemp);
        sdDataTemp[11]=crc16;
        sdDataTemp[12]=crc16>>8;
        m_serial->write(sdDataTemp);
    }
    timer1.start(500);
}

void SerialThread::powerStart26()
{
    timer1.stop();
    sleep(100);
    QByteArray sdDataTemp;
    sdDataTemp[0]=0x01;
    sdDataTemp[1]=0x10;
    sdDataTemp[2]=0x00;
    sdDataTemp[3]=0x07;
    sdDataTemp[4]=0x00;
    sdDataTemp[5]=0x05;
    sdDataTemp[6]=0x0A;
    sdDataTemp[7]=0x00;
    sdDataTemp[8]=0x00;
    sdDataTemp[9]=0x65;
    sdDataTemp[10]=0x90;
    sdDataTemp[11]=0x00;
    sdDataTemp[12]=0x00;
    sdDataTemp[13]=0x00;
    sdDataTemp[14]=0x00;
    sdDataTemp[15]=0xFF;
    sdDataTemp[16]=0x00;
    sdDataTemp[17]=0x10;
    sdDataTemp[18]=0xD1;
//    quint16 crc16=ModbusCRC16(sdDataTemp);
//    sdDataTemp[9]=crc16;
//    sdDataTemp[10]=crc16>>8;
    m_serial->write(sdDataTemp);
    timer1.start(500);
    qDebug()<<"外设电源电压启动："<<sdDataTemp;

}
void SerialThread::powerStart110()
{
    timer1.stop();
    sleep(100);
    QByteArray sdDataTemp;
    sdDataTemp[0]=0x01;
    sdDataTemp[1]=0x10;
    sdDataTemp[2]=0x00;
    sdDataTemp[3]=0x07;
    sdDataTemp[4]=0x00;
    sdDataTemp[5]=0x05;
    sdDataTemp[6]=0x0A;
    sdDataTemp[7]=0x00;
    sdDataTemp[8]=0x01;
    sdDataTemp[9]=0xAD;
    sdDataTemp[10]=0xB0;
    sdDataTemp[11]=0x00;
    sdDataTemp[12]=0x00;
    sdDataTemp[13]=0x00;
    sdDataTemp[14]=0x00;
    sdDataTemp[15]=0xFF;
    sdDataTemp[16]=0x00;
    sdDataTemp[17]=0x31;
    sdDataTemp[18]=0x75;
//    quint16 crc16=ModbusCRC16(sdDataTemp);
//    sdDataTemp[9]=crc16;
//    sdDataTemp[10]=crc16>>8;
    m_serial->write(sdDataTemp);
    timer1.start(500);
    qDebug()<<"外设电源电压启动："<<sdDataTemp;
}
void SerialThread::powerStop()
{
    timer1.stop();
    sleep(100);
    QByteArray sdDataTemp;
    sdDataTemp[0]=0x01;
    sdDataTemp[1]=0x10;
    sdDataTemp[2]=0x00;
    sdDataTemp[3]=0x0B;
    sdDataTemp[4]=0x00;
    sdDataTemp[5]=0x01;
    sdDataTemp[6]=0x02;
    sdDataTemp[7]=0x00;
    sdDataTemp[8]=0x00;
    quint16 crc16=ModbusCRC16(sdDataTemp);
    sdDataTemp[9]=crc16;
    sdDataTemp[10]=crc16>>8;
    m_serial->write(sdDataTemp);
    timer1.start(500);

    qDebug()<<"外设电源电压停止："<<sdDataTemp;

}

void SerialThread::ReceiveData()
{
    //原始数据
    QByteArray  buffer= m_serial->readAll();
    QString strLog = buffer.toHex().toUpper();
//    qDebug()<<QDateTime::currentDateTime().toString("HH:mm:ss.zzz")<<"com6 rec:"<<strLog;

    strRec6 += strLog;

    if(!strRec6.startsWith("0103"))
        strRec6 = strRec6.right(strRec6.indexOf("0103"));
    if(strRec6.size()==8) qDebug()<<strRec6;
    if(strRec6.size()>37)
        strRec6 = strRec6.mid(strRec6.indexOf("0103"),37);
//    qDebug()<<QDateTime::currentDateTime().toString("HH:mm:ss.zzz")<<"com6 recALL:"<<strRec6;

    if(strRec6.startsWith("0103") && strRec6.size()>=34)
    {
        //电压
        QString strdy = strRec6.mid(6,8);
        double dy = (double)strdy.toInt(0,16);
        QString dy1 = QString::number(dy/1000,'f',3);
        //电流
        QString strdl = strRec6.mid(14,8);
        double dl = (double)strdl.toInt(0,16);
        QString dl1 = QString::number(dl/1000,'f',3);
        //功率
        QString strgl = strRec6.mid(22,8);
        double gl = (double)strgl.toInt(0,16);
        QString gl1 = QString::number(gl/1000,'f',3);
        emit sendPowerValue(dy1, dl1,gl1);

        //电源状态
        uchar byteState=strRec6.mid(32,2).toUShort(0,16);
        if(((byteState&0x20)>>5)==1){
            //1 主板
            state_list[5]=true;
        }else{
            state_list[5]=false;
        }
        if(((byteState&0x10)>>4)==1){
            //1 电压低
            state_list[4]=true;
        }else{
            state_list[4]=false;
        }
        if(((byteState&0x08)>>3)==1){
            //1 缺相
            state_list[3]=true;
        }else{
            state_list[3]=false;
        }
        if(((byteState&0x04)>>2)==1){
            //1 超温
            state_list[2]=true;
        }else{
            state_list[2]=false;
        }
        if(((byteState&0x02)>>1)==1){
            //1 稳压模式
            state_list[1]=true;
        }else{
            // 0 稳流模式
            state_list[1]=false;
        }
        if((byteState&0x01)==1){
            //1 工作状态 停机状态
            state_list[0]=true;
        }else{
            state_list[0]=false;
        }
        emit sendPowerState(state_list);
        strRec6="";
    }
}

quint16 SerialThread::ModbusCRC16(QByteArray senddata)
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

bool SerialThread::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    return true;
}

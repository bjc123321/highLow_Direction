#include "serialpowerthread.h"

SerialPowerThread::SerialPowerThread(QWidget *parent) : QThread(parent)
{
    m_serial = new QSerialPort();
    m_serial->setPortName("COM2");
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

void SerialPowerThread::run()
{
    while (1) {
        msleep(1);
    }
}

void SerialPowerThread::timer1Slot()
{
    powerSendStr("01 03 00 04 00 04 05 C8");//读电压电流
}

void SerialPowerThread::powerState(int state)
{
    if(state){
        timer1.stop();
        sleep(100);
        powerStart();
        timer1.start(100);
    }else{
        timer1.stop();
        sleep(100);
        powerStop();
        timer1.start(100);
    }
}

void SerialPowerThread::setPowerDC(float voltage,float current)
{
    timer1.stop();
    sleep(100);
    //01 10 00 0A 00 04 08 00 00 42 70 00 00 41 F0 D0 9F //60V 30A
    uchar temp[17]={0x01,0x10,0x00,0x0A,0x00,0x04,0x08,0x00,0x00,0x41,0xD0,0x00,0x00,0x42,0xC8,0xD0,0x9F};
    uchar *aa=(uchar *)(&voltage);
    temp[7]=aa[1];
    temp[8]=aa[0];
    temp[9]=aa[3];
    temp[10]=aa[2];
    aa=(uchar *)(&current);
    temp[11]=aa[1];
    temp[12]=aa[0];
    temp[13]=aa[3];
    temp[14]=aa[2];

    CRC_Check(temp,15,&temp[15]);

    sdData.clear();
    for(int i=0;i<17;i++){
        sdData[i]=temp[i];
    }
    m_serial->write(sdData);
    timer1.start(100);
}

void SerialPowerThread::powerStart()
{
    timer1.stop();
    sleep(100);
    QString str="01 10 00 09 00 01 02 00 03 E6 C8";
    powerSendStr(str);
    timer1.start(100);

}

void SerialPowerThread::powerStop()
{
//    QString str1="01 10 00 0A 00 04 08 00 00 41 D0 00 00 42 C8 51 97 ";
//    powerSendStr(str1);
    timer1.stop();
    sleep(100);
    QString str2="01 10 00 09 00 01 02 00 02 27 08";
    powerSendStr(str2);
    timer1.start(100);

}

void SerialPowerThread::powerSet()
{
    QString str="01 10 00 0A 00 04 08 00 00 41 D0 00 00 42 C8 51 97";
    powerSendStr(str);
}
void SerialPowerThread::ReceiveData()
{
    static QByteArray strData;
    strData += m_serial->readAll().toHex();
    comBatAnalyze(strData);
}

void SerialPowerThread::comBatAnalyze(QByteArray &allData)
{
    QByteArray dataTemp;
    int len;
    int start;
    int move;
    float *temp;
    dataTemp=allData;
    while((len=dataTemp.count())>=13*2){
        move=2;
        start=dataTemp.indexOf("010308");
        if(start>=0){
            len=len-start;
            dataTemp=dataTemp.right(len);
            if(len>=13*2){
                if(dataTemp.mid(0,6)=="010308"){
                    if(len>=26){
                        //01 03 08 F7 54 42 81 D0 30 3D 25 14 08
                        unsigned char aa[4];
                        bool ok;
                        aa[3]=dataTemp.mid(10,2).toInt(&ok,16);
                        aa[2]=dataTemp.mid(12,2).toInt(&ok,16);
                        aa[1]=dataTemp.mid(6,2).toInt(&ok,16);
                        aa[0]=dataTemp.mid(8,2).toInt(&ok,16);
                        temp=(float *)aa;
                        float vol=*temp;

                        aa[3]=dataTemp.mid(18,2).toInt(&ok,16);
                        aa[2]=dataTemp.mid(20,2).toInt(&ok,16);
                        aa[1]=dataTemp.mid(14,2).toInt(&ok,16);
                        aa[0]=dataTemp.mid(16,2).toInt(&ok,16);
                        temp=(float *)aa;
                        float cur=*temp;

                        move=26;
                        emit sendPowerValue(vol, cur);
                    }else{break;}
                }
                dataTemp=dataTemp.right(len-move);
            }
            else{

                break;
            }
        }
        else{
            dataTemp.clear();
            break;
        }
    }
    while((len=dataTemp.count())==16&&flag){
        move=2;
        start=dataTemp.indexOf("011000");
        if(start>=0){
            len=len-start;
            dataTemp=dataTemp.right(len);
            if(len>=16){
                if(dataTemp.mid(0,6)=="011000"){
                    if(len>=16){

                        move=16;
                        emit PowerRet(QString(allData));
                        flag=false;
                    }else{break;}
                }
                dataTemp=dataTemp.right(len-move);
            }
            else{

                break;
            }
        }
        else{
            dataTemp.clear();
            break;
        }
    }
    allData=dataTemp;
}

void SerialPowerThread::CRC_Check(uchar *buff,uchar len,uchar *res)//buff 数据，len 长度，res 结果
{
    unsigned short tmp = 0xffff;
    for(int n = 0; n < len; n++){/*此处的6 -- 要校验的位数为6个*/
        tmp = buff[n] ^ tmp;
        for(int i = 0;i < 8;i++){  /*此处的8 -- 指每一个char类型又8bit，每bit都要处理*/
            if(tmp & 0x01){
                tmp = tmp >> 1;
                tmp = tmp ^ 0xa001;
            }
            else{
                tmp = tmp >> 1;
            }
        }
    }
    res[0] = tmp;
    res[1] = tmp>>8;
}

void SerialPowerThread::powerSendStr(QString str)
{
    bool ok;
    sdData.clear();
    str.replace(" ","");
    for(int i=0;i<str.length();i+=2){
        sdData[i/2]=str.mid(i,2).toInt(&ok,16);
    }
    //qDebug()<<sdData;
    m_serial->write(sdData);

}

bool SerialPowerThread::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    return true;
}

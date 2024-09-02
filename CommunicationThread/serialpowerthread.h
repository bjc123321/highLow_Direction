#ifndef SERIALPOWERTHREAD_H
#define SERIALPOWERTHREAD_H

#include <QWidget>
#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <bitset>
#include <QTime>
#include <QTimer>
#include <QCoreApplication>

class SerialPowerThread : public QThread
{
    Q_OBJECT
public:
    explicit SerialPowerThread(QWidget *parent = nullptr);

    void run();
    void powerState(int state);
    void setPowerDC(float voltage,float current);
    void powerStart();
    void powerStop();
    void powerSet();

    bool flag=false;
signals:
    void sendPowerValue(float vol, float cur);
    void PowerRet(QString ret);

private slots:
    void ReceiveData();
    //void powerStart();
    void powerSendStr(QString str);
    bool sleep(unsigned int msec);
    void timer1Slot();
    void CRC_Check(uchar *buff,uchar len,uchar *res);
    void comBatAnalyze(QByteArray &allData);

private:
    QSerialPort *m_serial;
    QByteArray sdData;
    QTimer timer1;

signals:

public slots:
};

#endif // SERIALPOWERTHREAD_H

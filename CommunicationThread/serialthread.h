#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QWidget>
#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <bitset>
#include <QTime>
#include <QTimer>
#include <QCoreApplication>

class SerialThread : public QThread
{
    Q_OBJECT
public:
    explicit SerialThread(QWidget *parent = nullptr);

    void run();

    void powerState(bool state);

    void setPowerDC(float voltage,float current, bool state);
    void setPowerVorC(float voltage,float current, bool state);
    void powerStart26();
    void powerStart110();
    void powerStop();
signals:
    void sendPowerValue(QString vol, QString cur, QString pow);
    void sendPowerState(QList<bool> state);
private slots:
    void ReceiveData();
    void timer1Slot();
    quint16 ModbusCRC16(QByteArray senddata);


    bool sleep(unsigned int msec);

private:
    QSerialPort *m_serial;
    QByteArray sdData;
    QTimer timer1;
    QString strRec6;

signals:

public slots:
};

#endif // SERIALTHREAD_H
